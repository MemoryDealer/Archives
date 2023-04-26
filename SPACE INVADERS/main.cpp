/* main.cpp */

/* A Space Invaders clone about 70% complete (rush order)
    By Sparky 2010
*/

#include "Game.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

Game game;
int done = 0;

int _UpdateAliens(void* n);
int _Mothership(void* n);

int main(int argc, char** argv)
{
    Uint8* keys;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "ERROR initializing SDL: %s", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    game.screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(game.screen == NULL)
    {
        fprintf(stderr, "ERROR initializing 640x480 display: %s", SDL_GetError());
        return 1;
    }

    game.LoadData();
    SDL_WM_SetCaption("SpaceInvaders", NULL);
    SDL_ShowCursor(0);

    SDL_CreateThread(_UpdateAliens, NULL);
    SDL_CreateThread(_Mothership, NULL);

    while(done == 0)
    {
        while(SDL_PollEvent(&game.event))
        {
            switch(game.event.type)
            {
                case SDL_QUIT:
                    done = 1;
                    break;

                case SDL_KEYDOWN:
                    switch(game.event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            if(game.game_state == STATE_ACTIVE)
                                done = 1;
                            break;

                        case SDLK_SPACE:
                            if(game.game_state == STATE_ACTIVE)
                                game.ShipFire();
                            break;

                        case SDLK_p:
                            if(game.game_state == STATE_PAUSED)
                                game.game_state = STATE_ACTIVE;
                            else
                                game.game_state = STATE_PAUSED;
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        keys = SDL_GetKeyState(NULL);

        if(game.game_state == STATE_ACTIVE)
        {
            if(keys[SDLK_LEFT] && game.Ship.GetX() != 0)
            {
                game.Ship.MoveX(-1);
                //game.Ship.SetFrame(1);
            }
            if(keys[SDLK_RIGHT] && game.Ship.GetX() != (640 - 64))
            {
                game.Ship.MoveX(1);
                //game.Ship.SetFrame(2);
            }

            // check for next level
            if(game.CheckLevel() == 0)
            {
                game.NextLevel();
                SDL_Delay(1000);
            }

            game.DrawScene();
        }
    }

    return 0;
}

int _UpdateAliens(void* n)
{
    int left = 20;
    int right = 260;
    srand(SDL_GetTicks());

    while(done == 0)
    {
        if(game.game_state == STATE_ACTIVE)
        {
            if(game.Alien[0].bX)
            {
                for(int i=0; i<10; ++i){
                    game.Alien[i].MoveX(15);
                    game.Alien2[i].MoveX(15);
                    game.Alien3[i].MoveX(15);
                    game.Alien4[i].MoveX(15);
                }
            }
            else
            {
                for(int i=0; i<10; ++i){
                    game.Alien[i].MoveX(-15);
                    game.Alien2[i].MoveX(-15);
                    game.Alien3[i].MoveX(-15);
                    game.Alien4[i].MoveX(-15);
                }
            }

            // check boundaries
            if(game.Alien[0].GetX() == left)
            {
                game.Alien[0].bX = true;
                for(int i=0; i<10; ++i)
                {
                        game.Alien[i].MoveY(game.GetAlienYFraction());
                        game.Alien2[i].MoveY(game.GetAlienYFraction());
                        game.Alien3[i].MoveY(game.GetAlienYFraction());
                        game.Alien4[i].MoveY(game.GetAlienYFraction());
                }
            }
            else if(game.Alien[0].GetX() == right)
            {
                game.Alien[0].bX = false;
                for(int i=0; i<10; ++i)
                {
                        game.Alien[i].MoveY(game.GetAlienYFraction());
                        game.Alien2[i].MoveY(game.GetAlienYFraction());
                        game.Alien3[i].MoveY(game.GetAlienYFraction());
                        game.Alien4[i].MoveY(game.GetAlienYFraction());
                }
            }

            // randomly drop bombs
            for(int i=0; i<2; ++i)
            {
                if(rand() % 2 == 0)
                {
                    game.DropBomb();
                }
            }

            // calc new speed
            int speed = 1000;
            int c = 0;
            for(int i=0; i<10; ++i)
            {
                if(game.Alien[i].bDraw == false)
                    c++;
                if(game.Alien2[i].bDraw == false)
                    c++;
                if(game.Alien3[i].bDraw == false)
                    c++;
                if(game.Alien4[i].bDraw == false)
                    c++;
            }
            speed = 1000 - (c * game.GetAlienSpeed()); // increase the speed as more aliens die
            SDL_Delay(speed);
        }
    }

    return 0;
}

int _Mothership(void* n)
{
    srand(SDL_GetTicks());

    while(done == 0)
    {
        //int nWait = rand() % 45000 + 30000;
        int nWait = rand() % 10000 + 2000;
        SDL_Delay(nWait);

        if(rand() % 2 == 0)
        {
            game.Mothership.SetX(-64);
            game.bMothershipDir = true;
        }
        else
        {
            game.Mothership.SetX(640);
            game.bMothershipDir = false;

        }

        game.SpawnMothership();
        SDL_Delay(6000);
    }

    return 0;
}
