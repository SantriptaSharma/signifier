#pragma once
#include <memory>

#include "raylib.h"
#include "raymath.h"

struct BBox {
	Vector3 min, max;

	BBox(Vector3 min, Vector3 max): min(min), max(max) {};
	BBox(): min(Vector3Zero()), max(Vector3Zero()) {};
	
	BBox operator+(const BBox &other) {
		return BBox(Vector3Min(min, other.min), Vector3Max(max, other.max));
	}
};

enum class ObjectType {
	SPHERE,
	PLANE,
	BOX,
	CYLINDER,
	CONE
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

	void Rotate(Matrix rotation) {
		transform = MatrixMultiply(rotation, transform);
	}

	static std::shared_ptr<Object> MakeSphere(Vector3 pos, float radius, Color color);
	static std::shared_ptr<Object> MakeInfPlane(float y, Matrix rotation, Color color);
	static std::shared_ptr<Object> MakeBox(Vector3 pos, Vector3 size, Color color);
	static std::shared_ptr<Object> MakeCylinder(Vector3 pos, float radius, float height, Color color);

	BBox GetBBox();
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