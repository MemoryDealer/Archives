/* game.cpp */
#include <windows.h>
#include <stdlib.h>
#include <ctime>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include "game.h"

extern game_c game;
extern SDL_Surface* screen;
extern int done;
extern bool bPause, bPractice, bSound;

int _imp_ai(void* _null);
int _mov_ball(void* _null);
int serve = 0;
int game_speed = 1;
int pad_speed = 1;
int ai_level = 1;

game_c::game_c(LPCSTR fBall, LPCSTR fPad1, LPCSTR fPad2) // constructor
{
      srand(time(0));

      ball = new ball_t;
      pad1 = new paddle_t;
      pad2 = new paddle_t;

      ball->s = SDL_LoadBMP(fBall);
      pad1->s = SDL_LoadBMP(fPad1);
      pad2->s = SDL_LoadBMP(fPad2);

      ball->x = 21;
      ball->y = 210;
      ball->bX = true;


      pad1->x = 0;
      pad1->y = 180;
      pad2->x = 620;
      pad2->y = 180;

      AI = true; // AI is on by default
}

game_c::~game_c() // destructor
{
    delete ball; ball = 0;
    delete pad1; pad1 = 0;
    delete pad2; pad2 = 0;
}

void game_c::draw_image(SDL_Surface* img, int x, int y)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_BlitSurface(img, NULL, screen, &dst);
}

void game_c::draw_scene()
{
    SDL_FillRect(screen, NULL, 0x000000); // clear entire screen

    draw_image(ball->s, ball->x, ball->y);
    draw_image(pad1->s, pad1->x, pad1->y);
    draw_image(pad2->s, pad2->x, pad2->y);

    SDL_Flip(screen); // update screen buffer
}

