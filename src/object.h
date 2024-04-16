#pragma once
#include <memory>

#include "raylib.h"
#include "raymath.h"

enum class ObjectType {
	SPHERE,
	PLANE,
	CUBE,
	CONE,
	CYLINDER
};

enum class CombineType {
	UNION,
	SOFTUNION,
	SUBTRACT,
	INTERSECT
};

struct Object {
	ObjectType type;
	CombineType combineType;
	Matrix transform;
	Vector3 size;
	Color color;

	Object(ObjectType type, CombineType combineType, Matrix transform, Vector3 size, Color color): type(type), combineType(combineType),  transform(transform), size(size), color(color) {};
	Object(ObjectType type, Matrix transform, Vector3 size, Color color): type(type), combineType(CombineType::SOFTUNION), 
				transform(transform), size(size), color(color) {};
	Object(): 	type(ObjectType::SPHERE), combineType(CombineType::SOFTUNION), transform(MatrixIdentity()), size(Vector3One()), color(WHITE) {};
	~Object() {};

	static std::shared_ptr<Object> MakeSphere(Vector3 pos, float radius, Color color);
	static std::shared_ptr<Object> MakeInfPlane(float y, Matrix rotation, Color color);
};

enum class LightType {
	POINT,
	DIRECTIONAL
};

struct Light {
	Vector3 position;
	Color color;
	LightType lightType;
	float intensity;
};

std::shared_ptr<Light> MakePointLight(Vector3 pos, Color color, float intensity);
std::shared_ptr<Light> MakeDirectionalLight(Vector3 dir, Color color, float intensity);