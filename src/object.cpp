#include "object.h"

std::shared_ptr<Object> Object::MakeSphere(Vector3 pos, float radius, Color color) {
	Matrix transform = MatrixTranslate(pos.x, pos.y, pos.z);
	Vector3 size = {radius, radius, radius};
	return std::make_shared<Object>(ObjectType::SPHERE, transform, size, color);
}

std::shared_ptr<Object> Object::MakeInfPlane(float y, Matrix rotation, Color color) {
	Matrix transform = MatrixMultiply(rotation, MatrixTranslate(0, y, 0));

	Vector3 size = {0, 0, 0};
	return std::make_shared<Object>(ObjectType::PLANE, transform, size, color);
}

std::shared_ptr<Object> Object::MakeBox(Vector3 pos, Vector3 size, Color color) {
	Matrix transform = MatrixTranslate(pos.x, pos.y, pos.z);
	return std::make_shared<Object>(ObjectType::BOX, transform, size, color);
}

std::shared_ptr<Object> Object::MakeCylinder(Vector3 pos, float radius, float height, Color color) {
	Matrix transform = MatrixTranslate(pos.x, pos.y, pos.z);
	Vector3 size = {radius, height, radius};
	return std::make_shared<Object>(ObjectType::CYLINDER, transform, size, color);
}

BBox Object::GetBBox() {
	Vector3 min, max;
	
	switch (type) {
		case ObjectType::SPHERE:
		case ObjectType::PLANE:
		case ObjectType::BOX:
		case ObjectType::CYLINDER:
		case ObjectType::CONE:
			min = Vector3Transform(Vector3{-size.x, -size.y, -size.z}, transform);
			max = Vector3Transform(Vector3{size.x, size.y, size.z}, transform);
		break;
	}

	return BBox(min, max);
}

std::shared_ptr<Light> MakePointLight(Vector3 pos, Color color, float intensity)
{
	return std::make_shared<Light>(Light{pos, color, LightType::POINT, intensity});
}

std::shared_ptr<Light> MakeDirectionalLight(Vector3 dir, Color color, float intensity)
{
	dir = Vector3Normalize(dir);
	return std::make_shared<Light>(Light{dir, color, LightType::DIRECTIONAL, intensity});
}
