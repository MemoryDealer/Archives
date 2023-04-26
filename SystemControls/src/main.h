#include "resource.h"
#define _WIN32_IE 0x0500 //For SHGetFolderPath
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <commdlg.h>
#include <shellapi.h> //For ShellExecute
#include <shlobj.h> //For SHGetFolderPath
#include <string>
#include <fstream>

#define MP MAX_PATH

using namespace std;

typedef basic_string<TCHAR> ustring;

extern HWND EDIT_TEXTBOX;

HKEY key;
string file;
char *buf[MAX_PATH];

HWND CreateControl(const char szType[MAX_PATH], const char szCaption[MAX_PATH],
                     const HWND hParent, const HINSTANCE hInstance,
                     DWORD dwStyle, DWORD dwExStyle, const RECT& cs, const int id){

    dwStyle |= WS_VISIBLE | WS_CHILD;

    return CreateWindowEx(dwExStyle,
                          szType,
                          szCaption,
                          dwStyle,
                          cs.left,
                          cs.top,
                          cs.right,
                          cs.bottom,
                          hParent,
                          reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
                          hInstance,
                          0);
}

inline UINT AddString(const HWND hCombo, const ustring &s){
    return static_cast<UINT>(SendMessage(hCombo,CB_ADDSTRING,0,
                             reinterpret_cast<LPARAM>(s.c_str())));
    }

inline int ErrorMsg(const char *msg)
{
     return MessageBox(NULL,msg,NULL,MB_OK);
}

void TestPriv(){

    bool good;
    bool cmd;

    RegCreateKeyEx(HKEY_CURRENT_USER,
    "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",0,0,
    REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,NULL);
    RegCloseKey(key);
    RegCreateKeyEx(HKEY_CURRENT_USER,
    "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",0,0,
    REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,NULL);
    RegCloseKey(key);
    RegCreateKeyEx(HKEY_CURRENT_USER,
    "Software\\Policies",0,0,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,
    &key,NULL);
    RegCloseKey(key);
    RegCreateKeyEx(HKEY_CURRENT_USER,
    "Software\\Policies\\Microsoft",0,0,REG_OPTION_NON_VOLATILE,
    KEY_WRITE,NULL,&key,NULL);
    RegCloseKey(key);
    RegCreateKeyEx(HKEY_CURRENT_USER,
    "Software\\Policies\\Microsoft\\Windows",0,0,REG_OPTION_NON_VOLATILE,
    KEY_WRITE,NULL,&key,NULL);
    RegCloseKey(key);
    RegCreateKeyEx(HKEY_CURRENT_USER,
    "Software\\Policies\\Microsoft\\Windows\\System",0,0,REG_OPTION_NON_VOLATILE,
    KEY_WRITE,NULL,&key,NULL);
    RegCloseKey(key);

    RegOpenKeyEx(HKEY_CURRENT_USER,
    "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
    0,KEY_WRITE,&key);
    if(RegSetValueEx(key,"TEST",0,REG_SZ,(const unsigned char*)"test",25)
    !=
    ERROR_SUCCESS){
        good = false;
        }
    else{
        good = true;
        }
    RegDeleteValue(key,"TEST");
    RegCloseKey(key);
    RegOpenKeyEx(HKEY_CURRENT_USER,
    "Software\\Policies\\Microsoft\\Windows\\System",0,KEY_WRITE,&key);
    if(RegSetValueEx(key,"TEST",0,REG_SZ,(const unsigned char*)"TEST",25)
    !=
    ERROR_SUCCESS){
        cmd = false;
        }
    else{
        cmd = true;
        }
    RegDeleteValue(key,"TEST");
    RegCloseKey(key);

    if(good == false){
        MessageBox(NULL,"Some parts of the application may not function properly due to permission restrictions on the current account.\nTry logging into an Administrator account or elevating to current permissions.","NOTICE",MB_OK | MB_ICONEXCLAMATION);
        }
    if(cmd == false){
        MessageBox(NULL,"The \"CMD\" buttons may not function properly due to permission restrictions on the current account.\nTry logging into an Administrator account or elevating to current permissions.","NOTICE",MB_OK | MB_ICONEXCLAMATION);
        }

    }

