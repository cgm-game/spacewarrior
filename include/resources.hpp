//
// Created by gyan on 5/18/24.
//

#ifndef SPACEWARRIOR_RESOURCES_HPP
#define SPACEWARRIOR_RESOURCES_HPP

#include <raylib.h>

void loadResources(bool* isDataLoaded);

struct resources {
    Texture2D ship;
    Texture2D spacebg;
    Texture2D player_bullet;
    Texture2D enemy_ship;
    Texture2D enemy_bullets;
    Sound button_click;
    Sound gunfire;
    Sound gameover;
    Sound game_music;
    Texture2D thrust1;
    Texture2D thrust2;
    Texture2D thrust3;
    Sound engine;
};

extern struct resources resources;

#endif //SPACEWARRIOR_RESOURCES_HPP
