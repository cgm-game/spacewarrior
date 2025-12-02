#include <raylib.h>
#include <screen.hpp>
#include <functional>
#include <screens.hpp>
#include <resources.hpp>
#include <game.hpp>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

std::function<void()> currentScreenDrawFunction;

int main() {
    InitWindow(1200, 800, "Space Warrior");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);

    displayScreen(&LoadingScreen);

    while(!WindowShouldClose()) {

        BeginDrawing();
        currentScreenDrawFunction();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();


    return 0;
}
