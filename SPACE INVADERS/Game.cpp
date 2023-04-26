/* Game.cpp */

#include "Game.h"

Game::Game()
{
    game_state = STATE_ACTIVE;
    score = 0;
    level = 0; // Level 1
    bDoubleShot = false;
    bMothershipDir = true; // going right

    srand(SDL_GetTicks());
}

Game::~Game()
{
    // n/a
}

void Game::LoadData()
{
    bg = LoadSurface("data/bg.bmp");
    //lvl1 = LoadSurface("text/level1.bmp");
    ///lvl1 = LoadSurface("data/bg.bmp");
    //DrawIMG(bg, 0, 0);

    /* Sprites */
    Ship_Base.Init("data/ship");
    Ship.Init(&Ship_Base, screen);
    Ship.Set(320, 428);
    Ship.SetFrame(0);
    Ship.SetSpeed(1);
    Ship.StopAnim();
    Ship.bDraw = true;
    Ship.lives = 3;
    Ship.life = 70;

    Mothership_Base.Init("data/mother");
    Mothership.Init(&Mothership_Base, screen);
    Mothership.SetY(-10);
    Mothership.SetFrame(0);
    Mothership.SetSpeed(1);
    Mothership.StopAnim();
    Mothership.bDraw = false;
    Mothership.life = 170;

    DeadShip_Base.Init("data/deadship");
    DeadShip.Init(&DeadShip_Base, screen);
    DeadShip.SetFrame(0);
    DeadShip.StopAnim();

    DeadMothership_Base.Init("data/deadmother");
    DeadMothership.Init(&DeadMothership_Base, screen);
    DeadMothership.SetFrame(0);
    DeadMothership.StopAnim();

    Bullet_Base.Init("data/bullet");
    for(int i=0; i<2; ++i)
    {
        Bullet[i].Init(&Bullet_Base, screen);
        Bullet[i].SetFrame(0);
        Bullet[i].SetSpeed(1);
        Bullet[i].StopAnim();
    }

    Bomb_Base.Init("data/bomb");
    for(int i=0; i<5; ++i)
    {
        Bomb[i].Init(&Bomb_Base, screen);
        Bomb[i].SetFrame(0);
        Bomb[i].SetSpeed(1);
    }

    Block_Base.Init("data/block");
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<6; ++j)
        {
            Block[i][j].Init(&Block_Base, screen);
            Block[i][j].SetFrame(0);
            Block[i][j].bDraw = true;
            Block[i][j].StopAnim();
            Block[i][j].life = 5;
        }
    }

    Alien_Base.Init("data/alien");
    for(int i=0; i<10; ++i)
    {
        Alien[i].Init(&Alien_Base, screen);
        Alien2[i].Init(&Alien_Base, screen);
        Alien3[i].Init(&Alien_Base, screen);
        Alien4[i].Init(&Alien_Base, screen);
    }

    int aX = 20;
    int aY = 65;

        for(int i=0; i<10; ++i)
        {
            Alien[i].Set((aX += 30), aY);
            Alien[i].SetFrame(0);
            Alien[i].SetSpeed(1);
            Alien[i].bDraw = true;
            Alien[i].life = 30;
        }

    aX = 20;
    aY += 30;
        for(int i=0; i<10; ++i)
        {
            Alien2[i].Set((aX += 30), aY);
            Alien2[i].SetFrame(0);
            Alien2[i].SetSpeed(1);
            Alien2[i].bDraw = true;
            Alien2[i].life = 30;
        }
    aX = 20;
    aY += 30;
        for(int i=0; i<10; ++i)
        {
            Alien3[i].Set((aX += 30), aY);
            Alien3[i].SetFrame(0);
            Alien3[i].SetSpeed(1);
            Alien3[i].bDraw = true;
            Alien3[i].life = 30;
        }
    aX = 20;
    aY += 30;
        for(int i=0; i<10; ++i)
        {
            Alien4[i].Set((aX += 30), aY);
            Alien4[i].SetFrame(0);
            Alien4[i].SetSpeed(1);
            Alien4[i].bDraw = true;
            Alien4[i].life = 30;
        }

    DeadAlien_Base.Init("data/deadalien");
    DeadAlien.Init(&DeadAlien_Base, screen);
    DeadAlien.SetFrame(0);
    DeadAlien.StopAnim();

    /* ------- */
}