void SetBootShell(const char buffer[]){
    file = buffer;
    int i = 0;

    if(file == "Explorer")
    {
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
        if(RegSetValueEx(key,"Shell",0,REG_SZ,(const unsigned char*)"Explorer.exe",25) != ERROR_SUCCESS){i = 1;}
        RegCloseKey(key);
    }
    if(file == "Spider Solitare")
    {
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
        if(RegSetValueEx(key,"Shell",0,REG_SZ,(const unsigned char*)"spider.exe",25) != ERROR_SUCCESS){i = 1;}
        RegCloseKey(key);
    }
    if(file == "Solitare")
    {
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
        if(RegSetValueEx(key,"Shell",0,REG_SZ,(const unsigned char*)"sol.exe",25) != ERROR_SUCCESS){i = 1;}
        RegCloseKey(key);
    }
    if(file == "Minesweeper")
    {
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
        if(RegSetValueEx(key,"Shell",0,REG_SZ,(const unsigned char*)"winmine.exe",25) != ERROR_SUCCESS){i = 1;}
        RegCloseKey(key);
    }
    if(file == "CMD")
    {
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
        if(RegSetValueEx(key,"Shell",0,REG_SZ,(const unsigned char*)"cmd.exe",25) != ERROR_SUCCESS){i = 1;}
        RegCloseKey(key);
    }
    if(file == "Calculator")
    {
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
        if(RegSetValueEx(key,"Shell",0,REG_SZ,(const unsigned char*)"calc.exe",25) != ERROR_SUCCESS){i = 1;}
        RegCloseKey(key);
    }
    if(file == "Notepad")
    {
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
        if(RegSetValueEx(key,"Shell",0,REG_SZ,(const unsigned char*)"notepad.exe",25) != ERROR_SUCCESS){i = 1;}
        RegCloseKey(key);
    }
    if(file == "Screen Keyboard")
    {
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
        if(RegSetValueEx(key,"Shell",0,REG_SZ,(const unsigned char*)"osk.exe",25) != ERROR_SUCCESS){i = 1;}
        RegCloseKey(key);
    }
    else if(strlen(buffer) <= 0)
    {
        MessageBox(NULL,"Please select a boot shell.","Error",MB_OK);
        return;
    }

    if(i == 0)
        MessageBox(NULL,"Boot Shell set.","Boot Shell",MB_OK);
    else
        MessageBox(NULL,"Error changing boot shell.",NULL,MB_OK);
    }

void SetStartButtonText(const char szText[MAX_PATH]){
    HWND hWnd;
    hWnd = GetDlgItem(FindWindow("Shell_TrayWnd",NULL), 0x130);
    SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szText);
    }

