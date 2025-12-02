//
// Created by gyan on 5/19/24.
//

#include <game.hpp>
#include <raylib.h>
#include <screens.hpp>
#include <screen.hpp>
#include <resources.hpp>
#include <cmath>
#include <raymath.h>
#include <raygui.h>
#include <vector>
#include <string>
#include <algorithm>


struct resources resources;
struct GameState gamestate;

Camera2D camera = {
        .rotation = 0.0f,
        .zoom = 0.5f
};

int mouse_lockX;

void MainGameDisplay() {

    int left = gamestate.player_pos.x - 10 * GetScreenWidth();
    int right = gamestate.player_pos.x + 10 * GetScreenWidth();
    int top = gamestate.player_pos.y - 10 * GetScreenHeight();
    int bottom = gamestate.player_pos.y + 10 * GetScreenHeight();

    if(left % 1024 != 0) {
        left = left - (left % 1024);
        right = right - (right % 1024);
    }
    if(top % 1024 != 0) {
        top = top - (top % 1024);
        bottom = bottom - (bottom % 1024);
    }

    BeginMode2D(camera);
    // Draw background
    for(int i = top; i < bottom; i+=1024) {
        for(int j = left; j < right; j+=1024) {
            DrawTexture(resources.spacebg, j, i, WHITE);
        }
    }

    //Draw player bullets
    for(auto & player_bullet : gamestate.player_bullets) {
        DrawTexturePro(resources.player_bullet, {0, 0, 249, 114}, {player_bullet.pos.x, player_bullet.pos.y, 62.25, 28.5}, {31.125, 14.25}, player_bullet.rotation, WHITE);
    }

    //Draw enemy ships
    for(auto & enemy_ship : gamestate.enemy_ships) {
        DrawTexturePro(resources.enemy_ship, {0, 0, 136, 116}, {enemy_ship.pos.x, enemy_ship.pos.y, 136, 116}, {68, 58}, enemy_ship.rotation, WHITE);
        float currentHealth = enemy_ship.currenthealth;
        GuiProgressBar((Rectangle){enemy_ship.pos.x, enemy_ship.pos.y, 100, 20}, NULL, NULL, &currentHealth, 0, enemy_ship.hitpoints);
    }

    //Draw enemy bullets
    for(auto & enemy_bullet : gamestate.enemy_bullets) {
        DrawTexturePro(resources.enemy_bullets, {0, 0, 251, 114}, {enemy_bullet.pos.x, enemy_bullet.pos.y, 62.25, 28.5}, {31.125, 14.25}, enemy_bullet.rotation, WHITE);
    }

    // Draw player's ship
    DrawTexturePro(resources.ship, {0, 0, 112, 88}, {gamestate.player_pos.x, gamestate.player_pos.y, 112, 88}, {56, 44}, -camera.rotation, WHITE);

    EndMode2D();

    float player_health = gamestate.player_health;
    GuiProgressBar({10, (float)GetScreenHeight() - 30, 300, 20}, NULL, NULL, &player_health, 0, 100);
}

