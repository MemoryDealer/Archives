/* Sprite.h */

#ifndef SPRITE_H
#define SPRITE_H

#define WIN32_LEAN_AND_MEAN
#include <SDL/SDL.h>
#include <windows.h>

inline int ErrMsg(const char* msg)
{
    return MessageBox(GetForegroundWindow(), msg, 0, MB_ICONERROR);
}

struct SpriteFrame_t
{
    SDL_Surface* image;
    int pause;
};

class SpriteBase
{
    public:
        void Init(const char* dir);

        SpriteFrame_t* m_Anim;
        int m_Built, m_NumFrames, m_W, m_H;
};

class Sprite
{
    public:
        void Init(SpriteBase* base, SDL_Surface* screen);

        void ClearBG();
        void UpdateBG();
        void Draw();

        void SetFrame(int c)            { m_Frame = c; }
        int GetFrame()                  { return m_Frame; }
        void SetSpeed(int c)            { m_Speed = c; }
        int GetSpeed()                  { return m_Speed; }
        void ToggleAnim()               { m_Animating = !m_Animating; }
        void StartAnim()                { m_Animating = true; }
        void StopAnim()                 { m_Animating = false; }
        void Rewind()                   { m_Frame = 0; }

        void MoveX(int c)               { m_X += c; }
        void MoveY(int c)               { m_Y += c; }
        void SetX(int c)                { m_X = c; }
        void SetY(int c)                { m_Y = c; }
        void Set(int x, int y)          { m_X = x; m_Y = y; }
        int GetX()                      { return m_X; }
        int GetY()                      { return m_Y; }

        bool bX;
        bool bDraw;
        int life;
        int lives;

    private:
        int m_Frame;
        int m_X, m_Y, m_OldX, m_OldY;
        int m_Speed;
        long m_LastUpdate;

        bool m_Animating;
        bool m_Drawn;

        SpriteBase* m_SpriteBase;
        SDL_Surface* m_BGReplacement;
        SDL_Surface* m_Screen;
};

#endif