void WriteKey(const int x){
    DWORD zero = 0, one = 1, three = 3, four = 4;

    switch(x){

        case 0: //Enable Task Manager
            RegOpenKeyEx(HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
            0,KEY_WRITE,&key);
            RegSetValueEx(key,"DisableTaskMgr",0,REG_DWORD,(BYTE*)&zero,sizeof(zero));
            RegCloseKey(key);
            break;

        case 1: //Disable Task Manager
            RegOpenKeyEx(HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
            0,KEY_WRITE,&key);
            RegSetValueEx(key,"DisableTaskMgr",0,REG_DWORD,(BYTE*)&one,sizeof(one));
            break;

        case 2: //Enable Run
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
            0,KEY_WRITE,&key);
            RegSetValueEx(key,"NoRun",0,REG_DWORD,(BYTE*)&zero,sizeof(zero));
            RegCloseKey(key);
            break;

        case 3: //Disable Run
            RegOpenKeyEx(HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
            0,KEY_WRITE,&key);
            RegSetValueEx(key,"NoRun",0,REG_DWORD,(BYTE*)&one,sizeof(one));
            RegCloseKey(key);
            break;

        case 4: //Enable Regedit
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
            0,KEY_WRITE,&key);
	        RegSetValueEx(key,"DisableRegistryTools",0,REG_DWORD,(BYTE *)&zero,sizeof(zero));
	        RegCloseKey(key);
	        break;

	    case 5: //Disable Regedit
	        RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
            0,KEY_WRITE,&key);
	        RegSetValueEx(key,"DisableRegistryTools",0,REG_DWORD,(BYTE *)&one,sizeof(one));
	        RegCloseKey(key);
	        break;

        case 6: //Enable Display Prop.
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
            0,KEY_WRITE,&key);
	        RegSetValueEx(key,"NoDispCPL",0,REG_DWORD,(BYTE *)&zero,sizeof(zero));
	        RegCloseKey(key);
            break;

        case 7: //Disable Display Prop.
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
            0,KEY_WRITE,&key);
	        RegSetValueEx(key,"NoDispCPL",0,REG_DWORD,(BYTE *)&one,sizeof(one));
	        RegCloseKey(key);
            break;

        case 8: //Enable CMD
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Policies\\Microsoft\\Windows\\System",0,KEY_WRITE,&key);
	        RegSetValueEx(key,"DisableCMD",0,REG_DWORD,(BYTE *)&zero,sizeof(zero));
	        RegCloseKey(key);
            break;

        case 9: //Disable CMD
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Policies\\Microsoft\\Windows\\System",0,KEY_WRITE,&key);
	        RegSetValueEx(key,"DisableCMD",0,REG_DWORD,(BYTE *)&one,sizeof(one));
	        RegCloseKey(key);
            break;

        case 10: //Enable Drive "C"
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
            0,KEY_WRITE,&key);
	        RegDeleteValue(key,"NoViewOnDrive");
	        RegCloseKey(key);
	        break;

	    case 11: //Disable Drive "C"
	        RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",0,KEY_WRITE,&key);
	        RegSetValueEx(key,"NoViewOnDrive",0,REG_DWORD,(BYTE *)&four,sizeof(four));
	        RegCloseKey(key);
	        break;

	    case 12: //Enable ChangeBackground
	       RegOpenKeyEx(HKEY_CURRENT_USER,
           "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop",
           0,KEY_WRITE,&key);
           RegSetValueEx(key,"NoChangingWallpaper",0,REG_DWORD,(BYTE*)&zero,
           sizeof(zero));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.bmp",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&three,sizeof(three));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.jpeg",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&three,sizeof(three));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.jpg",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&three,sizeof(three));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.gif",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&three,sizeof(three));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.png",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&three,sizeof(three));
           RegCloseKey(key);
	       break;

	    case 13: //Disable ChangeBackground
	       RegOpenKeyEx(HKEY_CURRENT_USER,
	       "Software\\Microsoft\\Windows\\CurrentVersion\\Policies",
           0,KEY_WRITE,&key);
	       RegCreateKeyEx(key,"ActiveDesktop",0,0,REG_OPTION_NON_VOLATILE,
           KEY_WRITE,NULL,&key,NULL);
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CURRENT_USER,
           "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop",
           0,KEY_WRITE,&key);
           RegSetValueEx(key,"NoChangingWallpaper",0,REG_DWORD,(BYTE*)&one,
           sizeof(one));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.bmp",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&one,sizeof(one));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.jpeg",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&one,sizeof(one));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.jpg",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&one,sizeof(one));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.gif",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&one,sizeof(one));
           RegCloseKey(key);

           RegOpenKeyEx(HKEY_CLASSES_ROOT,
           "SystemFileAssociations\\.png",0,KEY_WRITE,&key);
           RegSetValueEx(key,"ImageOptionFlags",0,REG_DWORD,(BYTE*)&one,sizeof(one));
           RegCloseKey(key);
           break;

        case 14: //Enable Control Panel
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
            0,KEY_WRITE,&key);
	        RegSetValueEx(key,"NoControlPanel",0,REG_DWORD,(BYTE *)&zero,sizeof(zero));
	        RegCloseKey(key);
	        break;

	    case 15: //Disable Control Panel
	       RegOpenKeyEx(HKEY_CURRENT_USER,
	       "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
           0,KEY_WRITE,&key);
	       RegSetValueEx(key,"NoControlPanel",0,REG_DWORD,(BYTE *)&one,sizeof(one));
	       RegCloseKey(key);
	       break;

	    case 16: //Enable Balloon Tips
	       RegOpenKeyEx(HKEY_CURRENT_USER,
	       "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",
           0,KEY_WRITE,&key);
	       RegSetValueEx(key,"EnableBalloonTips",0,REG_DWORD,(BYTE *)&one,sizeof(one));
	       RegCloseKey(key);
	       break;

	    case 17: //Disable Balloon Tips
	       RegOpenKeyEx(HKEY_CURRENT_USER,
	       "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",0,KEY_WRITE,&key);
	       RegSetValueEx(key,"EnableBalloonTips",0,REG_DWORD,(BYTE *)&zero,sizeof(zero));
	       RegCloseKey(key);
	       break;

	    case 18: //Enable Folder Options
	       RegOpenKeyEx(HKEY_CURRENT_USER,
	       "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
           0,KEY_WRITE,&key);
           RegSetValueEx(key,"NoFolderOptions",0,REG_DWORD,(BYTE *)&zero,sizeof(zero));
           RegCloseKey(key);
           break;

        case 19: //Disable Folder Options
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
            0,KEY_WRITE,&key);
            RegSetValueEx(key,"NoFolderOptions",0,REG_DWORD,(BYTE *)&one,sizeof(one));
            RegCloseKey(key);
            break;

        case 20: //Enable Desktop Items
            RegOpenKeyEx(HKEY_CURRENT_USER,
	        "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
            0,KEY_WRITE,&key);
	        RegSetValueEx(key,"NoDesktop",0,REG_DWORD,(BYTE *)&zero,sizeof(zero));
	        RegCloseKey(key);
	        break;

	    case 21: //Disable Desktop Items
	       RegOpenKeyEx(HKEY_CURRENT_USER,
	       "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
           0,KEY_WRITE,&key);
	       RegSetValueEx(key,"NoDesktop",0,REG_DWORD,(BYTE *)&one,sizeof(one));
           RegCloseKey(key);
           break;

        case 22: //Enable Last Good Config
            RegOpenKeyEx(HKEY_LOCAL_MACHINE,
	        "Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
	        RegSetValueEx(key,"ReportBootOk",0,REG_SZ,(const unsigned char*)&one,sizeof(one));
	        RegCloseKey(key);
	        break;

	    case 23: //Disable Last Good Config
	       RegOpenKeyEx(HKEY_LOCAL_MACHINE,
	       "Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&key);
	       RegSetValueEx(key,"ReportBootOk",0,REG_SZ,(const unsigned char*)&zero,sizeof(zero));
	       RegCloseKey(key);
	       break;



        default:
            MessageBox(NULL,"Write Error","Error",MB_OK|MB_ICONEXCLAMATION);
            break;


        }

}

