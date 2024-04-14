#include "raylib.h"

#include "object.h"
#include "scene.h"

#include <iostream>

std::unique_ptr<Scene> setupScene() {
    Camera cam = {0};
    cam.position = Vector3{0, 0, 10};
    cam.target = Vector3{0, 0, 0};
    cam.up = Vector3{0, 1, 0};
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    std::cout << GetWorkingDirectory() << std::endl;
    Shader marcher = LoadShader(0, "res/shaders/march.fs");

    std::unique_ptr<Scene> scene = std::make_unique<Scene>(SceneConfig{}, cam, marcher);
    scene->AddObject(MakeSphere(Vector3{-1.5f, 0, 0}, 2.0f, {255, 0, 0, 255}));
    scene->AddObject(MakeSphere(Vector3{1.5f, 0, 0}, 1.0f, BLUE));
    return scene;
}

int main(int argc, const char **argv) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 800, "Signify SDF Editor");
    SetWindowMinSize(800, 600);

    SetTargetFPS(60);

    std::unique_ptr<Scene> s = setupScene();

    while(!WindowShouldClose()) {
        s->Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        s->Render();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}