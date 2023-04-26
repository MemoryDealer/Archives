/*
 * System Controls v2.0 Written by Sparky
 *          Copyright (C) 2009
 */
#include "main.h"

//WNDCLSEX Names
LPCTSTR ClsName = "SysCtrls_Class";
LPCTSTR WndName = "System Controls 2.0";


//Dialog Stuff
HBRUSH g_hbrBackground = NULL;
HBRUSH textCtrl = NULL;


//Control Window Declarations->

/*Icon Buttons*/
HWND BUTTON_EXPLORER;
HWND BUTTON_CMD;
HWND BUTTON_NOTEPAD;
HWND BUTTON_OTHER_HELP;


/*Enable Buttons*/
HWND BUTTON_E_TASKMGR;
HWND BUTTON_E_RUN;
HWND BUTTON_E_REGEDIT;
HWND BUTTON_E_DISPLAY_PROPERTIES;
HWND BUTTON_E_CMD;
HWND BUTTON_E_DRIVE_C;

HWND BUTTON_E_CHANGE_BACKGROUND;
HWND BUTTON_E_CONTROL_PANEL;
HWND BUTTON_E_BALLOON_TIPS;
HWND BUTTON_E_FOLDER_OPTIONS;
HWND BUTTON_E_DESKTOP_ITEMS;
HWND BUTTON_E_LAST_GOOD_CONFIG;

/*Disable Buttons*/
HWND BUTTON_D_TASKMGR;
HWND BUTTON_D_RUN;
HWND BUTTON_D_REGEDIT;
HWND BUTTON_D_DISPLAY_PROPERTIES;
HWND BUTTON_D_CMD;
HWND BUTTON_D_DRIVE_C;

HWND BUTTON_D_CHANGE_BACKGROUND;
HWND BUTTON_D_CONTROL_PANEL;
HWND BUTTON_D_BALLOON_TIPS;
HWND BUTTON_D_FOLDER_OPTIONS;
HWND BUTTON_D_DESKTOP_ITEMS;
HWND BUTTON_D_LAST_GOOD_CONFIG;

/*Other Buttons*/
HWND EDIT_TEXTBOX;
HWND BUTTON_PING;
HWND BUTTON_TRACEROUTE;
HWND BUTTON_SEARCH_FILE;
HWND BUTTON_CHECKDISK;
HWND BUTTON_DEFRAG;
HWND BUTTON_NS_LOOKUP;
HWND BUTTON_TELNET;

HWND COMBOBOX_BOOTSHELL;
HWND BUTTON_SET_BOOTSHELL;
HWND EDIT_SET_STARTBUTTON;
HWND BUTTON_SET_STARTBUTTON;



char *buffer; //Global char for all functions

LRESULT CALLBACK WndProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgAboutProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgRunProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgInstallerProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgPingLoopProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgSetAdminPassProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgTerminateProcProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void CreateGUIControls(const HWND hWnd, CREATESTRUCT* cstruct);

enum{ //Enumerate Control ID's

    //Icon buttons
    ID_EXPLORER,
    ID_CMD,
    ID_NOTEPAD,
    ID_OTHER_HELP,

    ID_STATICTEXT_ENABLE,

    //Enable Buttons:
    ID_E_TASKMGR,
    ID_E_RUN,
    ID_E_REGEDIT,
    ID_E_DISPLAY_PROPERTIES,
    ID_E_CMD,
    ID_E_DRIVE_C,
    ID_E_CHANGE_BACKGROUND,
    ID_E_CONTROL_PANEL,
    ID_E_BALLOON_TIPS,
    ID_E_FOLDER_OPTIONS,
    ID_E_DESKTOP_ITEMS,
    ID_E_LAST_GOOD_CONFIG,

    ID_STATICRECT1,
    ID_STATICTEXT_DISABLE,

    //Disable Buttons
    ID_D_TASKMGR,
    ID_D_RUN,
    ID_D_REGEDIT,
    ID_D_DISPLAY_PROPERTIES,
    ID_D_CMD,
    ID_D_DRIVE_C,
    ID_D_CHANGE_BACKGROUND,
    ID_D_CONTROL_PANEL,
    ID_D_BALLOON_TIPS,
    ID_D_FOLDER_OPTIONS,
    ID_D_DESKTOP_ITEMS,
    ID_D_LAST_GOOD_CONFIG,

    ID_STATICRECT2,

    ID_STATICTEXT_OTHER,
    //Other Buttons
    ID_TEXTBOX,
    ID_PING,
    ID_TRACEROUTE,
    ID_SEARCH_FILE,
    ID_CHECKDISK,
    ID_DEFRAG,
    ID_NS_LOOKUP,
    ID_PING_LOOP,
    ID_TELNET,