DWORD WINAPI _Ping(LPVOID args)
{
    STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    int len;
    char *arg, *buf;
    char cmd[MAX_PATH];

    GetSystemDirectory(cmd,sizeof(cmd));

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

    len = GetWindowTextLength(EDIT_TEXTBOX);
    arg = static_cast<char*>(GlobalAlloc(GPTR,100));
    buf = static_cast<char*>(GlobalAlloc(GPTR,len+1));
    strcat(arg, " /C ping ");
    GetWindowText(EDIT_TEXTBOX, buf, len+1);
    strcat(arg, buf);
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

    WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

    return 0;
}

DWORD WINAPI _TraceRoute(LPVOID args){

    STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    int len;
    char *arg, *buf;
    char cmd[MAX_PATH];

    GetSystemDirectory(cmd,sizeof(cmd));

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

    len = GetWindowTextLength(EDIT_TEXTBOX);
    arg = static_cast<char*>(GlobalAlloc(GPTR,100));
    buf = static_cast<char*>(GlobalAlloc(GPTR,len+1));
    strcat(arg, " /C tracert ");
    GetWindowText(EDIT_TEXTBOX, buf, len+1);
    strcat(arg, buf);
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

    WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

    return 0;
}

DWORD WINAPI _SearchFile(LPVOID args){

    STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    int len;
    char *arg, *buf;
    char cmd[MAX_PATH];

    GetSystemDirectory(cmd,sizeof(cmd));

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

    len = GetWindowTextLength(EDIT_TEXTBOX);
    arg = static_cast<char*>(GlobalAlloc(GPTR,100));
    buf = static_cast<char*>(GlobalAlloc(GPTR,len+1));
    strcat(arg, " /C cd\\ && dir /s ");
    GetWindowText(EDIT_TEXTBOX, buf, len+1);
    strcat(arg, buf);
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

    WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

    return 0;
}

