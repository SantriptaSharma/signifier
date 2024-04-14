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
	Vector3 size;
	Color color;

	Object(ObjectType type, Matrix transform, Vector3 size, Color color): type(type), transform(transform), size(size), color(color) {};
	Object(): type(ObjectType::SPHERE), transform(MatrixIdentity()), size(Vector3One()), color(WHITE) {};
	~Object() {};
};

std::unique_ptr<Object> MakeSphere(Vector3 pos, float radius, Color color);