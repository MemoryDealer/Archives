/* game.h */
#ifndef __GAME_H__
#define __GAME_H__
#define LPCSTR const char*

struct paddle_t
{
    int x;
    int y;
    int range;
    bool bY;
    SDL_Surface* s;
};

struct ball_t
{
    int x;
    int y;
    bool bX;
    bool bY;
    SDL_Surface* s;
};

class game_c
{
    public:
        game_c(LPCSTR fBall, LPCSTR fPad1, LPCSTR fPad2);
        ~game_c();

        // game functions
        void draw_image(SDL_Surface* img, int x, int y);
        void draw_scene();
        void reset_game(int sound);
        void mov_pad(int pad, int c);
        void mov_ballx(int c);
        void mov_bally(int c);
        int  get_range(paddle_t* pad);

        // game variables
        ball_t*   ball;
        paddle_t* pad1;
        paddle_t* pad2;
        bool AI;
        SDL_Thread* tAI;
};

#endif
