#include "raylib.h"

#include "object.h"
#include "scene.h"

#include "demoscenes.h"

#include <iostream>

static std::unique_ptr<Scene> (*sceneArray[])() = {
    &SpannerScene, &TestingScene
};

int main(int argc, const char **argv) {
    InitWindow(1000, 800, "Signify SDF Viewer");

    SetTargetFPS(45);

    int scene = 0;

    std::unique_ptr<Scene> s = sceneArray[scene]();

    while(!WindowShouldClose()) {
        float delta = GetFrameTime(), time = GetTime();

        switch (scene) {
            case 0:
                s->SetLayerTransform(MatrixRotateZYX(Vector3{0, time * 2, PI/5}));
            break;
        }

        if (IsWindowResized()) {
            s->WindowResized();
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            s->Render();
            DrawText(TextFormat("FPS: %.02f\nDelta: %.04f", 1/delta, delta), 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}