#ifndef BITMAPOBJECT_H
#define BITMAPOBJECT_H
#pragma once

#include <windows.h>

class BitMapObject
{
    private:
        HDC         hdcMem;
        HBITMAP     hbmNew;
        HBITMAP     hbmOld;
        int         iWidth;
        int         iHeight;

    public:
        BitMapObject();
        ~BitMapObject();
        void load(HDC hdcCompatible, LPCTSTR lpszFileName); // loads BMP from file
        void create(HDC hdcCompatible, int width, int height); // creates a BMP
        void destroy(); // destroys BMP
        int get_width(); // returns width
        int get_height(); // returns height

        operator HDC(); // converts to HDC
};

inline int ErrCheck()
{
    return MessageBox(NULL, "ErrCheck", 0, 0);
}

#endif


