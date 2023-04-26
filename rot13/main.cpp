/* Rot13 Encrypter/Decrypter by Sparky
 * 2010
 */

#define _WIN32_WINNT 0x501
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <shlwapi.h>
#include "resource.h"

/* Control ID's */
#define IDC_EDT_SOURCE 101
#define IDC_EDT_OUTPUT 102
#define IDC_BTN_CRYPT  103

/* Global Variables */
HINSTANCE g_hInst = NULL;
HFONT g_hFont     = NULL;
HFONT g_btnFont   = NULL;

/*  Make the class name into a global variable  */
char szClassName[] = "Rot13 Encrypter/DecrypterClass";

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

char* xstrchr(const char *str, char ch)
{
    while(*str && *str != ch) str++;
    return (*str == ch) ? (char*)str : NULL;
}

char rot13c(char c)
{
    char u[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char l[] = "abcdefghijklmnopqrstuvwxyz";
    char *p;

    if((p = xstrchr(u, c)) != NULL)
        return u[((p-u) + 13) % 26];
    else if((p = xstrchr(l, c)) != NULL)
        return l[((p-l) + 13) % 26];
    else
        return c;
}

void rot13(char* buf, char* in)
{
    while(*in)
        *buf++ = rot13c(*in++);
    *buf = 0;
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    OSVERSIONINFO lpVer;
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    DWORD dwExStyle;
    HDC hdc;

    g_hInst = hThisInstance;

    InitCommonControls();

    ZeroMemory(&messages, sizeof(MSG));
    ZeroMemory(&wincl, sizeof(WNDCLASSEX));

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_MAIN));
    wincl.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 0, 0, 0);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    dwExStyle = WS_EX_APPWINDOW;

    /* Get OS version */
    lpVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&lpVer) && (lpVer.dwMajorVersion > 5 ||
		(lpVer.dwMajorVersion == 5 && lpVer.dwMinorVersion == 1)))
		dwExStyle |= WS_EX_COMPOSITED;

    /* Create the main font */
    hdc = GetDC(HWND_DESKTOP);
    g_hFont = CreateFont(-MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72),
        0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, TEXT("Tahoma"));

    g_btnFont = CreateFont(-MulDiv(15, GetDeviceCaps(hdc, LOGPIXELSY), 72),
        0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, TEXT("Poor Richard"));
    ReleaseDC(HWND_DESKTOP, hdc);

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           dwExStyle,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           TEXT("Rot13 Encrypter/Decrypter"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           400,                 /* The programs width */
           430,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    if(!hwnd)
        return -1;

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND s_hLastFocus;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            {
                HWND hChild;

                // create source edit box and output edit box
                for(int i=0; i<2; ++i)
                {
                    hChild = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL,
                        ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL | WS_VSCROLL
                        | WS_CHILD | WS_TABSTOP | WS_VISIBLE,
                        0, 0, 0, 0, hwnd, (i == 0) ? (HMENU)IDC_EDT_SOURCE : (HMENU)IDC_EDT_OUTPUT,
                        g_hInst, NULL);
                    if(!hChild) return -1;
                    // set the font
                    SendMessage(hChild, WM_SETFONT, (WPARAM)g_hFont, FALSE);
                }

                // create encrypt/decrypt button
                hChild = CreateWindowEx(0, WC_BUTTON, TEXT("&Encrypt/Decrypt"),
                    BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_TABSTOP | WS_VISIBLE,
                    0, 0, 0, 0, hwnd, (HMENU)IDC_BTN_CRYPT, g_hInst, NULL);
                SendMessage(hChild, WM_SETFONT, (WPARAM)g_btnFont, FALSE);
            }
            return 0;

        case WM_GETMINMAXINFO:
            {
                LPMINMAXINFO lpInfo = (LPMINMAXINFO)lParam;
                lpInfo->ptMinTrackSize.x = 400, lpInfo->ptMinTrackSize.y = 430;
            }
            return 0;

        case WM_WINDOWPOSCHANGING:
        case WM_WINDOWPOSCHANGED:
            {
                HDWP hDWP;
                RECT rc;

                if(hDWP = BeginDeferWindowPos(5))
                {
                    GetClientRect(hwnd, &rc);

                    hDWP = DeferWindowPos(hDWP, GetDlgItem(hwnd, IDC_EDT_SOURCE), NULL,
                        10, 10, (rc.right - 20), 150,
                        SWP_NOZORDER | SWP_NOREDRAW);

                    hDWP = DeferWindowPos(hDWP, GetDlgItem(hwnd, IDC_EDT_OUTPUT), NULL,
                        10, (rc.bottom - 160), (rc.right - 20), 150,
                        SWP_NOZORDER | SWP_NOREDRAW);

                    hDWP = DeferWindowPos(hDWP, GetDlgItem(hwnd, IDC_BTN_CRYPT), NULL,
                        10, 168, (rc.right - 20), (rc.bottom - 335),
                        SWP_NOZORDER | SWP_NOREDRAW);

                    EndDeferWindowPos(hDWP);

                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN |
                        RDW_ERASE | RDW_NOFRAME | RDW_UPDATENOW);
                }
            }
            return 0;

        case WM_ACTIVATE:
            if(LOWORD(wParam) == WA_INACTIVE)
                s_hLastFocus = GetFocus();
            return 0;

        case WM_SETFOCUS:
            if(s_hLastFocus)
                SetFocus(s_hLastFocus);
            return 0;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDR_FILE_CLEAR:
                    {
                        HWND hEditSource = GetDlgItem(hwnd, IDC_EDT_SOURCE);
                        HWND hEditOutput = GetDlgItem(hwnd, IDC_EDT_OUTPUT);

                        for(int i=0; i<2; ++i)
                            SetWindowText((i == 0) ? hEditSource : hEditOutput, "");
                    }
                    break;

                case IDR_FILE_QUIT:
                    PostQuitMessage(0);
                    break;

                case IDR_HELP_ABOUT:
                    MessageBox(NULL, "Rot13 Encrypter/Decrypter by Sparky\n\n"
                                     "\t2010 - Open Source", "About", MB_ICONINFORMATION);
                    break;

                case IDC_BTN_CRYPT:
                    {
                        const int MAX = 65000;
                        HWND hEditSource = GetDlgItem(hwnd, IDC_EDT_SOURCE);
                        HWND hEditOutput = GetDlgItem(hwnd, IDC_EDT_OUTPUT);
                        TCHAR szBuffer[MAX] = {0}, szOutput[MAX] = {0};
                        int len = 0;

                        len = GetWindowTextLength(hEditSource);
                        if(len <= 0)
                        {
                            MessageBox(hwnd, "Enter text to crypt", 0, MB_ICONWARNING);
                            break;
                        }
                        else if(len > MAX)
                        {
                            MessageBox(NULL, "Query too large", 0, MB_ICONERROR);
                            break;
                        }
                        GetWindowText(hEditSource, szBuffer, len + 1);

                        /* Encrypt/Decrypt the string */
                        rot13(szOutput, szBuffer);

                        /* Show the output */
                        SetWindowText(hEditOutput, szOutput);
                    }
                    break;
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            return 0;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
