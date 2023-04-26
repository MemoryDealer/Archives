// Tetris
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdlib>
#include "bitmapobject.h"

using namespace std;

#define WNDCLASS "Tetris"
#define WNDTITLE "Tetris"

const int TILESIZE      = 16;
const int MAPWIDTH      = 10;
const int MAPHEIGHT     = 30;
const int GREY          = 8;

// enumerate color values
enum{TILENODRAW = -1, TILEBLACK = 0, TILEGREY = 1, TILEBLUE = 2, TILERED = 3, TILEGREEN = 4,
     TILEYELLOW = 5, TILEWHITE = 6, TILESTEEL = 7, TILEPURPLE = 8};

/* Function Prototypes */
bool game_init();
void game_loop();
void game_done();
void draw_tile(int x, int y, int tile);
void draw_map();
void new_block();
void rotate_block();
void mov(int x, int y);
int collision_test(int nx, int ny);
void remove_row(int row);
void new_game();

HINSTANCE hInst = NULL;
HWND hWnd = NULL;

int map[MAPWIDTH][MAPHEIGHT+1]; // the game map

typedef struct{
    int size[4][4];
    int x;
    int y;
} Piece;

Piece prePiece; // previous piece
Piece piece; // piece

DWORD dwStartTime; // used in timing
bool bGameStarted = false;

// map for game
BitMapObject bmoMap;
// block images
BitMapObject bmoBlocks;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_KEYDOWN:
            {
                switch(wParam)
                {
                    case VK_ESCAPE:
                        DestroyWindow(hWnd);
                        break;

                    case VK_DOWN:
                        mov(0, 1);
                        break;

                    case VK_UP:
                        rotate_block();
                        break;

                    case VK_RIGHT:
                        mov(1, 0);
                        break;

                    case VK_LEFT:
                        mov(-1, 0);
                        break;
                }
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            BitBlt(hdc, 0, 0, TILESIZE * MAPWIDTH + TILESIZE * GREY, TILESIZE * MAPHEIGHT, bmoMap, 0, 0, SRCCOPY);
            EndPaint(hwnd, &ps);
        } return 0;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        LPSTR lpszArgument, int nFunsterStil)
{
    hInst = hInstance;
    WNDCLASSEX wcx;
        wcx.cbSize          = sizeof(WNDCLASSEX);
        wcx.style           = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wcx.lpfnWndProc     = WndProc;
        wcx.lpszClassName   = WNDCLASS;
        wcx.cbClsExtra      = 0;
        wcx.cbWndExtra      = 0;
        wcx.hInstance       = hInstance;
        wcx.hIcon           = LoadIcon(NULL, IDI_APPLICATION);
        wcx.hCursor         = LoadCursor(NULL, IDC_ARROW);
        wcx.hbrBackground   = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wcx.lpszMenuName    = NULL;
        wcx.hIconSm         = NULL;

    if(!RegisterClassEx(&wcx)) { return 0; }

    hWnd = CreateWindowEx(0,
                          WNDCLASS,
                          WNDTITLE,
                          WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          520,
                          240,
                          NULL,
                          NULL,
                          hInst,
                          NULL);

    if(!hWnd) { return 0; }

    if(!game_init()) { return 0; }

    MSG msg;

    for(;;)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT) break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        game_loop();
    }

    game_done();

    return msg.wParam;
}

//------Game Functions------

bool game_init()
{
    RECT rc; // temp rect
    SetRect(&rc, 0, 0, MAPWIDTH * TILESIZE + TILESIZE * GREY, MAPHEIGHT * TILESIZE);
    AdjustWindowRect(&rc, WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE, FALSE);
    SetWindowPos(hWnd, NULL, 0, 0, rc.right = rc.left, rc.bottom - rc.top, SWP_NOMOVE);

    // create map image
    HDC hdc = GetDC(hWnd);
    bmoMap.create(hdc, MAPWIDTH * TILESIZE + TILESIZE * GREY, MAPHEIGHT * TILESIZE);
    FillRect(bmoMap, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
    ReleaseDC(hWnd, hdc);

    bmoBlocks.load(NULL, "blocks.bmp");
    new_game();

    return true;
}

void game_done()
{
    // clean up code:
}

void game_loop()
{
    // lock at 1 FPS
    if((GetTickCount() - dwStartTime) > 1000)
    {
        mov(0, 1);
        dwStartTime = GetTickCount();
    }
}

void new_game()
{
    dwStartTime = GetTickCount();
    bGameStarted = false;

    // start out the map
    for(int x=0; x < MAPWIDTH; ++x)
    {
        for(int y=0; y < MAPHEIGHT; ++y)
        {
            if(y == MAPHEIGHT)
                map[x][y] = TILEGREY;
            else
                map[x][y] = TILEBLACK;
        }
    }
    new_block();
    draw_map();
}

void draw_tile(int x, int y, int tile)
{
    // mask first
    BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoBlocks, tile * TILESIZE, TILESIZE, SRCAND);
    // then image
    BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoBlocks, tile * TILESIZE, 0, SRCPAINT);
}