    ID_CB_BOOTSHELL,
    ID_SET_BOOTSHELL,
    ID_STATICTEXT_SCROLLHERE,
    ID_STATICTEXT_BOOTSHELL,
    ID_SET_STARTMENU,
    ID_EDIT_SET_STARTMENU,
    ID_STATICTEXT_STARTMENU
    };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nFunsterStil){

     MSG msg;
     HWND hWnd;
     WNDCLASSEX wcx;

     //Fill wcx struct
     wcx.cbSize         = sizeof(WNDCLASSEX);
     wcx.style          = CS_HREDRAW | CS_VREDRAW;
     wcx.lpfnWndProc    = WndProcedure;
     wcx.cbClsExtra     = 0;
     wcx.cbWndExtra     = 0;
     wcx.hIcon          = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
     wcx.hCursor        = LoadCursor(NULL, IDC_ARROW);
     wcx.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
     wcx.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU);
     wcx.lpszClassName  = ClsName;
     wcx.hInstance      = hInstance;
     wcx.hIconSm        = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONSM), IMAGE_ICON, 0, 0, 0);

     //Register the window class
     if(!RegisterClassEx(&wcx)){

            MessageBox(NULL,"Window Registration Failed.","Error",MB_OK | MB_ICONHAND);

            }

     //Create Main Window
     hWnd = CreateWindowEx(0,
                           ClsName,
                           WndName,
                           WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                           145,
                           95,
                           950,
                           600,
                           NULL,
                           NULL,
                           hInstance,
                           NULL);

                        //If the window failed
                        if(!hWnd){

                            MessageBox(NULL,"Window Creation Failed.",
                            "Error",MB_OK | MB_ICONHAND);

                            }

      //Show the main window
      ShowWindow(hWnd, SW_SHOWNORMAL);
      UpdateWindow(hWnd);


      while(GetMessage(&msg, NULL, 0, 0)){

                TranslateMessage(&msg);
                DispatchMessage(&msg);

            }

      return msg.wParam;

}