DWORD WINAPI _CheckDisk(LPVOID args){

    STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    int len;
    char *arg, *buf;
    char cmd[MAX_PATH];

    GetSystemDirectory(cmd,sizeof(cmd));

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

    len = GetWindowTextLength(EDIT_TEXTBOX);
    arg = static_cast<char*>(GlobalAlloc(GPTR,100));
    buf = static_cast<char*>(GlobalAlloc(GPTR,len+1));
    strcat(arg, " /C chkdsk ");
    GetWindowText(EDIT_TEXTBOX, buf, len+1);
    strcat(arg, buf);
    strcat(arg, ": && pause");
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

    WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

    return 0;
}

DWORD WINAPI _Defrag(LPVOID args){

    STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    int len;
    char *arg, *buf;
    char cmd[MAX_PATH];

    GetSystemDirectory(cmd,sizeof(cmd));

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

    len = GetWindowTextLength(EDIT_TEXTBOX);
    arg = static_cast<char*>(GlobalAlloc(GPTR,100));
    buf = static_cast<char*>(GlobalAlloc(GPTR,len+1));
    strcat(arg, " /C defrag ");
    GetWindowText(EDIT_TEXTBOX, buf, len+1);
    strcat(arg, buf);
    strcat(arg, ": -f && pause");
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

    WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

    return 0;
}

DWORD WINAPI _NSLookup(LPVOID args){

   STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    int len;
    char *arg, *buf;
    char cmd[MAX_PATH];

    GetSystemDirectory(cmd,sizeof(cmd));

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

    len = GetWindowTextLength(EDIT_TEXTBOX);
    arg = static_cast<char*>(GlobalAlloc(GPTR,100));
    buf = static_cast<char*>(GlobalAlloc(GPTR,len+1));
    strcat(arg, " /C nslookup ");
    GetWindowText(EDIT_TEXTBOX, buf, len+1);
    strcat(arg, buf);
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

    WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

    return 0;
}

DWORD WINAPI _Telnet(LPVOID args){

    STARTUPINFO siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    int len;
    char *arg, *buf;
    char cmd[MAX_PATH];

    GetSystemDirectory(cmd,sizeof(cmd));

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

    len = GetWindowTextLength(EDIT_TEXTBOX);
    arg = static_cast<char*>(GlobalAlloc(GPTR,100));
    buf = static_cast<char*>(GlobalAlloc(GPTR,len+1));
    strcat(arg, " /C telnet ");
    GetWindowText(EDIT_TEXTBOX, buf, len+1);
    strcat(arg, buf);
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

    WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

    return 0;
}

class Installer{
    public:
        bool Desktop;
        bool StartMenu;
        bool All_Users;