void draw_map()
{
    int xmy, ymx;

    // place toolbar
    for(xmy = MAPWIDTH; xmy < MAPWIDTH + GREY; xmy++)
    {
        for(ymx = 0; ymx < MAPHEIGHT; ymx++)
        {
            draw_tile(xmy, ymx, TILEGREY);
        }
    }

    // draw preview block
    for(xmy = 0; xmy < 4; xmy++)
    {
        for(ymx = 0; ymx < 4; ymx++)
        {
            if(prePiece.size[xmy][ymx] != TILENODRAW){
                draw_tile(prePiece.x + xmy, prePiece.y + ymx, prePiece.size[xmy][ymx]);
            }
        }
    }

    // draw the map
    for(xmy = 0; xmy < MAPWIDTH; xmy++)
    {
        for(ymx = 0; ymx < MAPHEIGHT; ymx++)
        {
            draw_tile(xmy, ymx, map[xmy][ymx]);
        }
    }

    // draw moving block
    for(xmy = 0; xmy < 4; xmy++)
    {
        for(ymx = 0; ymx < 4; ymx++)
        {
            if(piece.size[xmy][ymx] != TILENODRAW){
                draw_tile(piece.x + xmy, piece.y + ymx, piece.size[xmy][ymx]);
            }
        }
    }

    InvalidateRect(hWnd, NULL, FALSE);
}

void new_block()
{
    int newblock;
    int i, j;
    //  0   1   2   3   4    5   6
	//   X                             These
	//   X   XX   X  XX   XX  XX   XX  are
	//   X   XX  XXX  XX XX    X   X   block
	//   X                     X   X   types

    srand(GetTickCount());

    // initialize pieces to all blank
    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            piece.size[i][j] = TILENODRAW;
        }
    }

    piece.x = MAPWIDTH / 2 - 2;
    piece.y = -1;

    if(bGameStarted == false)
    {
        bGameStarted = true;

        newblock = rand() % 7;

        switch(newblock)
        {
            case 0: // tower
                piece.size[1][0] = TILERED;
                piece.size[1][1] = TILERED;
                piece.size[1][2] = TILERED;
                piece.size[1][3] = TILERED;
                piece.y          = 0;
                break;

            case 1: // box
                piece.size[1][1] = TILEBLUE;
                piece.size[1][2] = TILEBLUE;
                piece.size[2][1] = TILEBLUE;
                piece.size[2][2] = TILEBLUE;
                break;

            case 2: // pyramid
                piece.size[1][1] = TILESTEEL;
                piece.size[0][2] = TILESTEEL;
                piece.size[1][2] = TILESTEEL;
                piece.size[2][2] = TILESTEEL;
                break;

            case 3: // left leaner
                piece.size[0][1] = TILEYELLOW;
                piece.size[1][1] = TILEYELLOW;
                piece.size[1][2] = TILEYELLOW;
                piece.size[2][2] = TILEYELLOW;
                break;

            case 4: // right leaner
                piece.size[2][1] = TILEGREEN;
                piece.size[1][1] = TILEGREEN;
                piece.size[1][2] = TILEGREEN;
                piece.size[0][2] = TILEGREEN;
                break;

            case 5: // left knight
                piece.size[1][1] = TILEWHITE;
                piece.size[2][1] = TILEWHITE;
                piece.size[2][2] = TILEWHITE;
                piece.size[2][3] = TILEWHITE;
                break;

            case 6: // right knight
                piece.size[2][1] = TILEPURPLE;
                piece.size[1][1] = TILEPURPLE;
                piece.size[1][2] = TILEPURPLE;
                piece.size[1][3] = TILEPURPLE;
                break;
        }
    }
    else
    {
        for(i = 0; i < 4; ++i)
        {
            for(j = 0; j < 4; ++j)
            {
                piece.size[i][j] = prePiece.size[i][j];
            }
        }
    }

    newblock = rand() % 7;

    for(i = 0; i < 4; ++i)
        for(j = 0; j < 4; ++j)
            prePiece.size[i][j] = TILENODRAW;

    prePiece.x = MAPWIDTH + GREY / 4;
    prePiece.y = GREY / 4;

    switch(newblock)
    {
            case 0: // tower
                piece.size[1][0] = TILERED;
                piece.size[1][1] = TILERED;
                piece.size[1][2] = TILERED;
                piece.size[1][3] = TILERED;
                piece.y          = 0;
                break;

            case 1: // box
                piece.size[1][1] = TILEBLUE;
                piece.size[1][2] = TILEBLUE;
                piece.size[2][1] = TILEBLUE;
                piece.size[2][2] = TILEBLUE;
                break;

            case 2: // pyramid
                piece.size[1][1] = TILESTEEL;
                piece.size[0][2] = TILESTEEL;
                piece.size[1][2] = TILESTEEL;
                piece.size[2][2] = TILESTEEL;
                break;

            case 3: // left leaner
                piece.size[0][1] = TILEYELLOW;
                piece.size[1][1] = TILEYELLOW;
                piece.size[1][2] = TILEYELLOW;
                piece.size[2][2] = TILEYELLOW;
                break;

            case 4: // right leaner
                piece.size[2][1] = TILEGREEN;
                piece.size[1][1] = TILEGREEN;
                piece.size[1][2] = TILEGREEN;
                piece.size[0][2] = TILEGREEN;
                break;

            case 5: // left knight
                piece.size[1][1] = TILEWHITE;
                piece.size[2][1] = TILEWHITE;
                piece.size[2][2] = TILEWHITE;
                piece.size[2][3] = TILEWHITE;
                break;

            case 6: // right knight
                piece.size[2][1] = TILEPURPLE;
                piece.size[1][1] = TILEPURPLE;
                piece.size[1][2] = TILEPURPLE;
                piece.size[1][3] = TILEPURPLE;
                break;
    }

    draw_map();
}

