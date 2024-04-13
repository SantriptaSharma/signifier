#pragma once
#include <memory>

#include "raylib.h"
#include "raymath.h"

enum class ObjectType {
	SPHERE,
	PLANE,
	CUBE,
	CONE,
	CYLINDER,
};

struct Object {
	ObjectType type;
	Matrix transform;
	Color color;

	Object(ObjectType type, Matrix transform, Color color): type(type), transform(transform), color(color) {};
	Object(): type(ObjectType::SPHERE), transform(MatrixIdentity()), color(WHITE) {};
	~Object() {};
};

std::unique_ptr<Object> MakeSphere(Vector3 pos, float radius, Color color);