void game_c::reset_game(int sound)
{
    pad1->x = 0;
    pad2->x = 620;
    pad1->y = 180;
    pad2->y = 180;
    ball->y = 210;

    int i = rand() % 30 + 1;
    if(i < 15) { ball->bY = true; }
    else       { ball->bY = false; }

    if(serve == 0) // blue serve
    {
        ball->x = 20;
        ball->bX = true;
        serve = 1;
    }
    else if(serve == 1) // red serve
    {
        ball->x = 590;
        ball->bX = false;
        serve = 0;
    }

    if(bPractice)
    {
        pad2->x = 640;
        pad2->y = 480;
    }

    SDL_Delay(1000);
    if(bSound && sound == 1){
        PlaySound("Data\\Sound\\serve.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
}

void game_c::mov_pad(int pad, int c)
{
    if(pad == 0){
        pad1->y += c;
    }
    else if(pad == 1){
        pad2->y += c;
    }
}

void game_c::mov_ballx(int c)
{
    ball->x += c;
}

void game_c::mov_bally(int c)
{
    ball->y += c;
}

int game_c::get_range(paddle_t* pad)
{
    return pad->y + 95;
}

//==================

int _imp_ai(void* _null) // (implement AI thread)
{
    while(done == 0 && game.AI == true)
    {
        if(!bPause && bPractice == false) // make sure this is an active game
        {
            SDL_Delay(1);

            /* prevent paddle from going out of bounds */
            if(game.pad2->y == (ai_level + 2)) { game.pad2->y++; }
            if(game.pad2->y == (385 - ai_level)) { game.pad2->y--; }

            else if(game.ball->x < 320) // if ball is on left half of the screen
            {
                 if(game.ball->bX) // if ball is going right
                 {
                     if(game.ball->bY) // if ball is going down
                     {
                         if(game_speed == 1)
                         {
                             if(game.ball->y < 240) // if ball is in upper half of screen
                             {
                                if(game.ball->y < (game.pad2->y + 48))
                                    game.pad2->y--;
                                else
                                    game.pad2->y++;
                             }
                             else // if ball is in lower half of screen
                             {
                                if(game.ball->y < (game.pad2->y + 48))
                                    game.pad2->y--;
                                else
                                    game.pad2->y++;
                             }
                         }
                         else
                         {
                                game.pad2->y--;
                         }
                     }
                     else   // if ball is going up
                     {
                         if(game_speed == 1)
                         {
                             if(game.ball->y < 240) // if ball is in upper half of screen
                             {
                                if(game.ball->y < (game.pad2->y + 48))
                                    game.pad2->y++;
                                else
                                    game.pad2->y--;
                             }
                             else                   // if ball is in lower half of screen
                             {
                                if(game.ball->y < (game.pad2->y + 48))
                                    game.pad2->y++;
                                else
                                    game.pad2->y--;
                             }
                         }
                         else
                         {
                             game.pad2->y++;
                         }
                     }
                 }
                 else // if ball is going left
                 {
                     if(game.ball->bY) { game.pad2->y--; } // if ball is going down
                     else              { game.pad2->y++; }
                 }
            }
            else if(game.ball->x > 335) // if ball is on right portion of the screen
            {
                if(game.ball->bX) // if ball is moving right
                {
                    /* Move the paddle toward the ball */
                    if((game.pad2->y + 48) < game.ball->y && game.pad2->y <= (385 - ai_level)) { game.pad2->y += ai_level; }
                    else if(game.pad2->y >= (ai_level + 1))                                    { game.pad2->y -= ai_level; }

                }
            }
        }else { SDL_Delay(30); } // sleep to prevent RAM destruction

    }

    return 0;
}

int _mov_ball(void* _null)
{
    char caption[128];
    int blueScore = 0;
    int redScore  = 0;
    bool bFirst = true;
    sprintf(caption, "SparkPong | Blue: %i  Red: %i", blueScore, redScore);
    SDL_WM_SetCaption(caption, NULL);


    while(done == 0)
    {
        if(!bPause) // make sure game is active
        {
            SDL_Delay(1); // slow it to speed
            // check for y axis collisions
            if(game.ball->y == 0 || game.ball->y == 450)
            {
                game.ball->bY = !game.ball->bY;
                if(bSound){
                    PlaySound("Data\\Sound\\hit.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
            }

            // check collision with paddle 1
             // check y pos
            if((game.ball->y > game.pad1->y && game.ball->y < game.get_range(game.pad1))
                || ((game.ball->y + 30) > game.pad1->y && (game.ball->y + 30) < game.get_range(game.pad1)))
            {
                // check x pos
                if(game.ball->x == 20)
                {
                    game.ball->bX = true;
                    if(bSound){
                            PlaySound("Data\\Sound\\hit.wav", NULL, SND_FILENAME | SND_ASYNC);
                        }
                }
                else if((game.ball->x <= 20) && (game.ball->x >= 1)) // if x pos is less than normal hit, generate a reverse collision
                {
                    game.ball->bX = true;
                    if(game.ball->y < (game.pad1->y + 45))
                    {
                        game.ball->bY = false;
                    }
                    else
                    {
                        game.ball->bY = true;
                    }
                    if(bSound){
                            PlaySound("Data\\Sound\\hit.wav", NULL, SND_FILENAME | SND_ASYNC);
                        }
                }
            }

            if(!bPractice) // make sure this in an active game
            {
                // check collisions with paddle 2
                if(game.ball->y > game.pad2->y && game.ball->y < game.get_range(game.pad2)
                    || ((game.ball->y + 30) > game.pad2->y && (game.ball->y + 30) < game.get_range(game.pad2)))
                {
                    if(game.ball->x == 590)
                    {
                        game.ball->bX = false;
                        if(bSound){
                            PlaySound("Data\\Sound\\hit.wav", NULL, SND_FILENAME | SND_ASYNC);
                        }
                    }
                    else if((game.ball->x >= 590) && (game.ball->x <= 619)) // if x pos is greater than normal hit, generate a reverse collision
                    {
                        game.ball->bX = false;
                        if(game.ball->y < (game.pad2->y + 45))
                        {
                            game.ball->bY = false;
                        }
                        else
                        {
                            game.ball->bY = true;
                        }
                        if(bSound){
                            PlaySound("Data\\Sound\\hit.wav", NULL, SND_FILENAME | SND_ASYNC);
                        }
                    }
                }
            }
            else
            {
                /* for practice mode, the entire right wall is bouncable */
                if(game.ball->x == 620)
                {
                    game.ball->bX = false;
                    if(bSound){
                            PlaySound("Data\\Sound\\hit.wav", NULL, SND_FILENAME | SND_ASYNC);
                        }
                }
            }

            /* check for out of bounds */
            if((game.ball->x + 30) < 0)
            {
                if(bSound){
                    PlaySound("Data\\Sound\\out.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                if(!bPractice)
                {
                    redScore++;
                    sprintf(caption, "SparkPong | Blue: %i  Red: %i", blueScore, redScore);
                    SDL_WM_SetCaption(caption, NULL);
                }else { SDL_WM_SetCaption("SparkPong | Practice Mode", NULL); }
                game.reset_game(1);
            }
            if((game.ball->x > 640))
            {
                if(bSound){
                    PlaySound("Data\\Sound\\out.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                if(!bPractice)
                {
                    blueScore++;
                    sprintf(caption, "SparkPong | Blue: %i  Red: %i", blueScore, redScore);
                    SDL_WM_SetCaption(caption, NULL);
                }else { SDL_WM_SetCaption("SparkPong | Practice Mode", NULL); }
                game.reset_game(1);
            }

            /* Move the ball */
            if(bFirst)
            {
                SDL_Delay(1000);
                bFirst = false;
                SetForegroundWindow(FindWindow(NULL, "SparkPong"));
            }
            if(game.ball->bX) { game.mov_ballx(game_speed); }
            else              { game.mov_ballx(-game_speed); }
            if(game.ball->bY) { game.mov_bally(game_speed); }
            else              { game.mov_bally(-game_speed); }
        }else { SDL_Delay(30); }
    }

    return 0;
}


