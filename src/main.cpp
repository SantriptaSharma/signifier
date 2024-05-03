#include "raylib.h"
#include "rlgl.h"

#include "object.h"
#include "scene.h"

#include <iostream>

static std::shared_ptr<Object> rotatingSphere = nullptr;

std::unique_ptr<Scene> setupScene() {
    std::cout << GetWorkingDirectory() << std::endl;
    Shader marcher = LoadShader(0, "res/shaders/march.fs");

    std::unique_ptr<Scene> scene = std::make_unique<Scene>(SceneConfig{}, marcher);
    scene->AddObject(Object::MakeSphere(Vector3{0, 0, 0}, 1.5f, RED));
    scene->AddObject(Object::MakeSphere(Vector3{-5, 0, 3}, 1.0f, YELLOW));
    
    auto box = scene->AddObject(Object::MakeBox(Vector3{-5, 0, 3}, Vector3{2.0f, 0.5f, 0.5f}, GREEN));
    box->combineType = CombineType::SUBTRACT;
    box->transform = MatrixMultiply(MatrixRotateXYZ(Vector3{-PI/4, PI/4, -PI/4}), box->transform);

    auto cylinder = scene->AddObject(Object::MakeCylinder(Vector3{3, 0, 0}, 1.0f, 2.0f, PURPLE));
    cylinder->transform = MatrixMultiply(MatrixRotateXYZ(Vector3{0, 0, PI/2}), cylinder->transform);

    auto cap = scene->AddObject(Object::MakeCylinder(Vector3{-0.5, 0, 0}, 0.8f, 1.5f, PURPLE));
    cap->Rotate(MatrixRotateZ(PI/2));
    cap->combineType = CombineType::SUBTRACT;

    auto capInner = scene->AddObject(Object::MakeCylinder(Vector3{-0.3, 0, 0}, 0.6f, 2, PURPLE));
    capInner->Rotate(MatrixRotateZ(PI/2));
    capInner->combineType = CombineType::SUBTRACT;

    rotatingSphere = scene->AddObject(Object::MakeSphere(Vector3{1.5f, 0, 0}, 1.0f, YELLOW));
    rotatingSphere->combineType = CombineType::SUBTRACT;

    box = Object::MakeBox(Vector3{0, -3, 0}, Vector3{10, 0.2, 10}, BROWN);
    box->combineType = CombineType::UNION;
    scene->AddObject(box);

    int layer = scene->CreateLayer("SmallFloor");
    box = Object::MakeBox(Vector3{0, -2, 0}, Vector3{5, 0.2, 5}, GREEN);
    scene->AddObject(box, layer);

    cylinder = scene->AddObject(Object::MakeCylinder(Vector3{-3, -2, 0}, 1.0f, 2.0f, ORANGE), layer);
    cylinder = scene->AddObject(Object::MakeCylinder(Vector3{-3, -2, 0}, 0.6f, 3.0f, ORANGE), layer);
    cylinder->combineType = CombineType::SUBTRACT;

    scene->AddLight(MakeDirectionalLight(Vector3{0.5, -1, -0.8}, WHITE, 0.5f));
    scene->AddLight(MakePointLight(Vector3{-3, -2, 0}, YELLOW, 0.5f));

    scene->SetClearColor(SKYBLUE);
    return scene;
}

int main(int argc, const char **argv) {
    InitWindow(1000, 800, "Signify SDF Viewer");

    SetTargetFPS(45);

    std::unique_ptr<Scene> s = setupScene();

    rlEnableDepthMask();
    rlEnableDepthTest();

    while(!WindowShouldClose()) {
        float delta = GetFrameTime();

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