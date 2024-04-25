#include "raylib.h"

#include "object.h"
#include "scene.h"

#include <iostream>

static std::shared_ptr<Object> rotatingSphere = nullptr;

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
    scene->AddObject(Object::MakeSphere(Vector3{0, 0, 0}, 1.5f, RED));
    rotatingSphere = scene->AddObject(Object::MakeSphere(Vector3{1.5f, 0, 0}, 1.0f, BLUE));
    rotatingSphere->combineType = CombineType::SUBTRACT;
    scene->AddObject(Object::MakeSphere(Vector3{-5, 0, 3}, 1.0f, YELLOW));
    scene->AddObject(Object::MakeInfPlane(-3, MatrixIdentity(), GREEN));
    scene->AddLight(MakeDirectionalLight(Vector3{0.5, -1, -0.8}, WHITE, 0.5f));
    scene->SetClearColor(SKYBLUE);
    return scene;
}

int main(int argc, const char **argv) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 800, "Signify SDF Editor");
    SetWindowMinSize(800, 600);

    SetTargetFPS(60);

    std::unique_ptr<Scene> s = setupScene();

    while(!WindowShouldClose()) {
        float t = GetTime();

        float r = 1.8f + sin(t) * 0.6;
        float u = r * cos(t);
        float v = r * sin(t);
        rotatingSphere->transform = MatrixTranslate(u, 0, v);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        s->Render();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}