LRESULT CALLBACK WndProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    int len = 0;
    switch(msg){

        case WM_COMMAND: //For control events

            switch(LOWORD(wParam)){

                /*
                 *Menu Events
                 */
                case ID_FILE_OPEN:
                    OPENFILENAME ofn;
                    buffer = (char*)GlobalAlloc(GPTR,MAX_PATH);

                    ZeroMemory(&ofn,sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
                    ofn.lpstrFile = buffer;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
                    ofn.lpstrDefExt = "*";

                    if(GetOpenFileName(&ofn))
                    {
                        ShellExecute(NULL,"open",buffer,NULL,NULL,SW_SHOWNORMAL);
                    }

                    buffer = "";
                    GlobalFree((HANDLE)buffer);
                break;

                case ID_FILE_QUIT:
                    PostQuitMessage(WM_QUIT);
                    return 0;
                    break;


                //Run Dialog
                case ID_TOOLS_RUN:
                {
                    DialogBox(GetModuleHandle(NULL),
                        MAKEINTRESOURCE(DLG_RUN),hWnd,DlgRunProcedure);

                }
                break;

                //Terminate Process Dialog
                case ID_TOOLS_TERMINATE_PROC:
                {
                    DialogBox(GetModuleHandle(NULL),
                        MAKEINTRESOURCE(DLG_TERMINATE_PROC),hWnd,DlgTerminateProcProcedure);
                }
                break;

                //Install Dialog
                case ID_TOOLS_INSTALLER:
                {
                      DialogBox(GetModuleHandle(NULL),
                        MAKEINTRESOURCE(DLG_INSTALLER),hWnd,DlgInstallerProcedure);
                }
                break;

                //Renew IP Function
                case ID_TOOLS_RENEW_IP:
                {
                    RenewIP();
                    MessageBox(NULL,"IP Address Renewed.","System Controls",MB_OK | MB_ICONINFORMATION);
                }
                break;

                //Set Admin Pass Dialog
                case ID_TOOLS_SET_ADMIN_PASS:
                {
                    DialogBox(GetModuleHandle(NULL),
                        MAKEINTRESOURCE(DLG_SET_ADMIN_PASS),hWnd,DlgSetAdminPassProcedure);
                }
                break;

                //Disable Safe Mode Function
                case ID_TOOLS_DISABLE_SAFE_MODE:
                {
                    int msg =
                    MessageBox(NULL,"Are you sure you want to remove safe mode?","",MB_YESNO | MB_ICONEXCLAMATION);

                    if(msg == IDYES)
                    {
                        msg = MessageBox(NULL,"You cannot restore it with this program, proceed?","",MB_YESNO | MB_ICONSTOP);

                        if(msg == IDYES)
                        {
                            RemoveSafeMode();
                        }
                        else
                        {
                            MessageBox(NULL,"Function halted.","",MB_OK);
                        }
                    }
                    else
                    {
                        MessageBox(NULL,"Function halted.","",MB_OK);
                    }
                }
                break;

                //About Dialog
                case ID_HELP_ABOUT:
                {
                    DialogBox(GetModuleHandle(NULL),
                        MAKEINTRESOURCE(DLG_ABOUT),hWnd,DlgAboutProcedure);
                }
                break;

                //FAQ Function
                case ID_HELP_FAQ:
                {
                    MessageBox(NULL,"\n\nQ: Can I restore safe mode? \n A: Just make a backup of HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Safeboot. \n\nQ: How does this program work? \n A: Through the registry and hooks.\n\nQ: What is telnet?\n A: Google is your friend.\n\n\n**Q: Why do some of the functions not work right away? \n The following require logoff or restart:\nRun\nDrive\"C\"\nChange Background\nControl Panel\nBalloon Tips\nFolder Options\nDesktop Items", "FAQ",MB_OK | MB_ICONINFORMATION);
                }
                break;


                /*
                 *Icon Buttons
                 */
                case ID_EXPLORER:
                {
                    ShellExecute(NULL,"open","Explorer.exe",NULL,NULL,SW_SHOWNORMAL);
                }
                break;

                case ID_CMD:
                {
                    ShellExecute(NULL,"open","cmd.exe",NULL,NULL,SW_SHOWNORMAL);
                }
                break;

                case ID_NOTEPAD:
                {
                    ShellExecute(NULL,"open","notepad.exe",NULL,NULL,SW_SHOWNORMAL);
                }
                break;

                case ID_OTHER_HELP:
                {
                    MessageBox(NULL,"Enter the query into the text box below the word \"Other\" and click a button to apply the query to that function.\n\nFor the CheckDisk and Defrag operations, just enter the drive letter like \"C\"","Other Help",MB_OK | MB_ICONQUESTION);
                }
                break;



                /*
                 *Control Events
                 */

                case ID_E_TASKMGR:
                    WriteKey(0);
                    EnableWindow(BUTTON_E_TASKMGR,false);
                    EnableWindow(BUTTON_D_TASKMGR,true);
                    break;

                case ID_D_TASKMGR:
                    WriteKey(1);
                    EnableWindow(BUTTON_D_TASKMGR,false);
                    EnableWindow(BUTTON_E_TASKMGR,true);
                    break;

                case ID_E_RUN:
                    WriteKey(2);
                    EnableWindow(BUTTON_E_RUN,false);
                    EnableWindow(BUTTON_D_RUN,true);
                    break;

                case ID_D_RUN:
                    WriteKey(3);
                    EnableWindow(BUTTON_D_RUN,false);
                    EnableWindow(BUTTON_E_RUN,true);
                    break;

                case ID_E_REGEDIT:
                    WriteKey(4);
                    EnableWindow(BUTTON_E_REGEDIT,false);
                    EnableWindow(BUTTON_D_REGEDIT,true);
                    break;

                case ID_D_REGEDIT:
                    WriteKey(5);
                    EnableWindow(BUTTON_D_REGEDIT,false);
                    EnableWindow(BUTTON_E_REGEDIT,true);
                    break;

                case ID_E_DISPLAY_PROPERTIES:
                    WriteKey(6);
                    EnableWindow(BUTTON_E_DISPLAY_PROPERTIES,false);
                    EnableWindow(BUTTON_D_DISPLAY_PROPERTIES,true);
                    break;

                case ID_D_DISPLAY_PROPERTIES:
                    WriteKey(7);
                    EnableWindow(BUTTON_D_DISPLAY_PROPERTIES,false);
                    EnableWindow(BUTTON_E_DISPLAY_PROPERTIES,true);
                    break;

                case ID_E_CMD:
                    WriteKey(8);
                    EnableWindow(BUTTON_E_CMD,false);
                    EnableWindow(BUTTON_D_CMD,true);
                    break;

                case ID_D_CMD:
                    WriteKey(9);
                    EnableWindow(BUTTON_D_CMD,false);
                    EnableWindow(BUTTON_E_CMD,true);
                    break;

                case ID_E_DRIVE_C:
                    WriteKey(10);
                    EnableWindow(BUTTON_E_DRIVE_C,false);
                    EnableWindow(BUTTON_D_DRIVE_C,true);
                    break;

                case ID_D_DRIVE_C:
                    WriteKey(11);
                    EnableWindow(BUTTON_D_DRIVE_C,false);
                    EnableWindow(BUTTON_E_DRIVE_C,true);
                    break;

                case ID_E_CHANGE_BACKGROUND:
                    WriteKey(12);
                    EnableWindow(BUTTON_E_CHANGE_BACKGROUND,false);
                    EnableWindow(BUTTON_D_CHANGE_BACKGROUND,true);
                    break;

                case ID_D_CHANGE_BACKGROUND:
                    WriteKey(13);
                    EnableWindow(BUTTON_D_CHANGE_BACKGROUND,false);
                    EnableWindow(BUTTON_E_CHANGE_BACKGROUND,true);
                    break;

                case ID_E_CONTROL_PANEL:
                    WriteKey(14);
                    EnableWindow(BUTTON_E_CONTROL_PANEL,false);
                    EnableWindow(BUTTON_D_CONTROL_PANEL,true);
                    break;

                case ID_D_CONTROL_PANEL:
                    WriteKey(15);
                    EnableWindow(BUTTON_D_CONTROL_PANEL,false);
                    EnableWindow(BUTTON_E_CONTROL_PANEL,true);
                    break;

                case ID_E_BALLOON_TIPS:
                    WriteKey(16);
                    EnableWindow(BUTTON_E_BALLOON_TIPS,false);
                    EnableWindow(BUTTON_D_BALLOON_TIPS,true);
                    break;

                case ID_D_BALLOON_TIPS:
                    WriteKey(17);
                    EnableWindow(BUTTON_D_BALLOON_TIPS,false);
                    EnableWindow(BUTTON_E_BALLOON_TIPS,true);
                    break;

                case ID_E_FOLDER_OPTIONS:
                    WriteKey(18);
                    EnableWindow(BUTTON_E_FOLDER_OPTIONS,false);
                    EnableWindow(BUTTON_D_FOLDER_OPTIONS,true);
                    break;

                case ID_D_FOLDER_OPTIONS:
                    WriteKey(19);
                    EnableWindow(BUTTON_D_FOLDER_OPTIONS,false);
                    EnableWindow(BUTTON_E_FOLDER_OPTIONS,true);
                    break;

                case ID_E_DESKTOP_ITEMS:
                    WriteKey(20);
                    EnableWindow(BUTTON_E_DESKTOP_ITEMS,false);
                    EnableWindow(BUTTON_D_DESKTOP_ITEMS,true);
                    break;

                case ID_D_DESKTOP_ITEMS:
                    WriteKey(21);
                    EnableWindow(BUTTON_D_DESKTOP_ITEMS,false);
                    EnableWindow(BUTTON_E_DESKTOP_ITEMS,true);
                    break;

                case ID_E_LAST_GOOD_CONFIG:
                    WriteKey(22);
                    EnableWindow(BUTTON_E_LAST_GOOD_CONFIG,false);
                    EnableWindow(BUTTON_D_LAST_GOOD_CONFIG,true);
                    break;

                case ID_D_LAST_GOOD_CONFIG:
                    WriteKey(23);
                    EnableWindow(BUTTON_D_LAST_GOOD_CONFIG,false);
                    EnableWindow(BUTTON_E_LAST_GOOD_CONFIG,true);
                    break;


                /*Other Buttons*/

                case ID_PING:

                    len = GetWindowTextLength(EDIT_TEXTBOX);
                    if(len > 0)
                    {
                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_Ping, 0, 0, 0);
                    }
                    else
                    {
                        ErrorMsg("Please enter a query into the above text box.");
                    }
                    break;

                case ID_TRACEROUTE:
                    len = GetWindowTextLength(EDIT_TEXTBOX);
                    if(len > 0)
                    {
                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_TraceRoute, 0, 0, 0);
                    }
                    else
                    {
                        ErrorMsg("Please enter a query into the above text box.");
                    }
                    break;

                case ID_SEARCH_FILE:
                    len = GetWindowTextLength(EDIT_TEXTBOX);
                    if(len > 0)
                    {
                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_SearchFile, 0, 0, 0);
                    }
                    else
                    {
                        ErrorMsg("Please enter a query into the above text box.");
                    }
                    break;

                case ID_CHECKDISK:
                    len = GetWindowTextLength(EDIT_TEXTBOX);
                    if(len > 0)
                    {
                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_CheckDisk, 0, 0, 0);
                    }
                    else
                    {
                        ErrorMsg("Please enter a query into the above text box.");
                    }
                    break;

                case ID_DEFRAG:
                    len = GetWindowTextLength(EDIT_TEXTBOX);
                    if(len > 0)
                    {
                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_Defrag, 0, 0, 0);
                    }
                    else
                    {
                        ErrorMsg("Please enter a query into the above text box.");
                    }
                    break;

                case ID_NS_LOOKUP:
                    len = GetWindowTextLength(EDIT_TEXTBOX);
                    if(len > 0)
                    {
                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_NSLookup, 0, 0, 0);
                    }
                    else
                    {
                        ErrorMsg("Please enter a query into the above text box.");
                    }
                    break;

                case ID_TELNET:
                    len = GetWindowTextLength(EDIT_TEXTBOX);
                    if(len > 0)
                    {
                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_Telnet, 0, 0, 0);
                    }
                    else
                    {
                        ErrorMsg("Please enter a query into the above text box.");
                    }
                    break;

                case ID_SET_BOOTSHELL:
                    len = GetWindowTextLength(COMBOBOX_BOOTSHELL);
                    if(len > 0)
                    {
                        buffer = (char*)GlobalAlloc(GPTR,len+1);
                        GetWindowText(COMBOBOX_BOOTSHELL,buffer,len+1);
                        SetBootShell(buffer);

                        GlobalFree((HANDLE)buf);
                    }
                    else
                    {
                        MessageBox(NULL,"Please Select a boot shell to the left.","Error",MB_OK);
                    }
                    break;

                case ID_SET_STARTMENU:
                        len = GetWindowTextLength(EDIT_SET_STARTBUTTON);

                        buffer = (char*)GlobalAlloc(GPTR,len+1);
                        GetWindowText(EDIT_SET_STARTBUTTON,buffer,len+1);
                        SetStartButtonText(buffer);

                        GlobalFree((HANDLE)buffer);

                    break;

                }


            break; //case WM_COMMAND

        /*Setting Text Colors*/
        case WM_CTLCOLORSTATIC:
            if((HWND)lParam == GetDlgItem(hWnd, ID_STATICTEXT_ENABLE)){
                SetTextColor((HDC)wParam, RGB(0,255,40));
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LONG)textCtrl;
                }
            if((HWND)lParam == GetDlgItem(hWnd, ID_STATICTEXT_DISABLE)){
                SetTextColor((HDC)wParam, RGB(255,0,40));
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LONG)textCtrl;
                }
            if((HWND)lParam == GetDlgItem(hWnd, ID_STATICTEXT_OTHER)){
                SetTextColor((HDC)wParam, RGB(40, 0, 255));
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LONG)textCtrl;
                }
            if((HWND)lParam == GetDlgItem(hWnd, ID_STATICTEXT_BOOTSHELL)){
                SetTextColor((HDC)wParam, RGB(40, 0, 215));
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LONG)textCtrl;
                }
            if((HWND)lParam == GetDlgItem(hWnd, ID_STATICTEXT_SCROLLHERE)){
                SetTextColor((HDC)wParam, RGB(255,255,255));
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LONG)textCtrl;
                }
            if((HWND)lParam == GetDlgItem(hWnd, ID_STATICTEXT_STARTMENU)){
                SetTextColor((HDC)wParam, RGB(0,0,160));
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LONG)textCtrl;
                }

                break;


        case WM_CREATE:

            textCtrl = CreateSolidBrush(RGB(0,0,0));

            CreateGUIControls(hWnd, reinterpret_cast<CREATESTRUCT*>(lParam));

            TestPriv();

            break;


        case WM_DESTROY: //Quit Application

            DeleteObject(textCtrl);
            PostQuitMessage(WM_QUIT);

            break;

        default:

            return DefWindowProc(hWnd, msg, wParam, lParam);


        }

        return 0;
}

