#include "bitmapobject.h"

BitMapObject::BitMapObject() // constructor
{
    hdcMem  = NULL;
    hbmNew  = NULL;
    hbmOld  = NULL;
    iWidth  = 0;
    iHeight = 0;
}

BitMapObject::~BitMapObject() // destructor
{
    if(hdcMem)
        destroy();
}

void BitMapObject::load(HDC hdcCompatible, LPCTSTR lpszFileName)
{
    if(hdcMem)
        destroy();

    // create memory dc
    hdcMem = CreateCompatibleDC(hdcCompatible);
    // load the bitmap
    hbmNew = (HBITMAP)LoadImage(NULL, lpszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    // shove the image into the dc
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmNew);
    // get BMP's properties
    BITMAP bmp;
    GetObject(hbmNew, sizeof(BITMAP), (LPVOID)&bmp);
    // grab width and height
    iWidth  = bmp.bmWidth;
    iHeight = bmp.bmHeight;
}

void BitMapObject::create(HDC hdcCompatible, int width, int height)
{
    if(hdcMem)
        destroy();

    // create memory dc
    hdcMem = CreateCompatibleDC(hdcCompatible);
    // create the BMP
    hbmNew = CreateCompatibleBitmap(hdcCompatible, width, height);
    // shove the image into the dc
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmNew);
    // change width and height
    iWidth = width;
    iHeight = height;
}

void BitMapObject::destroy()
{
    // restore old bmp
    SelectObject(hdcMem, hbmOld);
    // delete new bmp
    DeleteObject(hbmNew);
    // delete dc
    DeleteDC(hdcMem);
    hdcMem  = NULL;
    hbmNew  = NULL;
    hbmOld  = NULL;
    iWidth  = 0;
    iHeight = 0;
}

BitMapObject::operator HDC()
{
    return hdcMem;
}

int BitMapObject::get_width()
{
    return iWidth;
}

int BitMapObject::get_height()
{
    return iHeight;
}

