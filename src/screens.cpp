//
// Created by gyan on 5/18/24.
//

#include <screens.hpp>
#include <raylib.h>
#include <chrono>
#include <screen.hpp>
#include <thread>
#include <resources.hpp>
#include <raygui.h>
#include <game.hpp>

bool isDataLoaded = false;
bool isLoadThreadCalled = false;

extern int mouse_lockX;

void LoadingScreen() {
    ClearBackground(BLACK);
    DrawText("Loading...", GetScreenWidth() / 2 - 110, GetScreenHeight() / 2 - 25, 50, GRAY);

    //Load Resources
    if (!isLoadThreadCalled) {
        //std::thread load_resources(loadResources, &isDataLoaded);
        isLoadThreadCalled = true;
        //load_resources.detach();
        // Cant load textures in different thread

        loadResources(&isDataLoaded);  // To be changed with multi threaded option in case project grows
    }

    if(isDataLoaded) {
        // Load images to textures in main thread because opengl context is unavailable in other thread
        displayScreen(&MainMenu);
    }
}



void AboutPage() {
    ClearBackground(BLACK);
    int defsize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 60);
    GuiLabel((Rectangle){(float)GetScreenWidth() / 2 - 100, 20, 400, 60}, "About");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiLabel((Rectangle){100, 70, 400, 60}, "Controls");

    GuiSetStyle(DEFAULT, TEXT_SIZE, defsize);
    GuiLabel((Rectangle){100, 70, (float)GetScreenWidth() - 200, 200}, "Use W, A, S, D to accelerate\nUse mouse to turn around\nUse left click to fire bullets\nPress ESC to pause game or go back to menu");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiLabel((Rectangle){100, 250, 400, 60}, "Objective");

    GuiSetStyle(DEFAULT, TEXT_SIZE, defsize);
    GuiLabel((Rectangle){100, 250, (float)GetScreenWidth() - 200, 200}, "Destroy the enemy space ships to score points, Move around to dodge bullets shot by enemies.\nDestroy as many ships as you can before you are taken down by them.");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiLabel((Rectangle){100, 400, 400, 60}, "Made By");

    GuiSetStyle(DEFAULT, TEXT_SIZE, defsize);
    GuiLabel((Rectangle){100, 400, (float)GetScreenWidth() - 200, 200}, "Gyan Prakash (2023ug1091)\ngyan.2023ug1091@iiitranchi.ac.in\n");
    GuiLabel((Rectangle){100, 450, (float)GetScreenWidth() - 200, 200}, "Shubham Kumar (2023ug1090)\nshubham.2023ug1090@iiitranchi.ac.in\n");
    GuiLabel((Rectangle){100, 500, (float)GetScreenWidth() - 200, 200}, "Shubham Singh (2023ug1099)\nshubham.2023ug1099@iiitranchi.ac.in\n");
    GuiLabel((Rectangle){100, 550, (float)GetScreenWidth() - 200, 200}, "Kasturi Marandi (2023ug1114)\nkasturi.2023ug1114@iiitranchi.ac.in\n");
    if(IsKeyPressed(KEY_ESCAPE)) {
        displayScreen(&MainMenu);
    }



}

void PauseScreen() {

    MainGameDisplay();

    int defsize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 50);
    GuiLabel((Rectangle){(float)GetScreenWidth() / 2 - 100, 100, 400, 60}, "Game Paused");
    GuiSetStyle(DEFAULT, TEXT_SIZE, defsize);
    GuiLabel((Rectangle){(float)GetScreenWidth() / 2 - 100, 200, 400, 60}, "Press ESC or R to resume");

    if(IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_R)) {
        mouse_lockX = GetMouseX();
        displayScreen(&MainGameProcess);
    }

    if(GuiButton((Rectangle){(float)GetScreenWidth() / 2 - 90, 300, 200, 60}, "Resume")) {
        PlaySound(resources.button_click);
        mouse_lockX = GetMouseX();
        displayScreen(&MainGameProcess);
    }
    if(GuiButton((Rectangle){(float)GetScreenWidth() / 2 - 90, 380, 200, 60}, "Main Menu")) {
        PlaySound(resources.button_click);
        displayScreen(&MainMenu);
    };
    if(GuiButton((Rectangle){(float)GetScreenWidth() / 2 - 90, 460, 200, 60}, "Exit")) {
        PlaySound(resources.button_click);
        CloseAudioDevice();
        CloseWindow();
        exit(0);
    }
}



void MainMenu() {
    if (!IsSoundPlaying(resources.game_music)) {
        PlaySound(resources.game_music);
    }
    ClearBackground(BLACK);
    int defsize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 50);
    GuiLabel((Rectangle){(float)GetScreenWidth() / 2 - 100, 20, 400, 60}, "Main Menu");
    GuiSetStyle(DEFAULT, TEXT_SIZE, defsize);

    if(GuiButton((Rectangle){(float)GetScreenWidth() / 2 - 90, 300, 200, 60}, "Play")) {
        PlaySound(resources.button_click);
        ResetGame();
        displayScreen(&MainGameProcess);
    }
    if(GuiButton((Rectangle){(float)GetScreenWidth() / 2 - 90, 380, 200, 60}, "About")) {
        PlaySound(resources.button_click);
        displayScreen(&AboutPage);
    };
    if(GuiButton((Rectangle){(float)GetScreenWidth() / 2 - 90, 460, 200, 60}, "Exit")) {
        PlaySound(resources.button_click);
        CloseAudioDevice();
        CloseWindow();
        exit(0);
    }

    DrawText("v-1.6.0 - release", 0, GetScreenHeight() - 10, 10, WHITE);
}