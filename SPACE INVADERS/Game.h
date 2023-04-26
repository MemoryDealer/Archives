/* Game.h */

#ifndef GAME_H
#define GAME_H

#define WIN32_LEAN_AND_MEAN

#include "Sprite.h"

#include <stdlib.h>
#include <windows.h>

#include <SDL/SDL.h>
#include <SDL/SDL_Thread.h>

enum GameState
{
    STATE_MENU,
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_OVER,
    STATE_WON
};

class Game
{
    public:
        Game();
        ~Game();

        void LoadData();
        void DrawScene();
        void DrawIMG(SDL_Surface* img, int x, int y);
        void UpdateScore();
        static SDL_Surface* LoadSurface(const char* file);
        int GetAlienSpeed();
        int GetAlienYFraction();
        int CheckLevel();
        void NextLevel();

        int game_state;
        int level;
        int score;
        bool bFirst;
        bool bMothershipDir;
        SDL_Surface* screen;
        SDL_Event event;

        // Sprites
        void ShipFire();
        void ShipRespawn();
        void SpawnMothership();
        void DropBomb();
        void BulletCollision(Sprite& bullet);
        void BombCollision(Sprite& bomb);
        void DrawBlocks();
        void BlockCollision(Sprite& sprite);

        SpriteBase Ship_Base;
        SpriteBase Mothership_Base;
        SpriteBase DeadShip_Base;
        SpriteBase DeadMothership_Base;
        SpriteBase Alien_Base;
        SpriteBase DeadAlien_Base;
        SpriteBase Bullet_Base;
        SpriteBase Bomb_Base;
        SpriteBase Block_Base;

        Sprite Ship;
        Sprite Mothership;
        Sprite DeadShip;
        Sprite DeadMothership;
        Sprite Alien[10];
        Sprite Alien2[10];
        Sprite Alien3[10];
        Sprite Alien4[10];
        Sprite DeadAlien;
        Sprite Bullet[2];
        Sprite Bomb[5];
        Sprite Block[4][6];

    private:
        SDL_Surface* bg;
        SDL_Surface* lvl1;
        bool bDoubleShot;
};

#endif
