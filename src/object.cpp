#include "object.h"

std::unique_ptr<Object> MakeSphere(Vector3 pos, float radius, Color color) {
	Matrix transform = MatrixTranslate(pos.x, pos.y, pos.z);
	Vector3 size = {radius, radius, radius};
	return std::make_unique<Object>(ObjectType::SPHERE, transform, size, color);
}

std::unique_ptr<Object> MakeInfPlane(float y, Matrix rotation, Color color) {
	Matrix transform = MatrixMultiply(MatrixTranslate(0, y, 0), rotation);

	Vector3 size = {0, 0, 0};
	return std::make_unique<Object>(ObjectType::PLANE, transform, size, color);
}