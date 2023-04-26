/* Sprite.cpp */

#include "Sprite.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

void SpriteBase::Init(const char* dir)
{
    char buf[255];
    char filename[255];
    char name[255];
    int pause = 0, r = 0, g = 0, b = 0;
    FILE* fp;

    sprintf(filename, "%s\\info", dir);

    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr, "ERROR opening file %s", filename);
        ErrMsg("0");
        exit(1);
    }

    fgets(buf, 255, fp); // grab one line
    sscanf(buf, "FILES: %d", &m_NumFrames); // get number of frames
    m_Anim = new SpriteFrame_t[m_NumFrames];

    m_Built = true;
    int count = 0;

    while(!feof(fp) && count < m_NumFrames)
    {
        fgets(buf, 255, fp);
        if(buf[0] != '#' && buf[0] != '\r' && buf[0] != '\n'
            && buf[0] != '\0' && strlen(buf) != 0)
        {
            sscanf(buf, "%s %d %d %d %d", name, &pause, &r, &g, &b);
            sprintf(filename, "%s\\%s", dir, name);

            SDL_Surface* temp = NULL;
            temp = SDL_LoadBMP(filename);
            if(temp == NULL)
            {
                char buf[256];
                sprintf(buf, "ERROR loading file: %s", filename);
                fprintf(stderr, "ERROR loading file %s", filename);
                ErrMsg(buf);
                exit(1);
            }

            if(r >= 0) SDL_SetColorKey(temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, r, g, b));

            m_Anim[count].image = SDL_DisplayFormat(temp);
            SDL_FreeSurface(temp);

            m_Anim[count].pause = pause;
            if(!m_W) m_W = m_Anim[count].image->w;
            if(!m_H) m_H = m_Anim[count].image->h;

            ++count;
        }
    }

    fclose(fp);
}

//=============

void Sprite::Init(SpriteBase* base, SDL_Surface* screen)
{
    m_SpriteBase = base;

    if(m_SpriteBase->m_Built)
    {
        if(m_SpriteBase->m_NumFrames > 1) m_Animating = true;

        m_BGReplacement = SDL_DisplayFormat(m_SpriteBase->m_Anim[0].image);
    }

    m_Screen = screen;
    bX = true;
    bDraw = false;
}

void Sprite::ClearBG()
{
    if(m_Drawn)
    {
        SDL_Rect dst;
        dst.x = m_OldX;
        dst.y = m_OldY;
        dst.w = m_SpriteBase->m_W;
        dst.h = m_SpriteBase->m_H;

        SDL_BlitSurface(m_BGReplacement, NULL, m_Screen, &dst);
    }
}

void Sprite::UpdateBG()
{
    SDL_Rect src;
    src.x = m_X;
    src.y = m_Y;
    src.w = m_SpriteBase->m_W;
    src.h = m_SpriteBase->m_H;

    SDL_BlitSurface(m_Screen, &src, m_BGReplacement, NULL);
}

void Sprite::Draw()
{
    if(m_Animating)
    {
        if(m_LastUpdate + (m_SpriteBase->m_Anim[m_Frame].pause * m_Speed) < SDL_GetTicks())
        {
            ++m_Frame;
            if(m_Frame > m_SpriteBase->m_NumFrames - 1) m_Frame = 0;
            m_LastUpdate = SDL_GetTicks();
        }
    }

    if(!m_Drawn) m_Drawn = true;

    SDL_Rect dst;
    dst.x = m_X;
    dst.y = m_Y;

    SDL_BlitSurface(m_SpriteBase->m_Anim[m_Frame].image, NULL, m_Screen, &dst);
}