void Game::DrawScene()
{
    SDL_FillRect(screen, NULL, 0);

    if(bFirst)
    {
        DrawIMG(lvl1, 0, 0);
        bFirst = false;
    }

    DrawIMG(bg, 0, 0);

    /* Sprites */
    if(Ship.bDraw)
    {
        Ship.ClearBG();

        Ship.UpdateBG();

        Ship.Draw();
        Ship.SetFrame(0);
    }
    else
    {
        //! blah
        DeadShip.Set(Ship.GetX(), Ship.GetY());
        if(Ship.life > 0)
        {
            DeadShip.Draw();
            Ship.life--;
        }
        else
            Ship.life = 70;
            SDL_Delay(500);
            ShipRespawn();
    }

    // draw mothership
    if(Mothership.bDraw)
    {
        Mothership.ClearBG();
        Mothership.UpdateBG();
        Mothership.Draw();
        if(bMothershipDir)
            Mothership.MoveX(1);
        else
            Mothership.MoveX(-1);

        if(Mothership.GetX() == 640 || Mothership.GetX() == -66)
            Mothership.bDraw = false;
    }
    else
    {
        DeadMothership.Set(Mothership.GetX(), Mothership.GetY());
        if(Mothership.life > 0)
        {
            DeadMothership.Draw();
            Mothership.life--;
        }
        //else
        //    Mothership.life = 170;
    }

    // draw aliens
    for(int i=0; i<10; ++i)
    {
        if(Alien[i].bDraw)
        {
            Alien[i].Draw();
        }
        else
        {
            DeadAlien.Set(Alien[i].GetX(), Alien[i].GetY());
            if(Alien[i].life > 0)
            {
                DeadAlien.Draw();
                Alien[i].life--;
            }
        }

        if(Alien2[i].bDraw)
        {
            Alien2[i].Draw();
        }
        else
        {
            DeadAlien.Set(Alien2[i].GetX(), Alien2[i].GetY());
            if(Alien2[i].life > 0)
            {
                DeadAlien.Draw();
                Alien2[i].life--;
            }
        }

        if(Alien3[i].bDraw)
        {
            Alien3[i].Draw();
        }
        else
        {
            DeadAlien.Set(Alien3[i].GetX(), Alien3[i].GetY());
            if(Alien3[i].life > 0)
            {
                DeadAlien.Draw();
                Alien3[i].life--;
            }
        }

        if(Alien4[i].bDraw)
        {
            Alien4[i].Draw();
        }
        else
        {
            DeadAlien.Set(Alien4[i].GetX(), Alien4[i].GetY());
            if(Alien4[i].life > 0)
            {
                DeadAlien.Draw();
                Alien4[i].life--;
            }
        }
    }

    // draw bombs
    for(int i=0; i<5; ++i)
    {
        if(Bomb[i].bDraw)
        {
            Bomb[i].MoveY(1);
            BombCollision(Bomb[i]);
            BlockCollision(Bomb[i]);
            Bomb[i].Draw();
        }
    }

    DrawBlocks();

    /* ------- */

    /* Bullets */
    for(int i=0; i<2; ++i)
    {
        if(Bullet[i].bDraw == true)
        {
            Bullet[i].MoveY(-1);
            BulletCollision(Bullet[i]);
            BlockCollision(Bullet[i]);
            Bullet[i].Draw();
        }
    }
    /* ------- */

    SDL_Flip(screen);
}

void Game::DrawIMG(SDL_Surface* img, int x, int y)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_BlitSurface(img, NULL, screen, &dst);
}

void Game::UpdateScore()
{
    char buf[128];
    sprintf(buf, "Space Invaders | Score: %d", score);
    SDL_WM_SetCaption(buf, NULL);
}

SDL_Surface* Game::LoadSurface(const char* file)
{
    SDL_Surface* temp = NULL;
    SDL_Surface* ret = NULL;

    temp = SDL_LoadBMP(file);
    ret = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    if(ret == NULL)
    {
        MessageBox(NULL, "ret = NULL", 0, 0);
    }

    return ret;
}

