#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include "resource.h"

#define USE_SUBTOTAL 1
#define USE_TOTAL    2

using std::ofstream;
using std::endl;

char tmpData[255] = {0};
BOOL bPrint = FALSE;
BOOL bComment = FALSE;
LPSTR comment = NULL;

inline int ErrMsg(LPCSTR msg)
{
    return MessageBox(NULL, msg, 0, MB_ICONEXCLAMATION);
}

void create_file(LPSTR path, char name[], char subTotal[], char tax[], char total[])
{
    //! Print "John Mayo - GizMayo Repair" at top of txt file
    SYSTEMTIME st;
    ofstream file;

    GetLocalTime(&st);

        file.open(path);
        file << "\nDate: " << st.wMonth << "/" << st.wDay << "/" << st.wYear << endl;
        file << "\nJohn Mayo - GizMayo Repair\n";
        file << "--------------------------\n\n\n";
        file << "-Invoice for " << name << "-";
        if(bComment == TRUE)
        {
            file << "\nComment: " << comment;
        }
        file << "\n\n--------------------------\n\n";
        file << "Sub-Total: $" << subTotal << endl;
        file << "Tax:       $" << tax << endl;
        file << "-------\n";
        file << "Total:     $" << total << endl;
        file.close();
}

void calc_value(double val, HWND hwnd)
{
    double subTotal, tax, total;
    int calcType = 0;

    if(IsDlgButtonChecked(hwnd, IDC_RADIO_SUBTOTAL))
    {
        calcType = USE_SUBTOTAL;
    }
    else if(IsDlgButtonChecked(hwnd, IDC_RADIO_TOTAL))
    {
        calcType = USE_TOTAL;
    }
    else
    {
        calcType = USE_SUBTOTAL; // use default if error
    }

    if(calcType == USE_SUBTOTAL)
    {
        subTotal = val; // subTotal is given

        tax = (subTotal * 0.0825); // calculate tax
        total = (subTotal + tax); // calculate total
    }

    else if(calcType == USE_TOTAL)
    {
        total = val; // total is given

        subTotal = (total / 1.0825); // calculate subTotal
        tax = (total - subTotal); // calculate tax
    }

    // set the text boxes with the corresponding values
    char s_buf[128], x_buf[128], t_buf[128];

    sprintf(s_buf, "%0.2f", subTotal);
    sprintf(x_buf, "%0.2f", tax);
    sprintf(t_buf, "%0.2f", total);

    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_SUBTOTAL), s_buf);
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_TAX), x_buf);
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_TOTAL), t_buf);
}