        int Install()
        {
        int x = 0;
        int *msg = new int;

        *msg = MessageBox(NULL,"Install for all users?","Installer",MB_YESNO | MB_ICONQUESTION);

        if(*msg == IDYES)
        {
            All_Users = true;
        }
        else if(*msg == IDNO)
        {
            All_Users = false;
        }

        *msg = MessageBox(NULL,"Do you want to create a start menu shortcut?","Installer",MB_YESNO | MB_ICONQUESTION);

        if(*msg == IDYES)
        {
            StartMenu = true;
        }
        else if(*msg == IDNO)
        {
            StartMenu = false;
        }

        *msg = MessageBox(NULL,"Create a desktop shortcut?","Installer",MB_YESNO | MB_ICONQUESTION);

        if(*msg == IDYES)
        {
            Desktop = true;
        }
        else if(*msg == IDNO)
        {
            Desktop = false;
        }

        delete msg;

        buf[MAX_PATH] = (char*)GlobalAlloc(GPTR,MAX_PATH);

        ////Install to Program Files

        SHGetFolderPath(NULL,CSIDL_PROGRAM_FILES,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\System Controls");

        CreateDirectory(buf[MAX_PATH],NULL);
        strcat(buf[MAX_PATH],"\\SystemControls.exe");

        char *szThis[MAX_PATH];
        *szThis = new char[MAX_PATH];     ////

        GetModuleFileName(NULL,*szThis,sizeof(szThis));
        CopyFile(*szThis,buf[MAX_PATH],false);

        GlobalFree((HANDLE)buf);

        buf[MAX_PATH] = (char*)GlobalAlloc(GPTR,MAX_PATH);


        if(All_Users == true)
        {
            if(Desktop == true)
            {
                //Install to desktop (All Users)
                SHGetFolderPath(NULL,CSIDL_COMMON_DESKTOPDIRECTORY,
                NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
                strcat(buf[MAX_PATH],"\\SysControls.exe");

                if(CopyFile(*szThis,buf[MAX_PATH],false) == 0)
                {
                    x++;
                }

            }
            if(StartMenu == true)
            {
                //Install to start menu(All Users)
                SHGetFolderPath(NULL,CSIDL_COMMON_PROGRAMS,
                NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
                strcat(buf[MAX_PATH],"\\System Controls");

                CreateDirectory(buf[MAX_PATH],NULL);
                strcat(buf[MAX_PATH],"\\SysControls.exe");

                if(CopyFile(*szThis,buf[MAX_PATH],false) == 0)
                {
                    x++;
                }
            }
        }

        if(Desktop == true && All_Users != true)
        {
            //Install to desktop(Current User)
            SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,
            NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
            strcat(buf[MAX_PATH],"\\SysControls.exe");

            if(CopyFile(*szThis,buf[MAX_PATH],false) == 0)
            {
                x++;
            }

        }

        if(StartMenu == true && All_Users != true)
        {
            //Install to start menu(Current User)
            SHGetFolderPath(NULL,CSIDL_PROGRAMS,
            NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
            strcat(buf[MAX_PATH],"\\System Controls");

            if(CreateDirectory(buf[MAX_PATH],NULL) == 0)
            {
                x++;
            }
            strcat(buf[MAX_PATH],"\\SysControls.exe");

            if(CopyFile(*szThis,buf[MAX_PATH],false) == 0)
            {
                x++;
            }

        }

        GlobalFree((HANDLE)buf);

        delete szThis;  // Warning: deleting array 'char*szThis[260]'
        // Tricky experiment with pointer array, never doing that again...  (@_@)

        return x;

    }

    int Uninstall()
    {
        int x = 0;

        buf[MAX_PATH] = (char*)GlobalAlloc(GPTR,MAX_PATH);

        //Uninstall from Program Files
        SHGetFolderPath(NULL,CSIDL_PROGRAM_FILES,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\System Controls\\SystemControls.exe");

        if(DeleteFile(buf[MAX_PATH]) == 0)
        {
            x++;
        }

        SHGetFolderPath(NULL,CSIDL_PROGRAM_FILES,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\System Controls");

        if(RemoveDirectory(buf[MAX_PATH]) == 0)
        {
            x++;
        }


        //Remove from Desktop
        SHGetFolderPath(NULL,CSIDL_COMMON_DESKTOPDIRECTORY,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\SysControls.exe");

        DeleteFile(buf[MAX_PATH]);

        SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\SysControls.exe");

        DeleteFile(buf[MAX_PATH]);

        //Remove from Start Menu
        SHGetFolderPath(NULL,CSIDL_COMMON_PROGRAMS,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\System Controls\\SysControls.exe");

        DeleteFile(buf[MAX_PATH]);

        SHGetFolderPath(NULL,CSIDL_COMMON_PROGRAMS,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\System Controls");

        RemoveDirectory(buf[MAX_PATH]);

        SHGetFolderPath(NULL,CSIDL_PROGRAMS,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\System Controls\\SysControls.exe");

        DeleteFile(buf[MAX_PATH]);

        SHGetFolderPath(NULL,CSIDL_PROGRAMS,NULL,SHGFP_TYPE_CURRENT,buf[MAX_PATH]);
        strcat(buf[MAX_PATH],"\\System Controls");

        DeleteFile(buf[MAX_PATH]);

        RemoveDirectory(buf[MAX_PATH]);


        GlobalFree((HANDLE)buf);

        return x;
    }
};

void RenewIP()
{
    system("ipconfig /release");
    system("ipconfig /flushdns");
    system("ipconfig /renew");
}

void RemoveSafeMode()
{
    HKEY fKey;

    RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{36FC9E60-C465-11CF-8056-444553540000}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E965-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E967-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E969-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E96A-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E96B-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E96F-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E977-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E97B-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E97D-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E980-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{533C5B84-EC70-11D2-9505-00C04F79DEAF}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{71A27CDD-812A-11D0-BEC7-08002BE2092F}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{745A17A0-74D3-11D0-B6FE-00A0C90F57DA}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"AppMgmt");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Base");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Boot Bus Extender");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Boot file system");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"CryptSvc");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"DcomLaunch");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmadmin");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmboot.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmio.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmload.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmserver");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"EventLog");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"File system");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Filter");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"HelpSvc");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Netlogon");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"PCI Configuration");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"PlugPlay");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"PNP Filter");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Primary disk");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"RpcSs");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"SCSI Class");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"sermouse.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"sr.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"SRService");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"System Bus Extender");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vds");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vga.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vgasave.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"WinMgmt");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vds");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vds");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vds");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vds");
        RegCloseKey(fKey);



        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Minimal");
        RegCloseKey(fKey);



        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{36FC9E60-C465-11CF-8056-444553540000}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E965-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E967-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E969-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E96A-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E96B-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E96F-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E972-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E973-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E974-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E975-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E977-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E97B-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E97D-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{4D36E980-E325-11CE-BFC1-08002BE10318}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{71A27CDD-812A-11D0-BEC7-08002BE2092F}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"{745A17A0-74D3-11D0-B6FE-00A0C90F57DA}");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"AFD");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"AppMgmt");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Base");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Boot Bus Extender");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Boot file system");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Browser");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"CryptSvc");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"DcomLaunch");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Dhcp");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmadmin");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmboot.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmio.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmload.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"dmserver");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"DnsCache");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"EventLog");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"File system");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Filter");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"HelpSvc");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"ip6fw.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"ipnat.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"LanmanServer");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"LanmanWorkstation");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"LmHosts");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Messenger");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NDIS");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NDIS Wrapper");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Ndisuio");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NetBIOS");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NetBIOSGroup");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NetBT");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NetDDEGroup");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Netlogon");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NetMan");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Network");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NetworkProvider");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"NtLmSsp");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"PCI Configuration");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"PlugPlay");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"PNP Filter");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"PNP_TDI");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Primary disk");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"rdpcdd.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"rdpdd.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"rdpwd.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"rdsessmgr");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"RpcSs");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"SCSI Class");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"sermouse.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"SharedAccess");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"sr.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"SRService");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Streams Drivers");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"System Bus Extender");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Tcpip");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"TDI");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"tdpipe.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"tdtcp.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"termservice");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vga.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"vgasave.sys");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"WinMgmt");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"WZCSVC");
        RegCloseKey(fKey);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\SafeBoot",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"Network");
        RegCloseKey(fKey);



        RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control",0,KEY_SET_VALUE,&fKey);
        RegDeleteKey(fKey,"SafeBoot");
        RegCloseKey(fKey);

}
