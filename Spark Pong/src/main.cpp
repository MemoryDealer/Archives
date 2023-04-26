/*
 * SparkPong by Sparky (Jordan Sparks) 2010
 *      csparky@live.com
 * You may freely use this code
 */

/* main.cpp */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include "game.h"
#include "resource.h"

extern int _imp_ai(void* _null);
extern int _mov_ball(void* _null);
extern int game_speed, ai_level, pad_speed;

game_c game("Data\\Objects\\ball.bmp", "Data\\Objects\\pad1.bmp", "Data\\Objects\\pad2.bmp");
SDL_Surface* screen     = NULL;
int done                = 0;
bool bPause             = false;
bool bPractice          = false;
bool bSound             = true;

HWND hGame      = NULL;
bool bOnePlayer = true,
     bTwoPlayer = false,
     bSpeed1    = true,
     bSpeed2    = false,
     bSpeed3    = false,
     bAI1       = true,
     bAI2       = false,
     bAI3       = false;

/* Dialog window procedure: */
BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hOnePlayer = GetDlgItem(hwnd, IDC_RDO_1PLAYER),
         hTwoPlayer = GetDlgItem(hwnd, IDC_RDO_2PLAYER),
         hSpeed1    = GetDlgItem(hwnd, IDC_RDO_SPEED1),
         hSpeed2    = GetDlgItem(hwnd, IDC_RDO_SPEED2),
         hSpeed3    = GetDlgItem(hwnd, IDC_RDO_SPEED3),
         hAI1       = GetDlgItem(hwnd, IDC_RDO_AI1),
         hAI2       = GetDlgItem(hwnd, IDC_RDO_AI2),
         hAI3       = GetDlgItem(hwnd, IDC_RDO_AI3),
         hSound     = GetDlgItem(hwnd, IDC_CHK_SOUND),
         hEditSpeed = GetDlgItem(hwnd, IDC_EDT_SPEED);

    switch(msg)
    {
        case WM_INITDIALOG:
            /* Place this window under the game window */
            //SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 252, 350, SWP_NOMOVE);

            /* set checkboxes */
            SendMessage(hOnePlayer, BM_SETCHECK, 1, 0);
            SendMessage(hSpeed1, BM_SETCHECK, 1, 0);
            SendMessage(hAI1, BM_SETCHECK, 1, 0);
            SendMessage(hSound, BM_SETCHECK, 1, 0);

            SetWindowText(hEditSpeed, "1");
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                /* Control each radio button group */
                case IDC_RDO_1PLAYER:
                    if(!bOnePlayer){
                        SendMessage(hOnePlayer, BM_SETCHECK, 1, 0);
                        SendMessage(hTwoPlayer, BM_SETCHECK, 0, 0);
                        bOnePlayer = true;
                        bTwoPlayer = false;
                        game.AI = true;

                        SDL_KillThread(game.tAI);
                        game.tAI = SDL_CreateThread(_imp_ai, NULL);
                    }
                    SetForegroundWindow(hGame);
                    break;

                case IDC_RDO_2PLAYER:
                    if(!bTwoPlayer){
                        SendMessage(hTwoPlayer, BM_SETCHECK, 1, 0);
                        SendMessage(hOnePlayer, BM_SETCHECK, 0, 0);
                        bTwoPlayer = true;
                        bOnePlayer = false;
                        game.AI = false;
                    }
                    SetForegroundWindow(hGame);
                    break;

                //---------------------------------------------------

                case IDC_RDO_SPEED1:
                    if(!bSpeed1){
                        SendMessage(hSpeed1, BM_SETCHECK, 1, 0);
                        SendMessage(hSpeed2, BM_SETCHECK, 0, 0);
                        SendMessage(hSpeed3, BM_SETCHECK, 0, 0);
                        bSpeed1 = true;
                        bSpeed2 = false;
                        bSpeed3 = false;
                        game_speed = 1;
                        SetForegroundWindow(hGame);
                        game.reset_game(0);
                    }
                    SetForegroundWindow(hGame);
                    break;

                case IDC_RDO_SPEED2:
                    if(!bSpeed2){
                        SendMessage(hSpeed2, BM_SETCHECK, 1, 0);
                        SendMessage(hSpeed1, BM_SETCHECK, 0, 0);
                        SendMessage(hSpeed3, BM_SETCHECK, 0, 0);
                        bSpeed2 = true;
                        bSpeed1 = false;
                        bSpeed3 = false;
                        game_speed = 2;
                        SetForegroundWindow(hGame);
                        game.reset_game(0);
                    }
                    SetForegroundWindow(hGame);
                    break;

                case IDC_RDO_SPEED3:
                    if(!bSpeed3){
                        SendMessage(hSpeed3, BM_SETCHECK, 1, 0);
                        SendMessage(hSpeed1, BM_SETCHECK, 0, 0);
                        SendMessage(hSpeed2, BM_SETCHECK, 0, 0);
                        bSpeed3 = true;
                        bSpeed1 = false;
                        bSpeed2 = false;
                        game_speed = 3;
                        SetForegroundWindow(hGame);
                        game.reset_game(0);
                    }
                    SetForegroundWindow(hGame);
                    break;

                //---------------------------------------------------

                case IDC_RDO_AI1:
                    if(!bAI1){
                        SendMessage(hAI1, BM_SETCHECK, 1, 0);
                        SendMessage(hAI2, BM_SETCHECK, 0, 0);
                        SendMessage(hAI3, BM_SETCHECK, 0, 0);
                        bAI1 = true;
                        bAI2 = false;
                        bAI3 = false;
                        ai_level = 1;
                    }
                    SetForegroundWindow(hGame);
                    break;

                case IDC_RDO_AI2:
                    if(!bAI2){
                        SendMessage(hAI2, BM_SETCHECK, 1, 0);
                        SendMessage(hAI1, BM_SETCHECK, 0, 0);
                        SendMessage(hAI3, BM_SETCHECK, 0, 0);
                        bAI2 = true;
                        bAI1 = false;
                        bAI3 = false;
                        ai_level = 2;
                    }
                    SetForegroundWindow(hGame);
                    break;

                case IDC_RDO_AI3:
                    if(!bAI3){
                        SendMessage(hAI3, BM_SETCHECK, 1, 0);
                        SendMessage(hAI1, BM_SETCHECK, 0, 0);
                        SendMessage(hAI2, BM_SETCHECK, 0, 0);
                        bAI3 = true;
                        bAI1 = false;
                        bAI2 = false;
                        ai_level = 3;
                    }
                    if(bSound){
                        PlaySound("Data\\Sound\\lion.wav", NULL, SND_FILENAME | SND_ASYNC);
                    }
                    SetForegroundWindow(hGame);
                    break;

                //---------------------------------------------------

                case IDC_CHK_SOUND:
                    if(bSound){
                        SendMessage(hSound, BM_SETCHECK, 0, 0);
                        bSound = false;
                    }
                    else{
                        SendMessage(hSound, BM_SETCHECK, 1, 0);
                        bSound = true;
                    }
                    SetForegroundWindow(hGame);
                    break;

                //---------------------------------------------------

                case IDC_BTN_PLUS:
                    {
                        char buf[32];
                        int temp = 0;

                        GetWindowText(hEditSpeed, buf, 32);
                        temp = atoi(buf);
                        if(temp == 7){ // max
                            SetForegroundWindow(hGame);
                            break;
                        }

                        sprintf(buf, "%i", temp + 1);
                        SetWindowText(hEditSpeed, buf);
                        pad_speed = temp + 1;
                        SetForegroundWindow(hGame);
                    }
                    break;

                case IDC_BTN_MINUS:
                    {
                        char buf[32];
                        int temp = 0;

                        GetWindowText(hEditSpeed, buf, 32);
                        temp = atoi(buf);
                        if(temp == 1){ // min
                            SetForegroundWindow(hGame);
                            break;
                        }

                        sprintf(buf, "%i", temp - 1);
                        SetWindowText(hEditSpeed, buf);
                        pad_speed = temp - 1;
                        SetForegroundWindow(hGame);
                    }
                    break;

            }
            return TRUE;

        default:
            return FALSE;
    }

    return TRUE;
}