int Game::GetAlienSpeed()
{
    switch(level) // increases with level
    {
        case 0: return 10;
        case 1: return 13;
        case 2: return 16;
        case 3: return 20;
        case 4: return 25;

        default:
            return 25;
    }
}

int Game::GetAlienYFraction()
{
    switch(level) // increases with level
    {
        case 0: return 5;
        case 1: return 9;
        case 2: return 12;
        case 3: return 17;
        case 4: return 22;

        default:
            return 22;
    }
}

int Game::CheckLevel()
{
    for(int i=0; i<10; ++i)
    {
        if(Alien[i].bDraw == true)
            return 1;

        if(Alien2[i].bDraw == true)
            return 1;

        if(Alien3[i].bDraw == true)
            return 1;

        if(Alien4[i].bDraw == true)
            return 1;
    }

    return 0;
}

void Game::NextLevel()
{
    if(level < 3)
    {
        int x = 20;
        int y = (65 + GetAlienSpeed());
        level++;

        for(int i=0; i<10; ++i)
        {
            Alien[i].Set((x += 30), y);
            Alien[i].SetFrame(0);
            Alien[i].bDraw = true;
            Alien[i].life = 30;
        }
        x = 20;
        y += 30;
        for(int i=0; i<10; ++i)
        {
            Alien2[i].Set((x += 30), y);
            Alien2[i].SetFrame(0);
            Alien2[i].bDraw = true;
            Alien2[i].life = 30;
        }
        x = 20;
        y += 30;
        for(int i=0; i<10; ++i)
        {
            Alien3[i].Set((x += 30), y);
            Alien3[i].SetFrame(0);
            Alien3[i].bDraw = true;
            Alien3[i].life = 30;
        }
        x = 20;
        y += 30;
        for(int i=0; i<10; ++i)
        {
            Alien4[i].Set((x += 30), y);
            Alien4[i].SetFrame(0);
            Alien4[i].bDraw = true;
            Alien4[i].life = 30;
        }
    }
}

void Game::ShipFire()
{
    // bullet = 5x19
    // ship = 64x52
    if(bDoubleShot)
    {
        for(int i=0; i<2; ++i)
        {
            if(Bullet[i].bDraw == false)
            {
                Bullet[i].bDraw = true;
                Bullet[i].Set(Ship.GetX() + 30, 411);
                return;
            }
        }
    }
    else
    {
        if(Bullet[0].bDraw == false)
        {
            Bullet[0].bDraw = true;
            Bullet[0].Set(Ship.GetX() + 30, 411);
        }
    }

}

void Game::ShipRespawn()
{
    if(Ship.lives > 0)
    {
        Ship.Set(320, 428);
        Ship.bDraw = true;
    }
    else
    {
        game_state = STATE_OVER;
        Ship.ClearBG();
    }
}

void Game::SpawnMothership()
{
    Mothership.bDraw = true;
}

void Game::DropBomb()
{
    int a;
    // find an alien to drop the bomb

    for(int i=0; i<1; ++i)
    {
        a = rand() % 10 + 1;
        if(Alien[a].bDraw == true && Bomb[i].bDraw == false)
        {
            Bomb[i].bDraw = true;
            Bomb[i].Set(Alien[a].GetX() + 22, Alien[a].GetY() + 5);
        }
    }

}

