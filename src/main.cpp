#include "raylib.h"

void frame() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Hello world", 190, 200, 20, BLACK);

    EndDrawing();
}

int main(int argc, const char **argv) {
    InitWindow(800, 450, "Raylib + Meson + C/C++");

    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        frame();
    }

    CloseWindow();

    return 0;
}