///Dialogs

BOOL CALLBACK DlgAboutProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){

        case WM_INITDIALOG:
            g_hbrBackground = CreateSolidBrush(RGB(205, 205, 193));

            SendMessage(hWnd,WM_SETICON,ICON_SMALL,
            reinterpret_cast<LPARAM>(LoadImage(GetModuleHandle(0),
                                               MAKEINTRESOURCE(IDI_ICONSM),
                                               IMAGE_ICON,
                                               0,0,LR_DEFAULTCOLOR)));
        break;

        case WM_CTLCOLORDLG:
            return (LONG)g_hbrBackground;
        case WM_CTLCOLORSTATIC:
        {
              HDC hdcStatic = (HDC)wParam;
              SetTextColor(hdcStatic, RGB(0, 0, 0));
              SetBkMode(hdcStatic, TRANSPARENT);
              return (LONG)g_hbrBackground;
        }
        break;

        case WM_COMMAND:

            switch(LOWORD(wParam)){

                case IDOK:
                    EndDialog(hWnd, IDOK);
                    break;
                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    break;
                }
        break;
        case WM_DESTROY:
            DeleteObject(g_hbrBackground);
        default:
            return FALSE;
        }
        return TRUE;

    }

BOOL CALLBACK DlgRunProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

    switch(msg){
        case WM_INITDIALOG:
            g_hbrBackground = CreateSolidBrush(RGB(205, 205, 193));
            break;

        case WM_CTLCOLORDLG:
            return (LONG)g_hbrBackground;
        case WM_CTLCOLORSTATIC:
        {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(0,0,0));
            SetBkMode(hdc, TRANSPARENT);
            return (LONG)g_hbrBackground;
        }
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case IDOK:
                {
                    //Run Command
                    int len = GetWindowTextLength(GetDlgItem(hWnd, DLG_RUN_IDC_TEXT));
                    if(len > 0)
                    {
                    buffer = (char*)GlobalAlloc(GPTR,len+1);

                    GetDlgItemText(hWnd, DLG_RUN_IDC_TEXT, buffer, len +1);
                    ShellExecute(NULL,"open",buffer,NULL,NULL,SW_SHOWNORMAL);

                    GlobalFree((HANDLE)buffer);
                    EndDialog(hWnd, IDOK);
                    }
                    break;
                }

                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    break;
                }
        break;
        case WM_DESTROY:
            DeleteObject(g_hbrBackground);
            EndDialog(hWnd, 0);
            break;
        default:
            return FALSE;
        }
        return TRUE;

    }