void MainGameProcess() {

    camera.rotation = camera.rotation + (mouse_lockX - GetMouseX()) / 5;
    camera.target = (Vector2) {
            .x = gamestate.player_pos.x,
            .y = gamestate.player_pos.y
    };
    camera.offset = (Vector2) {
            .x = (float) GetScreenWidth() / 2,
            .y = (float) GetScreenHeight() / 1.5f
    };
    if (!IsSoundPlaying(resources.game_music)) {
        PlaySound(resources.game_music);
    }

    DisableCursor();
    mouse_lockX = GetMouseX();
    if(IsKeyPressed(KEY_ESCAPE)) {
        EnableCursor();
        displayScreen(&PauseScreen);
    }

    if(IsKeyDown(KEY_W)) {
        gamestate.player_speed.y -= cos(camera.rotation * PI / 180.0f) / 25;
        gamestate.player_speed.x -= sin(camera.rotation * PI / 180.0f) / 25;
    }
    if(IsKeyDown(KEY_S)) {
        gamestate.player_speed.y += cos(camera.rotation * PI / 180.0f) / 50;
        gamestate.player_speed.x += sin(camera.rotation * PI / 180.0f) / 50;
    }
    if(IsKeyDown(KEY_A)) {
        gamestate.player_speed.y += sin(camera.rotation * PI / 180.0f) / 50;
        gamestate.player_speed.x -= cos(camera.rotation * PI / 180.0f) / 50;
    }
    if(IsKeyDown(KEY_D)) {
        gamestate.player_speed.y -= sin(camera.rotation * PI / 180.0f) / 50;
        gamestate.player_speed.x += cos(camera.rotation * PI / 180.0f) / 50;
    }

    gamestate.player_pos.x += gamestate.player_speed.x;
    gamestate.player_pos.y += gamestate.player_speed.y;


    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(resources.gunfire);
        gamestate.player_bullets.push_back((PlayerBullet) {
            .pos = gamestate.player_pos,
            .speed = (Vector2) {
                .x = gamestate.player_speed.x - 10 * sin(camera.rotation * PI / 180.0f),
                .y = gamestate.player_speed.y - 10 * cos(camera.rotation * PI / 180.0f)
                },
            .rotation = -camera.rotation - 90
        });
    }



    //Update projectile positions
    for(auto & player_bullet : gamestate.player_bullets) {
        player_bullet.pos.x += player_bullet.speed.x;
        player_bullet.pos.y += player_bullet.speed.y;
    }

    if(gamestate.enemy_ships.size() < 2) {
        gamestate.enemy_ships.push_back((EnemyShip) {
            .pos = (Vector2) {
                .x = (float)(gamestate.player_pos.x + GetRandomValue(3 * GetScreenWidth(), 2 * GetScreenWidth()) * (GetRandomValue(0, 1) ? (-1) : (1))),
                .y = (float)(gamestate.player_pos.y + GetRandomValue(3 * GetScreenHeight(), 2 * GetScreenHeight()) * (GetRandomValue(0, 1) ? (-1) : (1)))
            },
            .speed = gamestate.player_speed,
            .rotation = (float)GetRandomValue(0, 360),
            .hitpoints = 100,
            .currenthealth = 100,
            .lastfiretime = 0
        });
    }

    for(auto & enemy_ship : gamestate.enemy_ships) {
        enemy_ship.rotation = -180 * Vector2Angle((Vector2) {
            .x = gamestate.player_pos.x - enemy_ship.pos.x,
            .y = gamestate.player_pos.y - enemy_ship.pos.y
        }, (Vector2){1, 0}) / PI + 90;



        Vector2 player_direction = {
                .x = gamestate.player_pos.x - enemy_ship.pos.x,
                .y = gamestate.player_pos.y - enemy_ship.pos.y
        };
        Vector2Normalize(player_direction);
        if(Vector2Distance(enemy_ship.pos, gamestate.player_pos) > 500) {
            enemy_ship.speed.x += player_direction.x / 500;
            enemy_ship.speed.y += player_direction.y / 500;
        } else if(Vector2Distance(enemy_ship.pos, gamestate.player_pos) < 500) {
            enemy_ship.speed.x -= player_direction.x / 500;
            enemy_ship.speed.y -= player_direction.y / 500;
        }

        enemy_ship.pos.x += enemy_ship.speed.x;
        enemy_ship.pos.y += enemy_ship.speed.y;

        if(abs(enemy_ship.speed.x - gamestate.player_speed.x) > 10) {
            if(enemy_ship.speed.x - gamestate.player_speed.x > 0) {
                enemy_ship.speed.x--;
            } else {
                enemy_ship.speed.x++;
            }
        }
        if(abs(enemy_ship.speed.y - gamestate.player_speed.y) > 5) {
            if(enemy_ship.speed.y - gamestate.player_speed.y > 0) {
                enemy_ship.speed.y--;
            } else {
                enemy_ship.speed.y++;
            }
        }

        //Fire Enemy bullets
        if(enemy_ship.lastfiretime < GetTime() - 1) {
            PlaySound(resources.gunfire);
            gamestate.enemy_bullets.push_back((EnemyBullet) {
                .pos = enemy_ship.pos,
                .speed = (Vector2) {
                        .x = enemy_ship.speed.x - 10 * sin(-enemy_ship.rotation * PI / 180.0f),
                        .y = enemy_ship.speed.y - 10 * cos(-enemy_ship.rotation * PI / 180.0f)
                },
                .rotation = enemy_ship.rotation - 90
            });
            enemy_ship.lastfiretime = GetTime();
        }

    }

    for(auto & enemy_ship : gamestate.enemy_ships) {
        auto colliding = [&enemy_ship](auto & player_bullet) {
            if(CheckCollisionCircles(player_bullet.pos, 20, enemy_ship.pos, 50)) {
                enemy_ship.currenthealth -= 10;
                return true;
            } else {
                return false;
            }
        };
        std::erase_if(gamestate.player_bullets, colliding);
    }

    auto isHealthZero = [](auto & enemy_ship) {
        if(enemy_ship.currenthealth <= 0) {
            gamestate.score += 5;
            return true;
        } else {
            return false;
        }
    };
    std::erase_if(gamestate.enemy_ships, isHealthZero);

    auto isTooFar = [](auto & enemy_ship) {
        return Vector2Distance(enemy_ship.pos, gamestate.player_pos) > 3 * GetScreenWidth();
    };
    std::erase_if(gamestate.enemy_ships, isTooFar);

    //Update Enemy projectiles
    for(auto & enemy_bullet : gamestate.enemy_bullets) {
        enemy_bullet.pos.x += enemy_bullet.speed.x;
        enemy_bullet.pos.y += enemy_bullet.speed.y;
    }

    auto enemy_bullet_colliding = [](auto & enemy_bullet) {
        if(CheckCollisionCircles(enemy_bullet.pos, 20, gamestate.player_pos, 40)) {
            gamestate.player_health--;
            return true;
        } else {
            return false;
        }
    };
    std::erase_if(gamestate.enemy_bullets, enemy_bullet_colliding);

    if(gamestate.player_health <= 0) {
        PlaySound(resources.gameover);
        WaitTime(1);
        EnableCursor();
        gamestate.isgameover = true;
        displayScreen(&GameOverScreen);
    }

    MainGameDisplay();

}

void ResetGame() {
    gamestate = {
            .player_pos = (Vector2) {
                    .x = 0.0f,
                    .y = 0.0f,
            },
            .player_speed = (Vector2) {
                    .x = 0.0f,
                    .y = 0.0f
            },
            .player_health = 100,
            .score = 0,
            .isgameover = false
    };
    gamestate.player_bullets.clear();
    gamestate.enemy_bullets.clear();
    gamestate.enemy_ships.clear();
}

void GameOverScreen() {
    MainGameDisplay();

    if (gamestate.isgameover) {
        int msgbox = GuiMessageBox((Rectangle) {(float) GetScreenWidth() / 2 - 200, (float) GetScreenHeight() / 2 - 150, 400, 300},
                      "Game Over", ((std::string) "Score " + std::to_string(gamestate.score)).c_str(),
                      "Replay;Main Menu");

        if(msgbox == 0 || msgbox == 2) {
            displayScreen(&MainMenu);
        } else if(msgbox == 1) {
            ResetGame();
            displayScreen(&MainGameProcess);
        }

    }
}