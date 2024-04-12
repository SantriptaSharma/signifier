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

class Object {
protected:
	ObjectType m_type;
	Matrix m_transform;
	Color m_color;

public:
	Object(ObjectType type, Matrix transform, Color color): m_type(type), m_transform(transform), m_color(color) {};
	Object(): m_type(ObjectType::SPHERE), m_transform(MatrixIdentity()), m_color(WHITE) {};
	~Object() {};
};

std::unique_ptr<Object> MakeSphere(Vector3 pos, float radius, Color color);