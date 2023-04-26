#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int change_icon(const char* src, const char* dst)
{
    HRSRC hRes, hResLoad;
    HANDLE hUpdateRes;
    HMODULE hSrcExe, hDstExe;
    char* lpResLock;
    BOOL bResult;
    int i;

    hSrcExe = LoadLibrary(src);
    if(hSrcExe == NULL)
        return (int)GetLastError();

    // find the icon resource
    for(i=1;;++i)
    {
        char str[256] = {0};

        wsprintf(str, "#%d", i);
        hRes = FindResource(hSrcExe, str, RT_ICON);
        if(hRes == NULL)
            continue;
        else if(i == 10)
            return 2;
        else
            break;
    }

    // load icon
    hResLoad = (HRSRC)LoadResource(hSrcExe, hRes);
    if(hResLoad == NULL)
        return 3;

    // lock the icon
    lpResLock = LockResource(hResLoad);
    if(lpResLock == NULL)
        return 4;

    hDstExe = LoadLibrary(dst);
    if(hDstExe == NULL)
        return 5;

    for(i=1;;++i)
    {
        char str[256] = {0};

        wsprintf(str, "#%d", i);
        if(FindResource(hDstExe, str, RT_ICON) == NULL)
            continue;
        else if(i == 10)
            return 6;
        else
            break;
    }

    FreeLibrary(hDstExe);

    // add the icon resource
    hUpdateRes = BeginUpdateResource(dst, FALSE);
    if(hUpdateRes == NULL)
        return 7;

    bResult = UpdateResource(hUpdateRes,
                            RT_ICON,
                            MAKEINTRESOURCE(1),
                            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                            lpResLock,
                            SizeofResource(hSrcExe, hRes));

    if(!bResult)
        return 8;

    if(!EndUpdateResource(hUpdateRes, FALSE))
        return 9;

    return 0;
}

int main()
{
    char src_file[MAX_PATH] = {0};
    char dst_file[MAX_PATH] = {0};
    int x;

    printf("Enter source file:");
    fgets(src_file, sizeof(src_file), stdin);

    printf("\nEnter destination file:");
    fgets(dst_file, sizeof(dst_file), stdin);

    if((x = change_icon(src_file, dst_file)) == 0)
        printf("Change successful.\n");
    else
        printf("Change failed: %d.\n", x);

    return 0;
}