DWORD WINAPI _dlg_thread(LPVOID _null)
{
    // this thread is necessary to create this dialog and continue with the game
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPTIONS), NULL, DlgProc);

    return 0;
}

int read_cnf()
{
    FILE* fp;
    char buffer[255];
    int mem;

    fp = fopen("Data\\config", "r");
    if(fp == NULL)
    {
        printf("ERROR opening file \"Data\\config\"");
        return -1;
    }

    while(!feof(fp))
    {
        fgets(buffer, 255, fp);

        if(buffer[0] != '#' && buffer[0] != '\r' && buffer[0] != '\0'
            && buffer[0] != '\n' && strlen(buffer) != 0)
        {
                sscanf(buffer, "MEMLOAD %i", &mem);
        }
    }

    if(mem == 1)
        return 1;
    else if(mem == 2)
        return 2;
    else
        return 666;
}

int _eff(void* _null)
{
    SetForegroundWindow(hGame);

    return 0;
}

void main_menu()
{
    PlaySound("Data\\Sound\\hit.wav", NULL, SND_FILENAME | SND_ASYNC);
    SDL_Surface* title = NULL;
    SDL_Surface* text1 = NULL;
    SDL_Surface* text2 = NULL;
    SDL_Thread*  eff   = NULL;

    title = SDL_LoadBMP("Data\\Menu\\title.bmp");
    text1 = SDL_LoadBMP("Data\\Menu\\text1.bmp");
    text2 = SDL_LoadBMP("Data\\Menu\\text2.bmp");

    game.draw_image(title, 48, 30);
    game.draw_image(text1, 33, 210);
    game.draw_image(text2, 57, 320);

    eff = SDL_CreateThread(_eff, NULL);
    int start = 0;
    while(start == 0)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    SDL_Quit();
                    exit(0);
                    break;

                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_RETURN){
                        start = 1;
                    }
                    if(event.key.keysym.sym == SDLK_ESCAPE){
                        SDL_Quit();
                        exit(0);
                    }
                    break;
            }
        }
    }

    SDL_KillThread(eff);
    delete title; title = 0;
    delete text1; text1 = 0;
    delete text2; text2 = 0;
}

