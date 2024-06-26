#pragma once

#include "scene.h"
#include "object.h"

#include <iostream>

std::unique_ptr<Scene> TestingScene() {
    Shader marcher = LoadShader(0, "res/shaders/march.fs");

    std::unique_ptr<Scene> scene = std::make_unique<Scene>(SceneConfig{.clear_color = SKYBLUE, .light_zero_point = 0.01f}, marcher);
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

    auto rotatingSphere = scene->AddObject(Object::MakeSphere(Vector3{1.5f, 0, 0}, 1.0f, YELLOW));
    rotatingSphere->combineType = CombineType::SUBTRACT;

    box = Object::MakeBox(Vector3{0, -8, 0}, Vector3{10, 0.2, 10}, BROWN);
    box->combineType = CombineType::UNION;
    scene->AddObject(box);

    int layer = scene->CreateLayer("SmallFloor", DEFAULT_LAYER_CONFIG);
    box = Object::MakeBox(Vector3{0, -2, 0}, Vector3{5, 0.2, 5}, GREEN);
    scene->AddObject(box, layer);

    cylinder = scene->AddObject(Object::MakeCylinder(Vector3{-3, -2, 0}, 1.0f, 2.0f, ORANGE), layer);
    cylinder = scene->AddObject(Object::MakeCylinder(Vector3{-3, -2, 0}, 0.6f, 3.0f, ORANGE), layer);
    cylinder->combineType = CombineType::SUBTRACT;

    scene->AddLight(MakeDirectionalLight(Vector3{0.5, -1, -0.8}, WHITE, 0.5f));

    return scene;
}

std::unique_ptr<Scene> SpannerScene() {
	Shader marcher = LoadShader(0, "res/shaders/march.fs");
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(SceneConfig{.clear_color = SKYBLUE, .light_zero_point = 0.01f}, marcher);

    scene->AddLight(MakeDirectionalLight(Vector3{0.5, -1, -0.8}, WHITE, 0.5f));

	auto handle = scene->AddObject(Object::MakeBox(Vector3{0.0f, 0.0f, 0.0f}, Vector3{1, 3, 0.5f}, BLUE));

	float cylinderDiff = 3.5f, cylinderRadius = 1.2f, cylinderDepth = 0.5f;


	auto handlecut = scene->AddObject(Object::MakeBox(Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.6f, 2.7f, 0.6f}, BLACK));
	handlecut->combineType = CombineType::SUBTRACT;

	auto topcylinder = scene->AddObject(Object::MakeCylinder(Vector3{0.0f, cylinderDiff, 0.0f}, cylinderRadius, cylinderDepth, GREEN));
	auto bottomcylinder = scene->AddObject(Object::MakeCylinder(Vector3{0.0f, -cylinderDiff, 0.0f}, cylinderRadius, cylinderDepth, RED));
	topcylinder->Rotate(MatrixRotateX(PI/2));
	bottomcylinder->Rotate(MatrixRotateX(PI/2));
	// topcylinder->combineType = CombineType::UNION;
	// bottomcylinder->combineType = CombineType::UNION;

	float interp = 1.15f;
	float radius = 0.65f;
	float xoff = 0.15f;

	auto topcylindercut = scene->AddObject(Object::MakeCylinder(Vector3{xoff, interp * cylinderDiff, 0.0f}, cylinderRadius * radius, cylinderDepth + 0.1, GREEN));
	auto bottomcylindercut = scene->AddObject(Object::MakeCylinder(Vector3{-xoff, -interp * cylinderDiff, 0.0f}, cylinderRadius * radius, cylinderDepth + 0.1, GREEN));
	
	topcylindercut->Rotate(MatrixRotateX(PI/2));
	bottomcylindercut->Rotate(MatrixRotateX(PI/2));

	topcylindercut->combineType = CombineType::SUBTRACT;
	bottomcylindercut->combineType = CombineType::SUBTRACT;

	auto lc = DEFAULT_LAYER_CONFIG;
	lc.blendFactor = 0.05f;
	scene->SetLayerConfig(lc);

	return scene;
}

std::unique_ptr<Scene> MetaBallsScene() {
	Shader marcher = LoadShader(0, "res/shaders/march.fs");
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(SceneConfig{.clear_color = SKYBLUE, .light_zero_point = 0.01f}, marcher);

    scene->AddLight(MakeDirectionalLight(Vector3{0.5, -1, -0.8}, WHITE, 0.5f));

	auto metaball1 = scene->AddObject(Object::MakeSphere(Vector3{2, 0, 0}, 2.5f, PINK));
	auto metaball2 = scene->AddObject(Object::MakeSphere(Vector3{-2, 0, 0}, 2.5f, GREEN));

	return scene;
}

std::unique_ptr<Scene> WazowskiScene() {
	Shader marcher = LoadShader(0, "res/shaders/march.fs");
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(SceneConfig{.clear_color = SKYBLUE, .light_zero_point = 0.01f}, marcher);

    scene->AddLight(MakeDirectionalLight(Vector3{0.5, -1, -0.8}, WHITE, 0.5f));

    Color skin {60, 148, 32};

	auto head1 = scene->AddObject(Object::MakeSphere(Vector3{0, 0, 0}, 1.5f, skin));
	auto head2 = scene->AddObject(Object::MakeSphere(Vector3{0, 0.8f, 0}, 0.75f, skin));

    auto horn1 = scene->AddObject(Object::MakeCylinder(Vector3{0.8f, 1.5f, 0}, 0.15f, 0.25f, GRAY));
    horn1->Rotate(MatrixRotateZ(-PI/8));
    auto horn2 = scene->AddObject(Object::MakeCylinder(Vector3{-0.8f, 1.5f, 0}, 0.15f, 0.25f, GRAY));
    horn2->Rotate(MatrixRotateZ(PI/8));

    horn1->combineType = CombineType::UNION;
    horn2->combineType = CombineType::UNION;

    auto eyehole = scene->AddObject(Object::MakeSphere(Vector3{0, 0.55f, 1.1f}, 0.8f, BLACK));
    eyehole->combineType = CombineType::SUBTRACT;

    auto eyemass = scene->AddObject(Object::MakeSphere(Vector3{0, 0.5f, 0.9f}, 0.7f, WHITE));
    eyemass->combineType = CombineType::UNION;

    Color irisColor {95, 211, 176};
    auto iris = scene->AddObject(Object::MakeSphere(Vector3{0, 0.5f, 1.23f}, 0.4f, irisColor));
    iris->combineType = CombineType::UNION;

    auto pupil = scene->AddObject(Object::MakeSphere(Vector3{0, 0.5f, 1.39f}, 0.25f, BLACK));
    pupil->combineType = CombineType::UNION;

    auto mouthhole = scene->AddObject(Object::MakeSphere(Vector3{0, -0.75f, 1.75f}, 0.5f, BLACK));
    mouthhole->combineType = CombineType::SUBTRACT;

    return scene;
}