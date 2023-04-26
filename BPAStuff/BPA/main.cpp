/*
    Developed by Jordan Sparks (Sparky) 2010
*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <tchar.h>
#include <fstream>
#include "resource.h"

#define endf "\n\n" << lnbreak << "\n\n"

using namespace std;

HWND hWnd = NULL;
bool bServer0       = true,
     bServer1       = false,
     bFirewall0     = true,
     bFirewall1     = false,
     bRouter0       = true,
     bRouter1       = false,
     bSwitch0       = true,
     bSwitch1       = false,
     bAntivirus0    = true,
     bAntivirus1    = false,
     bAntivirus2    = false,
     bCalendar0     = true,
     bCalendar1     = false,
     bModem0        = true,
     bModem1        = false;

// quantity vars
int qServer         = 1,
    qFirewall       = 1,
    qRouter         = 1,
    qSwitch         = 1,
    qAntivirus      = 1,
    qCalendar       = 1,
    qModem          = 1;

int cQ = 0;

LPSTR pServer       = NULL,
      pFirewall     = NULL,
      pRouter       = NULL,
      pSwitch       = NULL,
      pAntivirus    = NULL,
      pCalendar     = NULL,
      pModem        = NULL;

double total = 0.0;

char file_out[MAX_PATH] = {0};

typedef basic_string<TCHAR> ustring;

inline UINT AddString(const HWND hCombo, const ustring& s)
{
    return static_cast<UINT>(SendMessage(hCombo, CB_ADDSTRING, 0,
                             reinterpret_cast<LPARAM>(s.c_str())));
}

void alloc_mem()
{
    pServer         = (LPSTR)GlobalAlloc(GPTR, 255);
    pFirewall       = (LPSTR)GlobalAlloc(GPTR, 255);
    pRouter         = (LPSTR)GlobalAlloc(GPTR, 255);
    pSwitch         = (LPSTR)GlobalAlloc(GPTR, 255);
    pAntivirus      = (LPSTR)GlobalAlloc(GPTR, 255);
    pCalendar       = (LPSTR)GlobalAlloc(GPTR, 255);
    pModem          = (LPSTR)GlobalAlloc(GPTR, 255);
}

void free_mem()
{
    GlobalFree((HANDLE)pServer);
    GlobalFree((HANDLE)pFirewall);
    GlobalFree((HANDLE)pRouter);
    GlobalFree((HANDLE)pSwitch);
    GlobalFree((HANDLE)pAntivirus);
    GlobalFree((HANDLE)pCalendar);
    GlobalFree((HANDLE)pModem);
}

void reset()
{
    free_mem();
    alloc_mem();

    cQ = 0;
    qServer         = 1;
    qFirewall       = 1;
    qRouter         = 1;
    qSwitch         = 1;
    qAntivirus      = 1;
    qCalendar       = 1;
    qModem          = 1;

    HWND hwnd = hWnd;
    HWND hServer0       = GetDlgItem(hwnd, IDC_RDO_SERVER_0),
         hServer1       = GetDlgItem(hwnd, IDC_RDO_SERVER_1),
         hFirewall0     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_0),
         hFirewall1     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_1),
         hRouter0       = GetDlgItem(hwnd, IDC_RDO_ROUTER_0),
         hRouter1       = GetDlgItem(hwnd, IDC_RDO_ROUTER_1),
         hSwitch0       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_0),
         hSwitch1       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_1),
         hAntivirus0    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_0),
         hAntivirus1    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_1),
         hAntivirus2    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_2),
         hCalendar0     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_0),
         hCalendar1     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_1),
         hModem0        = GetDlgItem(hwnd, IDC_RDO_MODEM_0),
         hModem1        = GetDlgItem(hwnd, IDC_RDO_MODEM_1);

    SendMessage(hServer0, BM_SETCHECK, 1, 0);
    SendMessage(hServer1, BM_SETCHECK, 0, 0);
    bServer0 = true;
    bServer1 = false;

    SendMessage(hFirewall0, BM_SETCHECK, 1, 0);
    SendMessage(hFirewall1, BM_SETCHECK, 0, 0);
    bFirewall0 = true;
    bFirewall1 = false;

    SendMessage(hRouter0, BM_SETCHECK, 1, 0);
    SendMessage(hRouter1, BM_SETCHECK, 0, 0);
    bRouter0 = true;
    bRouter1 = false;

    SendMessage(hSwitch0, BM_SETCHECK, 1, 0);
    SendMessage(hSwitch1, BM_SETCHECK, 0, 0);
    bSwitch0 = true;
    bSwitch1 = false;

    SendMessage(hAntivirus0, BM_SETCHECK, 1, 0);
    SendMessage(hAntivirus1, BM_SETCHECK, 0, 0);
    SendMessage(hAntivirus2, BM_SETCHECK, 0, 0);
    bAntivirus0 = true;
    bAntivirus1 = false;
    bAntivirus2 = false;

    SendMessage(hCalendar0, BM_SETCHECK, 1, 0);
    SendMessage(hCalendar1, BM_SETCHECK, 0, 0);
    bCalendar0 = true;
    bCalendar1 = false;

    SendMessage(hModem0, BM_SETCHECK, 1, 0);
    SendMessage(hModem1, BM_SETCHECK, 0, 0);
    bModem0 = true;
    bModem1 = false;

    SetWindowText(GetDlgItem(hWnd, IDC_EDT_QUANTITY), "1");
}

int get_save_file()
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0";
    ofn.lpstrFile = file_out;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = ".txt";

    if(GetSaveFileName(&ofn))
        return 0;
    else
        return -1;
}

void write_file()
{
    if(get_save_file() == -1) { return; }

    string lnbreak = "-------------";
    ofstream out;
    out.open(file_out);
    out << "--Network Financial Plans--\n\n";
    out << "Server: " << pServer << endf;
    out << "Firewall: " << pFirewall << endf;
    out << "Router: " << pRouter << endf;
    out << "Switch: " << pSwitch << endf;
    out << "Anti-virus: " << pAntivirus << endf;
    out << "Calendar Solution: " << pCalendar << endf;
    out << "\n\nTotal Price: $" << total << endl;
    out.close();

    EnableWindow(GetDlgItem(hWnd, IDC_BTN_VIEWFILE), true);
}

void update_quantity()
{
    char qbuf[64];
    GetWindowText(GetDlgItem(hWnd, IDC_EDT_QUANTITY), qbuf, 64);
    int nQ = atoi(qbuf);

    switch(cQ)
    {
        case 0:
            qServer = nQ;
            break;

        case 1:
            qFirewall = nQ;
            break;

        case 2:
            qRouter = nQ;
            break;

        case 3:
            qSwitch = nQ;
            break;

        case 4:
            qAntivirus = nQ;
            break;

        case 5:
            qCalendar = nQ;
            break;

        case 6:
            qModem = nQ;
            break;

        default:
            break;
    }
}

void get_quantity(int arg)
{
    char qbuf[64];
    int iQ = 0;

    switch(arg)
    {
        case 0:
            iQ = qServer;
            cQ = 0;
            break;

        case 1:
            iQ = qFirewall;
            cQ = 1;
            break;

        case 2:
            iQ = qRouter;
            cQ = 2;
            break;

        case 3:
            iQ = qSwitch;
            cQ = 3;
            break;

        case 4:
            iQ = qAntivirus;
            cQ = 4;
            break;

        case 5:
            iQ = qCalendar;
            cQ = 5;
            break;

        case 6:
            iQ = qModem;
            cQ = 6;
            break;

        default:
            break;
    }

    sprintf(qbuf, "%i", iQ);
    SetWindowText(GetDlgItem(hWnd, IDC_EDT_QUANTITY), qbuf);
}

void update_total()
{
    total = 0.0;

    if(bServer0){
        total += (449.00 * qServer);
        sprintf(pServer, "%s%i%s", "$449.00 - HP ProLiant ML110 G6 Server series (", qServer, ")");
    }
    else if(bServer1){
        total += (829.00 * qServer);
        sprintf(pServer, "%s%i%s", "$829.00 - HP ProLiant ML310 G5p Server series (", qServer, ")");
    }

    if(bFirewall0){
        total += (350.00 * qFirewall); // add crap to the sprintf functions
        sprintf(pFirewall, "%s%i%s", "$350.00 - Symantec SOHO Firewall (", qFirewall, ")");
    }
    else if(bFirewall1){
        total += (795.00 * qFirewall);
        sprintf(pFirewall, "%s%i%s", "$795.00 - SonicWALL TZ 210 Series (", qFirewall, ")");
    }

    if(bRouter0){
        total += (118.99 * qRouter);
        sprintf(pRouter, "%s%i%s", "$118.99 - Netgear FVG318 8-PORT Prosafe 802.11G Wireless VPN Firewall Router (", qRouter, ")");
    }
    else if(bRouter1){
        total += (449.99 * qRouter);
        sprintf(pRouter, "%s%i%s", "$449.99 - Cisco Small Business RV016 10/100Mbps Router 2 x 10/100Mbps WAN (", qRouter, ")");
    }

    if(bSwitch0){
        total += (700.00 * qSwitch);
        sprintf(pSwitch, "%s%i%s", "$700.00 - ProSafe 48-port 10/100 L3 Managed Stackable Switch with 4 Gigabit Ports (", qSwitch, ")");
    }
    else if(bSwitch1){
        total += (1005.00 * qSwitch);
        sprintf(pSwitch, "%s%i%s", "$1,005.00 - 3COM 4200G Switch (", qSwitch, ")");
    }

    if(bAntivirus1){
        total += (1150.0 * qAntivirus);
        sprintf(pAntivirus, "%s%i%s", "$1,150 - McAffee Total Protection Service (", qAntivirus, ")");
    }
    else if(bAntivirus2){
        total += (2087.00 * qAntivirus);
        sprintf(pAntivirus, "%s%i%s", "$2,087.00 - Symantec EndPoint (", qAntivirus, ")");
    }
    else if(bAntivirus0){
        sprintf(pAntivirus, "%s", "$0.00 - Spybot Search and Destroy (Freeware)");
    }

    if(bCalendar0){
        total += (1121.00 * qCalendar);
        sprintf(pCalendar, "%s%i%s", "$1,121.00 - Microsoft Exchange Server (", qCalendar, ")");
    }
    else if(bCalendar1){
        total += (1795.00 * qCalendar);
        sprintf(pCalendar, "%s%i%s", "$1,795.00 - Lotus Notes Server (", qCalendar, ")");
    }

    if(bModem0){
        total += (550.00 * qModem);
        sprintf(pModem, "%s%i%s", " (", qModem, ")");
    }
    else if(bModem1){
        total += (804.95 * qModem);
        sprintf(pModem, "%s%i%s", " (", qModem, ")");
    }

    // labor cost
    total += 7200.0;


    char buffer[128];
    sprintf(buffer, "$%0.2f", total);
    SetWindowText(GetDlgItem(hWnd, IDC_EDT_PRICE), buffer);
}

void set_budgeted()
{
    //bah a waste of code :(
    HWND hwnd = hWnd;
    HWND hServer0       = GetDlgItem(hwnd, IDC_RDO_SERVER_0),
         hServer1       = GetDlgItem(hwnd, IDC_RDO_SERVER_1),
         hFirewall0     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_0),
         hFirewall1     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_1),
         hRouter0       = GetDlgItem(hwnd, IDC_RDO_ROUTER_0),
         hRouter1       = GetDlgItem(hwnd, IDC_RDO_ROUTER_1),
         hSwitch0       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_0),
         hSwitch1       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_1),
         hAntivirus0    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_0),
         hAntivirus1    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_1),
         hAntivirus2    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_2),
         hCalendar0     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_0),
         hCalendar1     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_1),
         hModem0        = GetDlgItem(hwnd, IDC_RDO_MODEM_0),
         hModem1        = GetDlgItem(hwnd, IDC_RDO_MODEM_1);

    SendMessage(hServer0, BM_SETCHECK, 1, 0);
    SendMessage(hServer1, BM_SETCHECK, 0, 0);
    bServer0 = true;
    bServer1 = false;
    qServer = 3;

    SendMessage(hFirewall0, BM_SETCHECK, 1, 0);
    SendMessage(hFirewall1, BM_SETCHECK, 0, 0);
    bFirewall0 = true;
    bFirewall1 = false;
    qFirewall = 2;

    SendMessage(hRouter0, BM_SETCHECK, 1, 0);
    SendMessage(hRouter1, BM_SETCHECK, 0, 0);
    bRouter0 = true;
    bRouter1 = false;
    qRouter = 3;

    SendMessage(hSwitch0, BM_SETCHECK, 1, 0);
    SendMessage(hSwitch1, BM_SETCHECK, 0, 0);
    bSwitch0 = true;
    bSwitch1 = false;
    qSwitch = 4;

    SendMessage(hAntivirus0, BM_SETCHECK, 1, 0);
    SendMessage(hAntivirus1, BM_SETCHECK, 0, 0);
    SendMessage(hAntivirus2, BM_SETCHECK, 0, 0);
    bAntivirus0 = true;
    bAntivirus1 = false;
    bAntivirus2 = false;
    qAntivirus = 1;

    SendMessage(hCalendar0, BM_SETCHECK, 1, 0);
    SendMessage(hCalendar1, BM_SETCHECK, 0, 0);
    bCalendar0 = true;
    bCalendar1 = false;
    qCalendar = 1;

    SendMessage(hModem0, BM_SETCHECK, 1, 0);
    SendMessage(hModem1, BM_SETCHECK, 0, 0);
    bModem0 = true;
    bModem1 = false;
    qModem = 1;

    update_total();
}

void set_optimal()
{
    // :(
    HWND hwnd = hWnd;
    HWND hServer0       = GetDlgItem(hwnd, IDC_RDO_SERVER_0),
         hServer1       = GetDlgItem(hwnd, IDC_RDO_SERVER_1),
         hFirewall0     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_0),
         hFirewall1     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_1),
         hRouter0       = GetDlgItem(hwnd, IDC_RDO_ROUTER_0),
         hRouter1       = GetDlgItem(hwnd, IDC_RDO_ROUTER_1),
         hSwitch0       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_0),
         hSwitch1       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_1),
         hAntivirus0    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_0),
         hAntivirus1    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_1),
         hAntivirus2    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_2),
         hCalendar0     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_0),
         hCalendar1     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_1),
         hModem0        = GetDlgItem(hwnd, IDC_RDO_MODEM_0),
         hModem1        = GetDlgItem(hwnd, IDC_RDO_MODEM_1);

    SendMessage(hServer1, BM_SETCHECK, 1, 0);
    SendMessage(hServer0, BM_SETCHECK, 0, 0);
    bServer1 = true;
    bServer0 = false;
    qServer = 7;

    SendMessage(hFirewall0, BM_SETCHECK, 1, 0);
    SendMessage(hFirewall1, BM_SETCHECK, 0, 0);
    bFirewall0 = true;
    bFirewall1 = false;
    qFirewall = 4;

    SendMessage(hRouter0, BM_SETCHECK, 1, 0);
    SendMessage(hRouter1, BM_SETCHECK, 0, 0);
    bRouter0 = true;
    bRouter1 = false;
    qRouter = 3;

    SendMessage(hSwitch0, BM_SETCHECK, 1, 0);
    SendMessage(hSwitch1, BM_SETCHECK, 0, 0);
    bSwitch0 = true;
    bSwitch1 = false;
    qSwitch = 4;

    SendMessage(hAntivirus1, BM_SETCHECK, 1, 0);
    SendMessage(hAntivirus0, BM_SETCHECK, 0, 0);
    SendMessage(hAntivirus2, BM_SETCHECK, 0, 0);
    bAntivirus1 = true;
    bAntivirus0 = false;
    bAntivirus2 = false;
    qAntivirus = 1;

    SendMessage(hCalendar0, BM_SETCHECK, 1, 0);
    SendMessage(hCalendar1, BM_SETCHECK, 0, 0);
    bCalendar0 = true;
    bCalendar1 = false;
    qCalendar = 1;

    SendMessage(hModem1, BM_SETCHECK, 1, 0);
    SendMessage(hModem0, BM_SETCHECK, 0, 0);
    bModem1 = true;
    bModem0 = false;
    qModem = 1;

    update_total();
}

void set_future()
{
    HWND hwnd = hWnd;
    HWND hServer0       = GetDlgItem(hwnd, IDC_RDO_SERVER_0),
         hServer1       = GetDlgItem(hwnd, IDC_RDO_SERVER_1),
         hFirewall0     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_0),
         hFirewall1     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_1),
         hRouter0       = GetDlgItem(hwnd, IDC_RDO_ROUTER_0),
         hRouter1       = GetDlgItem(hwnd, IDC_RDO_ROUTER_1),
         hSwitch0       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_0),
         hSwitch1       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_1),
         hAntivirus0    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_0),
         hAntivirus1    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_1),
         hAntivirus2    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_2),
         hCalendar0     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_0),
         hCalendar1     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_1),
         hModem0        = GetDlgItem(hwnd, IDC_RDO_MODEM_0),
         hModem1        = GetDlgItem(hwnd, IDC_RDO_MODEM_1);

    SendMessage(hServer1, BM_SETCHECK, 1, 0);
    SendMessage(hServer0, BM_SETCHECK, 0, 0);
    bServer1 = true;
    bServer0 = false;
    qServer = 7;

    SendMessage(hFirewall1, BM_SETCHECK, 1, 0);
    SendMessage(hFirewall0, BM_SETCHECK, 0, 0);
    bFirewall1 = true;
    bFirewall0 = false;
    qFirewall = 4;

    SendMessage(hRouter1, BM_SETCHECK, 1, 0);
    SendMessage(hRouter0, BM_SETCHECK, 0, 0);
    bRouter1 = true;
    bRouter0 = false;
    qRouter = 3;

    SendMessage(hSwitch1, BM_SETCHECK, 1, 0);
    SendMessage(hSwitch0, BM_SETCHECK, 0, 0);
    bSwitch1 = true;
    bSwitch0 = false;
    qSwitch = 4;

    SendMessage(hAntivirus2, BM_SETCHECK, 1, 0);
    SendMessage(hAntivirus0, BM_SETCHECK, 0, 0);
    SendMessage(hAntivirus1, BM_SETCHECK, 0, 0);
    bAntivirus2 = true;
    bAntivirus0 = false;
    bAntivirus1 = false;
    qAntivirus = 1;

    SendMessage(hCalendar1, BM_SETCHECK, 1, 0);
    SendMessage(hCalendar0, BM_SETCHECK, 0, 0);
    bCalendar1 = true;
    bCalendar0 = false;
    qCalendar = 1;

    SendMessage(hModem1, BM_SETCHECK, 1, 0);
    SendMessage(hModem0, BM_SETCHECK, 0, 0);
    bModem1 = true;
    bModem0 = false;
    qModem = 1;

    update_total();
}

void calc_preset(LPCSTR buf)
{
    string cmp = buf;
    if(cmp == "Budgeted"){
        set_budgeted();
    }
    else if(cmp == "Optimal"){
        set_optimal();
    }
    else if(cmp == "Future"){
        set_future();
    }
    SetFocus(GetDlgItem(hWnd, IDC_RDO_SERVER_0));
}

BOOL CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hServer0       = GetDlgItem(hwnd, IDC_RDO_SERVER_0),
         hServer1       = GetDlgItem(hwnd, IDC_RDO_SERVER_1),
         hFirewall0     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_0),
         hFirewall1     = GetDlgItem(hwnd, IDC_RDO_FIREWALL_1),
         hRouter0       = GetDlgItem(hwnd, IDC_RDO_ROUTER_0),
         hRouter1       = GetDlgItem(hwnd, IDC_RDO_ROUTER_1),
         hSwitch0       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_0),
         hSwitch1       = GetDlgItem(hwnd, IDC_RDO_SWITCHES_1),
         hAntivirus0    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_0),
         hAntivirus1    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_1),
         hAntivirus2    = GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_2),
         hCalendar0     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_0),
         hCalendar1     = GetDlgItem(hwnd, IDC_RDO_CALENDAR_1),
         hModem0        = GetDlgItem(hwnd, IDC_RDO_MODEM_0),
         hModem1        = GetDlgItem(hwnd, IDC_RDO_MODEM_1);

    hWnd = hwnd;

    switch(msg)
    {
        case WM_INITDIALOG:
            alloc_mem();

            SendMessage(GetDlgItem(hwnd, IDC_RDO_SERVER_0), BM_SETCHECK, 1, 0);
            SendMessage(GetDlgItem(hwnd, IDC_RDO_FIREWALL_0), BM_SETCHECK, 1, 0);
            SendMessage(GetDlgItem(hwnd, IDC_RDO_ROUTER_0), BM_SETCHECK, 1, 0);
            SendMessage(GetDlgItem(hwnd, IDC_RDO_SWITCHES_0), BM_SETCHECK, 1, 0);
            SendMessage(GetDlgItem(hwnd, IDC_RDO_ANTIVIRUS_0), BM_SETCHECK, 1, 0);
            SendMessage(GetDlgItem(hwnd, IDC_RDO_CALENDAR_0), BM_SETCHECK, 1, 0);
            SendMessage(GetDlgItem(hwnd, IDC_RDO_MODEM_0), BM_SETCHECK, 1, 0);
            SetFocus(hServer0);

            AddString(GetDlgItem(hwnd, IDC_CMB_BUDGET), _T("None"));
            AddString(GetDlgItem(hwnd, IDC_CMB_BUDGET), _T("Budgeted"));
            AddString(GetDlgItem(hwnd, IDC_CMB_BUDGET), _T("Optimal"));
            AddString(GetDlgItem(hwnd, IDC_CMB_BUDGET), _T("Future"));

            // set the main icon
            SendMessage(hwnd, WM_SETICON, ICON_SMALL,
             (LPARAM)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));

            // load the menu
            HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU));
            SetMenu(hwnd, hMenu);

            EnableWindow(GetDlgItem(hwnd, IDC_BTN_VIEWFILE), false);

            get_quantity(0);
            update_total();
            return 0;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                // menu
                case IDR_FILE_QUIT:
                    free_mem();
                    EndDialog(hwnd, 0);
                    break;
                //*******************************
                // Manually control radio buttons
                //*******************************
                case IDC_RDO_SERVER_0:
                    if(!bServer0){
                        SendMessage(hServer0, BM_SETCHECK, 1, 0); // check it
                        SendMessage(hServer1, BM_SETCHECK, 0, 0); // uncheck it
                        bServer0 = true;
                        bServer1 = false;
                    }
                    get_quantity(0);
                    update_total();
                    break;

                case IDC_RDO_SERVER_1:
                    if(!bServer1){
                        SendMessage(hServer1, BM_SETCHECK, 1, 0);
                        SendMessage(hServer0, BM_SETCHECK, 0, 0);
                        bServer1 = true;
                        bServer0 = false;
                    }
                    get_quantity(0);
                    update_total();
                    break;

                case IDC_RDO_FIREWALL_0:
                    if(!bFirewall0){
                        SendMessage(hFirewall0, BM_SETCHECK, 1, 0);
                        SendMessage(hFirewall1, BM_SETCHECK, 0, 0);
                        bFirewall0 = true;
                        bFirewall1 = false;
                    }
                    get_quantity(1);
                    update_total();
                    break;

                case IDC_RDO_FIREWALL_1:
                    if(!bFirewall1){
                        SendMessage(hFirewall1, BM_SETCHECK, 1, 0);
                        SendMessage(hFirewall0, BM_SETCHECK, 0, 0);
                        bFirewall1 = true;
                        bFirewall0 = false;
                    }
                    get_quantity(1);
                    update_total();
                    break;

                case IDC_RDO_ROUTER_0:
                    if(!bRouter0){
                        SendMessage(hRouter0, BM_SETCHECK, 1, 0);
                        SendMessage(hRouter1, BM_SETCHECK, 0, 0);
                        bRouter0 = true;
                        bRouter1 = false;
                    }
                    get_quantity(2);
                    update_total();
                    break;

                case IDC_RDO_ROUTER_1:
                    if(!bRouter1){
                        SendMessage(hRouter1, BM_SETCHECK, 1, 0);
                        SendMessage(hRouter0, BM_SETCHECK, 0, 0);
                        bRouter1 = true;
                        bRouter0 = false;
                    }
                    get_quantity(2);
                    update_total();
                    break;

                case IDC_RDO_SWITCHES_0:
                    if(!bSwitch0){
                        SendMessage(hSwitch0, BM_SETCHECK, 1, 0);
                        SendMessage(hSwitch1, BM_SETCHECK, 0, 0);
                        bSwitch0 = true;
                        bSwitch1 = false;
                    }
                    get_quantity(3);
                    update_total();
                    break;

                case IDC_RDO_SWITCHES_1:
                    if(!bSwitch1){
                        SendMessage(hSwitch1, BM_SETCHECK, 1, 0);
                        SendMessage(hSwitch0, BM_SETCHECK, 0, 0);
                        bSwitch1 = true;
                        bSwitch0 = false;
                    }
                    get_quantity(3);
                    update_total();
                    break;

                case IDC_RDO_ANTIVIRUS_0:
                    if(!bAntivirus0){
                        SendMessage(hAntivirus0, BM_SETCHECK, 1, 0);
                        SendMessage(hAntivirus1, BM_SETCHECK, 0, 0);
                        SendMessage(hAntivirus2, BM_SETCHECK, 0, 0);
                        bAntivirus0 = true;
                        bAntivirus1 = false;
                        bAntivirus2 = false;
                    }
                    get_quantity(4);
                    update_total();
                    break;

                case IDC_RDO_ANTIVIRUS_1:
                    if(!bAntivirus1){
                        SendMessage(hAntivirus1, BM_SETCHECK, 1, 0);
                        SendMessage(hAntivirus0, BM_SETCHECK, 0, 0);
                        SendMessage(hAntivirus2, BM_SETCHECK, 0, 0);
                        bAntivirus1 = true;
                        bAntivirus0 = false;
                        bAntivirus2 = false;
                    }
                    get_quantity(4);
                    update_total();
                    break;

                case IDC_RDO_ANTIVIRUS_2:
                    if(!bAntivirus2){
                        SendMessage(hAntivirus2, BM_SETCHECK, 1, 0);
                        SendMessage(hAntivirus0, BM_SETCHECK, 0, 0);
                        SendMessage(hAntivirus1, BM_SETCHECK, 0, 0);
                        bAntivirus2 = true;
                        bAntivirus0 = false;
                        bAntivirus1 = false;
                    }
                    get_quantity(4);
                    update_total();
                    break;

                case IDC_RDO_CALENDAR_0:
                    if(!bCalendar0){
                        SendMessage(hCalendar0, BM_SETCHECK, 1, 0);
                        SendMessage(hCalendar1, BM_SETCHECK, 0, 0);
                        bCalendar0 = true;
                        bCalendar1 = false;
                    }
                    get_quantity(5);
                    update_total();
                    break;

                case IDC_RDO_CALENDAR_1:
                    if(!bCalendar1){
                        SendMessage(hCalendar1, BM_SETCHECK, 1, 0);
                        SendMessage(hCalendar0, BM_SETCHECK, 0, 0);
                        bCalendar1 = true;
                        bCalendar0 = false;
                    }
                    get_quantity(5);
                    update_total();
                    break;

                case IDC_RDO_MODEM_0:
                    if(!bModem0){
                        SendMessage(hModem0, BM_SETCHECK, 1, 0);
                        SendMessage(hModem1, BM_SETCHECK, 0, 0);
                        bModem0 = true;
                        bModem1 = false;
                    }
                    get_quantity(6);
                    update_total();
                    break;

                case IDC_RDO_MODEM_1:
                    if(!bModem1){
                        SendMessage(hModem1, BM_SETCHECK, 1, 0);
                        SendMessage(hModem0, BM_SETCHECK, 0, 0);
                        bModem1 = true;
                        bModem0 = false;
                    }
                    get_quantity(6);
                    update_total();
                    break;

                //*******************************
                // End of manual control
                //*******************************

                case IDC_BTN_APPLYPRESET:
                    {
                        int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_CMB_BUDGET));
                        if(len <= 0){ break; }
                        LPSTR buf = (LPSTR)GlobalAlloc(GPTR, len + 1);
                        GetWindowText(GetDlgItem(hwnd, IDC_CMB_BUDGET), buf, len + 1);
                        calc_preset(buf);
                        update_total();
                    }
                    break;

                case IDC_BTN_SAVEFILE:
                    write_file();
                    break;

                case IDC_BTN_VIEWFILE:
                    {
                        STARTUPINFO siStartupInfo;
                        PROCESS_INFORMATION piProcessInfo;
                        char cmd[255];
                        char arg[255];

                        memset(&siStartupInfo, 0, sizeof(siStartupInfo));
                        memset(&piProcessInfo, 0, sizeof(piProcessInfo));

                        siStartupInfo.cb = sizeof(siStartupInfo);
                        GetSystemDirectory(cmd, sizeof(cmd));
                        strcat(cmd, "\\cmd.exe");

                        sprintf(arg, "%s %s", " /C start", file_out);

                        CreateProcess(  cmd, // Cmd.exe
                                        arg, // argument
                                        NULL,
                                        NULL,
                                        FALSE,
                                        CREATE_NO_WINDOW,
                                        NULL,
                                        NULL,
                                        &siStartupInfo,
                                        &piProcessInfo);
                    }
                    break;

                case IDC_BTN_APPLYQ:
                    update_quantity();
                    update_total();
                    break;

                case IDC_BTN_RESET:
                    reset();
                    update_quantity();
                    update_total();
                    break;
            }
            return 0;

        case WM_CLOSE:
            free_mem();
            EndDialog(hwnd, 0);
            return 0;

        default:
            return FALSE;
    }
    return TRUE;
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    return DialogBox(hThisInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, WndProc);
}
// didn't break a sweat  :)