int main(int argc, char* argv[])
{
    Uint8* keys = NULL;
    srand(GetTickCount()); // seed random number generator

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error: %s", SDL_GetError());
        return -1;
    }

    if(read_cnf() == 2){
        screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    }
    else{
        screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
    }

    if(screen == NULL){
        printf("Error: %s", SDL_GetError());
        return -1;
    }
    SDL_WM_SetCaption("SparkPong", NULL);
    SDL_ShowCursor(0);

    int f = 0;
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&_dlg_thread, 0, 0, 0); // must use thread to continue w/ game
    hGame = FindWindow(NULL, "SparkPong"); // retrieve the main game window
    SendMessage(hGame, WM_SETICON, ICON_SMALL,
             (LPARAM)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));

    main_menu(); // wait for user to enter the game

    SDL_CreateThread(_mov_ball, NULL);
    if(game.AI)
    {
          game.tAI = SDL_CreateThread(_imp_ai, NULL);
    }
    while(done == 0)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    done = 1;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            done = 1;
                            break;

                        case SDLK_SPACE:
                            bPause = !bPause; // toggle pause
                            break;

                        case SDLK_F6:
                            bPractice = !bPractice; // toggle practice mode
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

        if(!bPause)
        {
            if(keys[SDLK_a]){ // move blue pad up
                if(game.pad1->y > 0){
                    /* This will allow a "1/2" speed at the "1" speed, the rest are one less than what they actually are */
                    game.mov_pad(0, (pad_speed == 2) ? -1 : (pad_speed == 1) ? -1 : -(pad_speed - 1));
                }
                if(pad_speed == 1 && f >= 2){
                    game.mov_pad(0, 1);
                }
            }
            if(keys[SDLK_z]){ // move blue pad down
                if(game.pad1->y < 385){
                    game.mov_pad(0, (pad_speed == 2) ? 1 : (pad_speed == 1) ? 1 : (pad_speed - 1));
                }
                if(pad_speed == 1 && f >= 2){
                    game.mov_pad(0, -1);
                }
            }
            if(keys[SDLK_UP] && game.AI == false){ // move red pad up
                if(game.pad2->y > 0){
                    game.mov_pad(1, (pad_speed == 2) ? -1 : (pad_speed == 1) ? -1 : -(pad_speed - 1));
                }
                if(pad_speed == 1 && f >= 2){
                    game.mov_pad(1, 1);
                }
            }
            if(keys[SDLK_DOWN] && game.AI == false){ // move red pad down
                if(game.pad2->y < 385){
                    game.mov_pad(1, (pad_speed == 2) ? 1 : (pad_speed == 1) ? 1 : (pad_speed - 1));
                }
                if(pad_speed == 1 && f >= 2){
                    game.mov_pad(1, -1);
                }
            }

            if(f >= 2){
                f = 0;
            }
            ++f;

            game.draw_scene();
        }
    }

    EndDialog(FindWindow(NULL, "SparkPong Options"), 0);
    SDL_Quit();
    return 0;
}
