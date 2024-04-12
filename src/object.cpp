#include "object.h"

std::unique_ptr<Object> MakeSphere(Vector3 pos, float radius, Color color) {
	Matrix transform = MatrixMultiply(MatrixScale(radius, radius, radius), MatrixTranslate(pos.x, pos.y, pos.z));
	return std::make_unique<Object>(ObjectType::SPHERE, transform, color);
}