void file_op(HWND hwnd, short arg)
{
    char name[128], subTotal[128], tax[128], total[128];
    HWND h_name         = GetDlgItem(hwnd, IDC_EDIT_NAME);
    HWND h_subTotal     = GetDlgItem(hwnd, IDC_EDIT_SUBTOTAL);
    HWND h_tax          = GetDlgItem(hwnd, IDC_EDIT_TAX);
    HWND h_total        = GetDlgItem(hwnd, IDC_EDIT_TOTAL);

    // get customer name
    int len = GetWindowTextLength(h_name);
    if(len <= 0)
    {
        ErrMsg("Please enter the customer's name.");
        return;
    }
    GetWindowText(h_name, name, len + 1);

    // get subtotal
    len = GetWindowTextLength(h_subTotal);
    if(len <= 0)
    {
        ErrMsg("Please enter the sub total value.");
        return;
    }
    GetWindowText(h_subTotal, subTotal, len + 1);

    // get tax
    len = GetWindowTextLength(h_tax);
    if(len <= 0)
    {
        ErrMsg("Please enter the tax value.");
        return;
    }
    GetWindowText(h_tax, tax, len + 1);

    // get total
    len = GetWindowTextLength(h_total);
    if(len <= 0)
    {
        ErrMsg("Please enter the total value.");
        return;
    }
    GetWindowText(h_total, total, len + 1);

    LPSTR filePath = (char*)GlobalAlloc(GPTR, MAX_PATH);
    if(arg != 2)
    {
        // Get the save file name path
        OPENFILENAME ofn; // file name struct
        ZeroMemory(&ofn,sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0";
        ofn.lpstrFile = filePath;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
        ofn.lpstrDefExt = ".txt";

        if(GetSaveFileName(&ofn)) // dispaly dialog to save the file
        {
            create_file(filePath, name, subTotal, tax, total);
        }
    }
    else if(arg == 2)
    {
        // create temp file
        SYSTEMTIME st;
        char buf[255];
        GetLocalTime(&st);
        GetCurrentDirectory(sizeof(filePath), filePath);
        strcat(filePath, "\\GizMayoTemp");
        CreateDirectory(filePath, NULL);
        sprintf(tmpData, "%s", filePath);

        sprintf(buf, "\\%s%d.%d.%d.txt", name, st.wMonth, st.wDay, st.wYear);
        strcat(filePath, buf);

        create_file(filePath, name, subTotal, tax, total);
    }

    if(arg != 1)
    {
        // print via ShellExecute()
        ShellExecute(NULL, "print", filePath, 0, 0, 0);
        bPrint = TRUE;
    }

    GlobalFree((HANDLE)filePath);
}

void temp_cleanup()
{
    // cleanup all temp files used for printing
    if(bPrint == FALSE) { return; }
    char buf[255], t_buf[255];
    HANDLE hFind;
    WIN32_FIND_DATA fd;

    sprintf(t_buf, "%s\\", tmpData);
    sprintf(buf, "%s", t_buf);
    strcat(t_buf, "\\*.txt");

    hFind = FindFirstFile(t_buf, &fd);
    strcat(buf, fd.cFileName);
    DeleteFile(buf);
    while(FindNextFile(hFind, &fd))
    {
        sprintf(buf, "%s\\", tmpData);
        strcat(buf, fd.cFileName);
        DeleteFile(buf);
    }
    FindClose(hFind);
    RemoveDirectory(tmpData);
}

BOOL CALLBACK ComProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND h_input = GetDlgItem(hwnd, IDC_EDIT_COMMENT);
    switch(msg)
    {
        case WM_INITDIALOG:
            if(bComment == TRUE)
            {
                SetWindowText(h_input, comment);
            }
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    int len = GetWindowTextLength(h_input);
                    if(len <= 0)
                    {
                        ErrMsg("Please enter a comment.");
                        break;
                    }

                    if(bComment == TRUE){ GlobalFree((HANDLE)comment); }
                    comment = (char*)GlobalAlloc(GPTR, len + 1);

                    GetWindowText(h_input, comment, len + 1);
                    bComment = TRUE;

                    EndDialog(hwnd, 0);
                break;

                case IDCANCEL:
                    EndDialog(hwnd, 0);
                break;
            }
        break;

        case WM_CLOSE:
            EndDialog(hwnd, 0);
        break;

        default:
            return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_INITDIALOG:
            HWND h_check = GetDlgItem(hwnd, IDC_BUTTON_CHECK);
            // set the main icon
            SendMessage(hwnd, WM_SETICON, ICON_SMALL,
             (LPARAM)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));

            // configure comment check button
            HICON hCheckIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CHECK_ICON));
            SendMessage(h_check, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hCheckIcon);
            ShowWindow(h_check, false);

            // load the menu
            HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU));
            SetMenu(hwnd, hMenu);

            // set the default radio button
            SendMessage(GetDlgItem(hwnd, IDC_RADIO_SUBTOTAL), BM_SETCHECK, 1, 0);
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDR_FILE_SAVE:
                case IDC_BUTTON_SAVE:
                    file_op(hwnd, 1);
                break;

                case IDR_FILE_PRINT:
                case IDC_BUTTON_PRINT:
                    file_op(hwnd, 2);
                break;

                case IDR_FILE_SAVE_PRINT:
                case IDC_BUTTON_SAVE_PRINT:
                    file_op(hwnd, 3);
                break;

                case IDR_FILE_QUIT:
                    temp_cleanup();
                    EndDialog(hwnd, 0);
                break;

                case IDR_HELP_ABOUT:
                    MessageBox(NULL, "GizMayo Invoice 1.0 - calculates total and sub-total prices with an 8.25% tax rate.\n\nCopyright(C) 2010 - Jordan Sparks\n\nContact: unixunited@live.com", "About", MB_ICONINFORMATION);
                break;

                case IDC_RADIO_SUBTOTAL:
                    SendMessage(GetDlgItem(hwnd, IDC_RADIO_SUBTOTAL), BM_SETCHECK, 1, 0);
                    SendMessage(GetDlgItem(hwnd, IDC_RADIO_TOTAL), BM_SETCHECK, 0, 0);
                break;

                case IDC_RADIO_TOTAL:
                    SendMessage(GetDlgItem(hwnd, IDC_RADIO_TOTAL), BM_SETCHECK, 1, 0);
                    SendMessage(GetDlgItem(hwnd, IDC_RADIO_SUBTOTAL), BM_SETCHECK, 0, 0);
                break;

                case IDC_BUTTON_CALC:
                {
                    LPSTR buf;
                    int len = 0;
                    HWND h_input = GetDlgItem(hwnd, IDC_EDIT_INPUT);
                    double value;

                    len = GetWindowTextLength(h_input);
                    if(len <= 0) // if the text box is blank
                    {
                        ErrMsg("Please enter a value to calculate.");
                        break;
                    }

                    buf = (char*)GlobalAlloc(GPTR, len + 1);
                    GetWindowText(h_input, buf, len + 1); // get user input

                    value = atof(buf); // convert value to double
                    calc_value(value, hwnd); // calculate the new value using formula

                    GlobalFree((HANDLE)buf);
                }
                break;

                case IDC_BUTTON_ADD_COMMENT:
                    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_COMMENT), NULL, ComProc);
                    if(bComment == TRUE)
                    {
                        ShowWindow(GetDlgItem(hwnd, IDC_BUTTON_CHECK), true);
                    }
                break;

                case IDC_BUTTON_CHECK:
                    MessageBox(NULL, "Your comment has been loaded.", "Comment", MB_ICONINFORMATION);
                break;

                case IDC_BUTTON_CLEAR:
                    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_NAME), 0);
                    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_INPUT), 0);
                    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_SUBTOTAL), 0);
                    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_TAX), 0);
                    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_TOTAL), 0);
                    if(bComment == TRUE){ GlobalFree((HANDLE)comment); }
                    bComment = FALSE;
                    comment = NULL;
                    ShowWindow(GetDlgItem(hwnd, IDC_BUTTON_CHECK), false);
                break;
            }
        break;

        case WM_CLOSE:
            temp_cleanup();
            EndDialog(hwnd, 0);
        break;

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