void Game::BulletCollision(Sprite& bullet)
{
    // check collision with aliens ( 25x19 )
    for(int i=0; i<10; ++i)
    {
        if(Alien[i].bDraw)
        {
             // check x pos
             if((Alien[i].GetX() < bullet.GetX() || Alien[i].GetX() < (bullet.GetX() + 5))
                && Alien[i].GetX() + 25 > bullet.GetX())
             {
                 // check y pos
                 if(Alien[i].GetY() < bullet.GetY() && Alien[i].GetY() + 19 > bullet.GetY())
                 {
                     Alien[i].bDraw = false;
                     bullet.bDraw = false;
                     score += 30;
                     UpdateScore();
                     return;
                 }
             }
        }
        if(Alien2[i].bDraw)
        {
             // check x pos
             if((Alien2[i].GetX() < bullet.GetX() || Alien2[i].GetX() < (bullet.GetX() + 5))
                && Alien2[i].GetX() + 25 > bullet.GetX())
             {
                 // check y pos
                 if(Alien2[i].GetY() < bullet.GetY() && Alien2[i].GetY() + 19 > bullet.GetY())
                 {
                     Alien2[i].bDraw = false;
                     bullet.bDraw = false;
                     score += 20;
                     UpdateScore();
                     return;
                 }
             }
        }
        if(Alien3[i].bDraw)
        {
             // check x pos
             if((Alien3[i].GetX() < bullet.GetX() || Alien3[i].GetX() < (bullet.GetX() + 5))
                && Alien3[i].GetX() + 25 > bullet.GetX())
             {
                 // check y pos
                 if(Alien3[i].GetY() < bullet.GetY() && Alien3[i].GetY() + 19 > bullet.GetY())
                 {
                     Alien3[i].bDraw = false;
                     bullet.bDraw = false;
                     score += 10;
                     UpdateScore();
                     return;
                 }
             }
        }
        if(Alien4[i].bDraw)
        {
             // check x pos
             if((Alien4[i].GetX() < bullet.GetX() || Alien4[i].GetX() < (bullet.GetX() + 5))
                && Alien4[i].GetX() + 25 > bullet.GetX())
             {
                 // check y pos
                 if(Alien4[i].GetY() < bullet.GetY() && Alien4[i].GetY() + 19 > bullet.GetY())
                 {
                     Alien4[i].bDraw = false;
                     bullet.bDraw = false;
                     score += 10;
                     UpdateScore();
                     return;
                 }
             }
        }

        // check mothership
        if(Mothership.bDraw)
        {
            if((Mothership.GetX() < bullet.GetX() || Mothership.GetX() < (bullet.GetX() + 5))
                && Mothership.GetX() + 64 > bullet.GetX())
            {
                // check y pos
                if(Mothership.GetY() < bullet.GetY() && Mothership.GetY() + 56 > bullet.GetY())
                {
                    Mothership.bDraw = false;
                    bullet.bDraw = false;
                    score += 200;
                    UpdateScore();
                    return;
                }
            }
        }
    }
    // check out of bounds
    if(bullet.GetY() == -19)
    {
        bullet.bDraw = false;
    }
}

void Game::BombCollision(Sprite& bomb)
{
    // check xpos
    if((bomb.GetX() > Ship.GetX() || bomb.GetX() + 5 > Ship.GetX())
        && bomb.GetX() + 5 < Ship.GetX() + 64)
    {
        // check y pos
        if(bomb.GetY() + 19 > Ship.GetY())
        {
            bomb.bDraw = false;
            Ship.bDraw = false;
            Ship.lives--;
        }
    }
    else if(bomb.GetY() > 480)
    {
        bomb.bDraw = false;
    }
}

void Game::DrawBlocks()
{
    int x = -85;
    int y = 335;
    for(int i=0; i<4; ++i)
    {
        x += 85;
        for(int j=0; j<3; ++j)
        {
            Block[i][j].Set(x, (j % 2 != 0) ? y : (y + 32));
            x += 32;

            if(Block[i][j].bDraw)
                Block[i][j].Draw();
        }
    }
}

void Game::BlockCollision(Sprite& sprite)
{
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            // check x pos
            if(Block[i][j].bDraw)
            {
                if((Block[i][j].GetX() < sprite.GetX() || Block[i][j].GetX() < sprite.GetX() + 5)
                    && Block[i][j].GetX() + 32 > sprite.GetX())
                {
                    // check y pos
                    if((Block[i][j].GetY() < sprite.GetY() + 19 || Block[i][j].GetY() < sprite.GetY())
                        && Block[i][j].GetY() + 32 > sprite.GetY())
                    {
                        Block[i][j].life--;
                        sprite.bDraw = false;
                        switch(Block[i][j].life)
                        {
                            case 4:
                                Block[i][j].SetFrame(1);
                                break;

                            case 3:
                                Block[i][j].SetFrame(2);
                                break;

                            case 2:
                                Block[i][j].SetFrame(3);
                                break;

                            case 1:
                                Block[i][j].SetFrame(4);
                                break;

                            case 0:
                                Block[i][j].bDraw = false;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
}
