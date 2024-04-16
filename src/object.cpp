#include "object.h"

std::shared_ptr<Object> Object::MakeSphere(Vector3 pos, float radius, Color color) {
	Matrix transform = MatrixTranslate(pos.x, pos.y, pos.z);
	Vector3 size = {radius, radius, radius};
	return std::make_shared<Object>(ObjectType::SPHERE, transform, size, color);
}

std::shared_ptr<Object> Object::MakeInfPlane(float y, Matrix rotation, Color color) {
	Matrix transform = MatrixMultiply(MatrixTranslate(0, y, 0), rotation);

	Vector3 size = {0, 0, 0};
	return std::make_shared<Object>(ObjectType::PLANE, transform, size, color);
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
