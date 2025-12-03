//
// Created by gyan on 5/18/24.
//

#include <resources.hpp>
#include <raylib.h>
#include <iostream>
#include <raygui.h>
#include <resource_dir.h>

void loadResources(bool* isDataLoaded) {
    std::cout << "Loading Resources..." << std::endl;

    InitAudioDevice();

    //Load Resources
    SearchAndSetResourceDir("spacewarrior");

    GuiLoadStyle("assets/style_cyber.rgs");

    resources.ship = LoadTexture("assets/ship.png");
    resources.spacebg = LoadTexture("assets/blue_nebula_bg.png");
    resources.player_bullet = LoadTexture("assets/player_bullet.png");
    resources.enemy_ship = LoadTexture("assets/enemy_ship.png");
    resources.enemy_bullets = LoadTexture("assets/enemy_bullet.png");

    resources.button_click = LoadSound("assets/button_click.mp3");
    resources.gunfire = LoadSound("assets/gunfire.wav");
    resources.gameover = LoadSound("assets/game_over.wav");
    resources.game_music = LoadSound("assets/game_music.mp3");

    resources.thrust1 = LoadTexture("assets/thrust1.png");
    resources.thrust2 = LoadTexture("assets/thrust2.png");
    resources.thrust3 = LoadTexture("assets/thrust3.png");

    resources.engine = LoadSound("assets/engine.mp3");

    *isDataLoaded = true;
}

