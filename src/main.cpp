#include "raylib.h"

#include "object.h"
#include "scene.h"

#include "demoscenes.h"

#include <iostream>

static std::unique_ptr<Scene> (*sceneArray[])() = {
    &MetaBallsScene, &TestingScene, &SpannerScene, &WazowskiScene
};

int main(int argc, const char **argv) {
    InitWindow(1000, 800, "Signify SDF Viewer");

    SetTargetFPS(45);

    int scene = 0;

    std::unique_ptr<Scene> s = sceneArray[scene]();

    while(!WindowShouldClose()) {
        float delta = GetFrameTime(), time = GetTime();

        std::shared_ptr<Object> obj1, obj2, obj3, obj4;

        int one = IsKeyPressed(KEY_ONE);
        int two = IsKeyPressed(KEY_TWO) * 2;
        int three = IsKeyPressed(KEY_THREE) * 3;
        int four = IsKeyPressed(KEY_FOUR) * 4;

        if (one || two || three || four) {
            int selected = (one + two + three + four) % 5;
            scene = selected - 1;
            std::cout <<  selected << std::endl;
            s = sceneArray[selected - 1]();
        }

        switch (scene) {
            case 2:
                s->SetLayerTransform(MatrixRotateZYX(Vector3{0, time * 2, PI/5}));
            break;

            case 0:
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