BOOL CALLBACK DlgTerminateProcProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

    switch(msg){
        case WM_INITDIALOG:
            g_hbrBackground = CreateSolidBrush(RGB(205, 205, 193));
            break;

        case WM_CTLCOLORDLG:
            return (LONG)g_hbrBackground;
        case WM_CTLCOLORSTATIC:
        {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(0,0,0));
            SetBkMode(hdc, TRANSPARENT);
            return (LONG)g_hbrBackground;
        }
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam)){
                {
                case IDOK:
                    int len = GetWindowTextLength(GetDlgItem(hWnd,DLG_TERMINATE_PROC_IDC_TEXT));

                    if(len > 0)
                    {
                           buffer = (char*)GlobalAlloc(GPTR,len+1);

                           GetDlgItemText(hWnd, DLG_TERMINATE_PROC_IDC_TEXT, buffer, len +1);

                            STARTUPINFO siStartupInfo;
                            PROCESS_INFORMATION piProcessInfo;
                            char *arg;
                            char cmd[155];

                            GetSystemDirectory(cmd,sizeof(cmd));

                            memset(&siStartupInfo, 0, sizeof(siStartupInfo));
                            memset(&piProcessInfo, 0, sizeof(piProcessInfo));

                            siStartupInfo.cb = sizeof(siStartupInfo);

                            arg = static_cast<char*>(GlobalAlloc(GPTR,100));
                            strcat(arg, " /C tskill ");
                            strcat(arg, buffer);
                            strcat(cmd,"\\cmd.exe");

                            CreateProcess(cmd, // Cmd.exe
                            arg, // argument
                            NULL,
                            NULL,
                            FALSE,
                            CREATE_NO_WINDOW,
                            NULL,
                            NULL,
                            &siStartupInfo,
                            &piProcessInfo);

                            WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

                           GlobalFree((HANDLE)buffer);
                    }
                }
                    break;

                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    break;
                }
        break;
        case WM_DESTROY:
            DeleteObject(g_hbrBackground);
            EndDialog(hWnd, 0);
            break;
        default:
            return FALSE;
        }
        return TRUE;

    }

