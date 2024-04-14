#include "object.h"

std::unique_ptr<Object> MakeSphere(Vector3 pos, float radius, Color color) {
	Matrix transform = MatrixTranslate(pos.x, pos.y, pos.z);
	Vector3 size = {radius, radius, radius};
	return std::make_unique<Object>(ObjectType::SPHERE, transform, size, color);
}