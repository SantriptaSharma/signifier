#include "raylib.h"

#include "object.h"
#include "scene.h"

#include "demoscenes.h"

#include <iostream>

static std::unique_ptr<Scene> (*sceneArray[])() = {
    &SpannerScene, &TestingScene, &MetaBallsScene, &WazowskiScene
};

int main(int argc, const char **argv) {
    InitWindow(1000, 800, "Signify SDF Viewer");

    SetTargetFPS(45);

    int scene = 3;

    std::unique_ptr<Scene> s = sceneArray[scene]();

    while(!WindowShouldClose()) {
        float delta = GetFrameTime(), time = GetTime();

        std::shared_ptr<Object> obj1, obj2, obj3, obj4;

        switch (scene) {
            case 0:
                s->SetLayerTransform(MatrixRotateZYX(Vector3{0, time * 2, PI/5}));
            break;

            case 2:
                obj1 = s->GetObject(0);
                obj2 = s->GetObject(1);

                obj1->transform = MatrixTranslate((sin(time) + 1) * 2.5f, 0, 0);
                obj2->transform = MatrixTranslate((sin(time * 2) - 1) * 2.5f, 0, 0);
            break;
        }

        auto lc = s->GetLayerConfig();

        auto change = IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN);

        if (change != 0) {
            lc.blendFactor += change * 0.05;
            lc.blendFactor = Clamp(lc.blendFactor, 0.0f, 100.0f);
            s->SetLayerConfig(lc);
        }


        if (IsWindowResized()) {
            s->WindowResized();
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            s->Render();
            DrawText(TextFormat("FPS: %.02f\nDelta: %.04f", 1/delta, delta), 10, 10, 20, DARKGRAY);
            DrawText(TextFormat("Layer 0 blend strength: %.04f", lc.blendFactor), 10, 45, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}