BOOL CALLBACK DlgInstallerProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

    switch(msg){
        case WM_INITDIALOG:
            g_hbrBackground = CreateSolidBrush(RGB(205, 205, 193));
            SendMessage(hWnd,WM_SETICON,ICON_SMALL,
            reinterpret_cast<LPARAM>(LoadImage(GetModuleHandle(0),
                                               MAKEINTRESOURCE(IDI_ICONSM),
                                               IMAGE_ICON,
                                               0,0,LR_DEFAULTCOLOR)));
            break;

        case WM_CTLCOLORDLG:
            return (LONG)g_hbrBackground;
        case WM_CTLCOLORSTATIC:
        {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(0,0,0));
            SetBkMode(hdc, TRANSPARENT);
            return (LONG)g_hbrBackground;
        }
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam)){

                case DLG_INSTALLER_INSTALL:
                {
                    Installer inst;

                    if(inst.Install() == 0)
                    {
                        SetDlgItemText(hWnd,DLG_INSTALLER_TEXT,"System Controls has been successfully installed.");
                    }
                    else
                    {
                        SetDlgItemText(hWnd,DLG_INSTALLER_TEXT,"Error installing some components.");
                    }
                }
                    break;

                case DLG_INSTALLER_UNINSTALL:
                {
                    Installer inst;

                    if(inst.Uninstall() == 0)
                    {
                        SetDlgItemText(hWnd,DLG_INSTALLER_TEXT,"System Controls has been successfully uninstalled.");
                    }
                    else
                    {
                        SetDlgItemText(hWnd,DLG_INSTALLER_TEXT,"Error uninstalling some components.");
                    }
                }
                    break;

                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    break;
                }
        break;
        case WM_DESTROY:
            DeleteObject(g_hbrBackground);
            EndDialog(hWnd, 0);
            break;
        default:
            return FALSE;
        }
        return TRUE;

    }

BOOL CALLBACK DlgSetAdminPassProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

    switch(msg){
        case WM_INITDIALOG:
            g_hbrBackground = CreateSolidBrush(RGB(205, 205, 193));
            SendMessage(hWnd,WM_SETICON,ICON_SMALL,
            reinterpret_cast<LPARAM>(LoadImage(GetModuleHandle(0),
                                               MAKEINTRESOURCE(IDI_ICONSM),
                                               IMAGE_ICON,
                                               0,0,LR_DEFAULTCOLOR)));
            break;

        case WM_CTLCOLORDLG:
            return (LONG)g_hbrBackground;
        case WM_CTLCOLORSTATIC:
        {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(0,0,0));
            SetBkMode(hdc, TRANSPARENT);
            return (LONG)g_hbrBackground;
        }
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case IDOK:
                {
                    int len = GetWindowTextLength(GetDlgItem(hWnd, DLG_SET_ADMIN_PASS_IDC_TEXT));

                    if(len > 0)
                    {
                        buffer = (char*)GlobalAlloc(GPTR,60);
                        GetDlgItemText(hWnd,DLG_SET_ADMIN_PASS_IDC_TEXT,buffer,len+1);
                        STARTUPINFO siStartupInfo;
                        PROCESS_INFORMATION piProcessInfo;
                        char *arg;
                        char cmd[MAX_PATH];

                        GetSystemDirectory(cmd,sizeof(cmd));

                        memset(&siStartupInfo, 0, sizeof(siStartupInfo));
                        memset(&piProcessInfo, 0, sizeof(piProcessInfo));

                        siStartupInfo.cb = sizeof(siStartupInfo);

                        arg = static_cast<char*>(GlobalAlloc(GPTR,100));

                        strcat(arg, " /C net user Administrator ");
                        strcat(arg, buffer);
                        strcat(arg, " && pause");
                        strcat(cmd,"\\cmd.exe");

                        CreateProcess(cmd, // Cmd.exe
                        arg, // argument
                        NULL,
                        NULL,
                        FALSE,
                        CREATE_NEW_CONSOLE,
                        NULL,
                        NULL,
                        &siStartupInfo,
                        &piProcessInfo);

                    }
                }
                    break;

                case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                    break;
                }
        break;
        case WM_DESTROY:
            DeleteObject(g_hbrBackground);
            EndDialog(hWnd, 0);
            break;
        default:
            return FALSE;
        }
        return TRUE;

    }