void rotate_block()
{
    int i, j, temp[4][4];

    // copy and rotate to temp array
    for(i = 0; i < 4; ++i)
        for(j = 0; j < 4; ++j)
            temp[3 - j][i] = piece.size[i][j];

    // check collision of temp array
    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            if(temp[i][j] != TILENODRAW){
                if(piece.x + i < 0 || piece.x + i > MAPWIDTH - 1 || piece.y + j < 0 || piece.y + j > MAPHEIGHT - 1){
                    return;
                }
            }
        }
    }

    // check for collision with blocks
    for(int x = 0; x < MAPWIDTH; ++x)
    {
        for(int y = 0; y < MAPHEIGHT; ++y)
        {
            if(x >= piece.x && x < piece.x + 4)
                if(y >= piece.y && y < piece.y + 4)
                    if(map[x][y] != TILEBLACK)
                        if(temp[x - piece.x][y - piece.y] != TILENODRAW)
                            return;
        }
    }

    /* collision check done */
    for(i = 0; i < 4; ++i)
        for(j = 0; j < 4; ++j)
            piece.size[i][j] = temp[i][j];

    draw_map();

    return;
}

void mov(int x, int y)
{
    if(collision_test(x, y))
    {
        if(y == 1)
        {
            if(piece.y < 1){
                new_game();
            }else{
                bool bKillBlock = false;
                int i, j;
                for(i = 0; i < 4; ++i){
                    for(j = 0; j < 4; ++j){
                        if(piece.size[i][j] != TILENODRAW){
                            map[piece.x + i][piece.y + j] = piece.size[i][j];
                        }
                    }
                }

            // check for cleared row
            for(j = 0; j < MAPHEIGHT; ++j)
            {
                bool bFilled = true;
                for(i = 0; i < MAPWIDTH; ++i)
                {
                    if(map[i][j] == TILEBLACK){
                        bFilled = false;
                    }

                    if(bFilled){
                        remove_row(j);
                        bKillBlock = true;
                    }
                }
            }

            if(bKillBlock)
            {
                for(i = 0; i < 4; ++i)
                    for(j = 0; j < 4; ++j)
                        piece.size[i][j] = TILENODRAW;


            }
            new_block();
                }
        }
    }
    else
    {
        piece.x += x;
        piece.y += y;
    }

    draw_map();
}

int collision_test(int nx, int ny)
{
    int newx = piece.x + nx;
    int newy = piece.y + ny;

    int i, j, x, y;

    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            if(piece.size[i][j] != TILENODRAW){
                if(newx + i < 0 || newx + i  > MAPWIDTH - 1 || newy + j < 0 || newy + j > MAPHEIGHT - 1){
                    return 1;
                }
            }
        }
    }
    for(x = 0; x < MAPWIDTH; ++x)
    {
        for(y = 0; y < MAPHEIGHT; ++y)
        {
            if(x >= newx && x < newx + 4){
                if(y >= newy && y < newy + 4){
                    if(map[x][y] != TILEBLACK){
                        if(piece.size[x - newx][y - newy] != TILENODRAW){
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

void remove_row(int row)
{
    int x, y;
    int ctr = 0;

    for(x = 0; x < MAPWIDTH; ++x)
    {
        for(y = row; y > 0; --y)
        {
            map[x][y] = map[x][--y];
        }
    }
}
