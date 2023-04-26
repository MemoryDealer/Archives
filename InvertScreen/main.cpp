#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>

#include "resource.h"

HINSTANCE hInst;

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            srand(GetTickCount());
            return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                /*
                 * TODO: Add more control ID's, when needed.
                 */
                case IDC_BTN_QUIT:
                    EndDialog(hwndDlg, 0);
                    return TRUE;

                case IDC_BTN_TEST:
                    {
                        HDC hdc = GetWindowDC(NULL);
                        long color;

                        for (int y = 0; y < GetSystemMetrics(SM_CYSCREEN); y++)
                        {
                            for (int x = 0; x < GetSystemMetrics(SM_CXSCREEN); x++)
                            {
                                color = GetPixel(hdc, x, y);
                                color <<= rand() % 15 + 1;
                                color = ~color;
                                color >>= rand() % 15 + 1;
                                SetPixel(hdc, x, y, color);
                            }
                        }
                        ReleaseDC(NULL, hdc);
                    }
                    return TRUE;
            }
    }

    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;

    // The user interface is a modal dialog box
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, DialogProc);
}