void CreateGUIControls(const HWND hWnd, CREATESTRUCT* cstruct){

            /*
             * All Controls
             */
             RECT rc = {0, 0, 45, 45};

             /*Create Icon Buttons*/
             BUTTON_EXPLORER            = CreateControl("button","",hWnd,cstruct->hInstance, BS_ICON, 0, rc, ID_EXPLORER);
             HICON explorerIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_EXPLORER));
             if(!explorerIcon)
             {
                explorerIcon = LoadIcon(GetModuleHandle(NULL),IDI_APPLICATION);
             }
             SendMessage(BUTTON_EXPLORER,BM_SETIMAGE,IMAGE_ICON,reinterpret_cast<LPARAM>(explorerIcon));

             rc.left = 313;

             BUTTON_CMD                 = CreateControl("button","",hWnd,cstruct->hInstance, BS_ICON, 0, rc, ID_CMD);
             HICON cmdIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_CMD));
             if(!cmdIcon)
             {
                cmdIcon = LoadIcon(GetModuleHandle(NULL),IDI_EXCLAMATION);
             }
             SendMessage(BUTTON_CMD,BM_SETIMAGE,IMAGE_ICON,reinterpret_cast<LPARAM>(cmdIcon));

             rc.left = 669;

             BUTTON_NOTEPAD             = CreateControl("button","",hWnd,cstruct->hInstance, BS_ICON, 0, rc, ID_NOTEPAD);
             HICON notepadIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_NOTEPAD));
             if(!notepadIcon)
             {
                notepadIcon = LoadIcon(GetModuleHandle(NULL),IDI_HAND);
             }
             SendMessage(BUTTON_NOTEPAD,BM_SETIMAGE,IMAGE_ICON,reinterpret_cast<LPARAM>(notepadIcon));

             rc.left = 899;

             BUTTON_OTHER_HELP          = CreateControl("button","",hWnd,cstruct->hInstance, BS_ICON, 0, rc, ID_OTHER_HELP);
             HICON otherHelpIcon = reinterpret_cast<HICON>(LoadImage(0,IDI_QUESTION,IMAGE_ICON,0,0,LR_SHARED));
             SendMessage(BUTTON_OTHER_HELP,BM_SETIMAGE,IMAGE_ICON,reinterpret_cast<LPARAM>(otherHelpIcon));

            SetRect(&rc, 132, 30, 200, 100);

            //Create "Enable" Static text
            CreateControl("static", "Enable", hWnd, cstruct->hInstance, SS_SIMPLE, 0, rc, ID_STATICTEXT_ENABLE);


            /*Start Creating Enable Buttons*/

            SetRect(&rc, 20, 80, 120, 30);

            BUTTON_E_TASKMGR            = CreateControl("button", "Task Manager", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_TASKMGR);

            rc.top += 60;

            BUTTON_E_RUN                = CreateControl("button", "Run", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_RUN);

            rc.top += 60;

            BUTTON_E_REGEDIT            = CreateControl("button", "Registry Editor", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_REGEDIT);

            rc.top += 60;

            BUTTON_E_DISPLAY_PROPERTIES = CreateControl("button", "Display Prop.", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_DISPLAY_PROPERTIES);

            rc.top += 60;

            BUTTON_E_CMD                = CreateControl("button", "CMD", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_CMD);

            rc.top += 60;

            BUTTON_E_DRIVE_C            = CreateControl("button", "Drive \"C\"", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_DRIVE_C);


            ////New Collumn **
            rc.left = 170;
            rc.top = 80;

            BUTTON_E_CHANGE_BACKGROUND  = CreateControl("button", "Change Backgr.", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_CHANGE_BACKGROUND);

            rc.top += 60;

            BUTTON_E_CONTROL_PANEL      = CreateControl("button", "Control Panel", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_CONTROL_PANEL);

            rc.top += 60;

            BUTTON_E_BALLOON_TIPS       = CreateControl("button", "Balloon Tips", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_BALLOON_TIPS);

            rc.top += 60;

            BUTTON_E_FOLDER_OPTIONS     = CreateControl("button", "Folder Options", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_FOLDER_OPTIONS);

            rc.top += 60;

            BUTTON_E_DESKTOP_ITEMS      = CreateControl("button", "Desktop Items", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_DESKTOP_ITEMS);

            rc.top += 60;

            BUTTON_E_LAST_GOOD_CONFIG   = CreateControl("button", "LastGoodConfig", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_E_LAST_GOOD_CONFIG);




            //Create Static Line Between Enable and Disable Sections

            SetRect(&rc, 326, 45, 20, 430);

            CreateControl("static", "", hWnd, cstruct->hInstance, SS_GRAYRECT | WS_CHILD | WS_VISIBLE, 0, rc, ID_STATICRECT1);




            //Create "Disable" Static Text

            SetRect(&rc, 491, 30, 200, 100);

            CreateControl("static", "Disable", hWnd, cstruct->hInstance, SS_SIMPLE, 0, rc, ID_STATICTEXT_DISABLE);


            //Create Disable Buttons

            SetRect(&rc, 380, 80, 120, 30);

            BUTTON_D_TASKMGR            = CreateControl("button", "Task Manager", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_TASKMGR);

            rc.top += 60;

            BUTTON_D_RUN                = CreateControl("button", "Run", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_RUN);

            rc.top += 60;

            BUTTON_D_REGEDIT            = CreateControl("button", "Registry Editor", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_REGEDIT);

            rc.top += 60;

            BUTTON_D_DISPLAY_PROPERTIES = CreateControl("button", "Display Prop.", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_DISPLAY_PROPERTIES);

            rc.top += 60;

            BUTTON_D_CMD                = CreateControl("button", "CMD", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_CMD);

            rc.top += 60;

            BUTTON_D_DRIVE_C            = CreateControl("button", "Drive \"C\"", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_DRIVE_C);

            rc.top += 60;


            ////New Collumn **

            rc.left = 530;
            rc.top = 80;

            BUTTON_D_CHANGE_BACKGROUND  = CreateControl("button", "Change Backgr.", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_CHANGE_BACKGROUND);

            rc.top += 60;

            BUTTON_D_CONTROL_PANEL      = CreateControl("button", "Control Panel", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_CONTROL_PANEL);

            rc.top += 60;

            BUTTON_D_BALLOON_TIPS       = CreateControl("button", "Balloon Tips", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_BALLOON_TIPS);

            rc.top += 60;

            BUTTON_D_FOLDER_OPTIONS     = CreateControl("button", "Folder Options", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_FOLDER_OPTIONS);

            rc.top += 60;

            BUTTON_D_DESKTOP_ITEMS      = CreateControl("button", "Desktop Items", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_DESKTOP_ITEMS);

            rc.top += 60;

            BUTTON_D_LAST_GOOD_CONFIG   = CreateControl("button", "LastGoodConfig", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_D_LAST_GOOD_CONFIG);



            /*Create line between disable section and other section*/

            SetRect(&rc, 682, 45, 20, 430);

            CreateControl("static", "", hWnd, cstruct->hInstance, SS_GRAYRECT, 0, rc, ID_STATICRECT2);


            /*Create "Other" Static Text*/

            SetRect(&rc, 805, 30, 200, 100);

            CreateControl("static", "Other", hWnd, cstruct->hInstance, SS_SIMPLE, 0, rc, ID_STATICTEXT_OTHER);


            /*Create edit boxes for other buttons*/

            SetRect(&rc, 762, 80, 125, 17);

            EDIT_TEXTBOX            = CreateControl("edit", "", hWnd, cstruct->hInstance, ES_AUTOHSCROLL, 0, rc, ID_TEXTBOX);

            SetRect(&rc, 781, 117, 90, 30);

            BUTTON_PING             = CreateControl("button", "Ping", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_PING);

            rc.top += 50;

            BUTTON_TRACEROUTE       = CreateControl("button", "TraceRoute", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_TRACEROUTE);

            rc.top += 50;

            BUTTON_SEARCH_FILE      = CreateControl("button", "Search File", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_SEARCH_FILE);

            rc.top += 50;

            BUTTON_CHECKDISK        = CreateControl("button", "CheckDisk", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_CHECKDISK);

            rc.top += 50;

            BUTTON_DEFRAG           = CreateControl("button", "Defrag", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_DEFRAG);

            rc.top += 50;

            BUTTON_NS_LOOKUP        = CreateControl("button", "NS Lookup", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_NS_LOOKUP);

            rc.top += 50;

            BUTTON_TELNET           = CreateControl("button", "Telnet", hWnd, cstruct->hInstance, BS_PUSHBUTTON, WS_EX_CLIENTEDGE, rc, ID_TELNET);

            SetRect(&rc, 130, 501, 100, 17);

            CreateControl("static","Set Boot Shell:", hWnd, cstruct->hInstance, SS_SIMPLE, 0, rc, ID_STATICTEXT_BOOTSHELL);

            SetRect(&rc, 235, 497, 150, 430);

            COMBOBOX_BOOTSHELL      = CreateControl("combobox","", hWnd, cstruct->hInstance, CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL, WS_EX_CLIENTEDGE, rc, ID_CB_BOOTSHELL);
            AddString(COMBOBOX_BOOTSHELL,_T("Explorer"));
            AddString(COMBOBOX_BOOTSHELL,_T("Spider Solitare"));
            AddString(COMBOBOX_BOOTSHELL,_T("Solitare"));
            AddString(COMBOBOX_BOOTSHELL,_T("Minesweeper"));
            AddString(COMBOBOX_BOOTSHELL,_T("CMD"));
            AddString(COMBOBOX_BOOTSHELL,_T("Calculator"));
            AddString(COMBOBOX_BOOTSHELL,_T("Notepad"));
            AddString(COMBOBOX_BOOTSHELL,_T("Screen Keyboard"));

            SetRect(&rc, 397, 497, 42, 25);

            BUTTON_SET_BOOTSHELL    = CreateControl("button", "Set", hWnd, cstruct->hInstance, BS_PUSHBUTTON | BS_FLAT | WS_DLGFRAME, 0, rc, ID_SET_BOOTSHELL);

            SetRect(&rc, 262, 525, 120, 17);

            CreateControl("static","^ Choose Here ^", hWnd, cstruct->hInstance, SS_SIMPLE, 0, rc, ID_STATICTEXT_SCROLLHERE);

            SetRect(&rc, 540, 501, 140, 17);

            CreateControl("static","Set Start Button Text:", hWnd, cstruct->hInstance, SS_SIMPLE, 0, rc, ID_STATICTEXT_STARTMENU);

            rc.left += 145;
            rc.right = 42;

            EDIT_SET_STARTBUTTON      = CreateControl("edit", "", hWnd, cstruct->hInstance, ES_AUTOHSCROLL | WS_OVERLAPPED, 0, rc, ID_EDIT_SET_STARTMENU);

            SetRect(&rc, 733, 497, 42, 25);

            BUTTON_SET_STARTBUTTON    = CreateControl("button", "Set", hWnd, cstruct->hInstance, BS_PUSHBUTTON | BS_FLAT | WS_DLGFRAME, 0, rc, ID_SET_STARTMENU);
    }
