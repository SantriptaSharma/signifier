#include "raylib.h"
#include "scene.h"

#include "utils.h"
#include <stdexcept>

int Scene::SCN_ID = 0;

std::shared_ptr<Object> Scene::AddObject(std::shared_ptr<Object> object) {
	if (m_objects.size() >= MAX_OBJECTS) {
		throw std::runtime_error(TextFormat("Maximum number of objects (%d) reached", MAX_OBJECTS));
		return object;
	}
	m_objects.push_back(object);
	return object;
}

std::shared_ptr<Light> Scene::AddLight(std::shared_ptr<Light> light) {
	if (m_lights.size() >= MAX_LIGHTS) {
		throw std::runtime_error(TextFormat("Maximum number of lights (%d) reached", MAX_LIGHTS));
		return light;
	}
	m_lights.push_back(light);
	return light;
}

static void AddToCache(Shader &shad, std::map<string, int> &cache, const string &name) {
	cache[name] = GetShaderLocation(shad, name.c_str());
}

void Scene::PopulateCache() {
	AddToCache(m_marcher, m_uniform_cache, "resolution");
	AddToCache(m_marcher, m_uniform_cache, "viewEye");
	AddToCache(m_marcher, m_uniform_cache, "viewCenter");
	AddToCache(m_marcher, m_uniform_cache, "objectCount");
	AddToCache(m_marcher, m_uniform_cache, "lightCount");
	AddToCache(m_marcher, m_uniform_cache, "clearColor");

	for (int i = 0; i < MAX_OBJECTS; i++) {
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].type", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].combineType", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].invTransform", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].color", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].size", i));
	}

	for (int i = 0; i < MAX_LIGHTS; i++) {
		AddToCache(m_marcher, m_uniform_cache, TextFormat("lights[%d].position", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("lights[%d].color", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("lights[%d].type", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("lights[%d].intensity", i));
	}
}

void Scene::Render() {
	this->Update();

	BeginShaderMode(m_marcher);
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
	EndShaderMode();
}

void Scene::Update() {
	UpdateCamera(&m_camera, CAMERA_FREE);

	float res[2] = {(float)GetScreenWidth(), (float)GetScreenHeight()};
	uint64_t obj_size = m_objects.size();
	uint64_t light_size = m_lights.size();

	SetShaderValue(m_marcher, m_uniform_cache.at("resolution"), &res, SHADER_UNIFORM_VEC2);
	SetShaderValue(m_marcher, m_uniform_cache.at("viewEye"), Vector3ToFloatV(m_camera.position).v, SHADER_UNIFORM_VEC3);
	SetShaderValue(m_marcher, m_uniform_cache.at("viewCenter"), Vector3ToFloatV(m_camera.target).v, SHADER_UNIFORM_VEC3);
	SetShaderValue(m_marcher, m_uniform_cache.at("objectCount"), &obj_size, SHADER_UNIFORM_INT);
	SetShaderValue(m_marcher, m_uniform_cache.at("lightCount"), &light_size, SHADER_UNIFORM_INT);

	float clear[3] = COLOR2FLOAT3(m_clear_color);
	SetShaderValue(m_marcher, m_uniform_cache.at("clearColor"), &clear, SHADER_UNIFORM_VEC3);

	for (uint64_t i = 0; i < obj_size; i++) {
		float color[3] = COLOR2FLOAT3(m_objects[i]->color);
		float size[3] = {m_objects[i]->size.x, m_objects[i]->size.y, m_objects[i]->size.z};

		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].type", i)), &m_objects[i]->type, SHADER_UNIFORM_INT);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].combineType", i)), &m_objects[i]->combineType, SHADER_UNIFORM_INT);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].color", i)), &color, SHADER_UNIFORM_VEC3);
		SetShaderValueMatrix(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].invTransform", i)), MatrixInvert(m_objects[i]->transform));
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].size", i)), &size, SHADER_UNIFORM_VEC3);
	}

	for (uint64_t i = 0; i < light_size; i++) {
		float pos[3] = {m_lights[i]->position.x, m_lights[i]->position.y, m_lights[i]->position.z};
		float color[3] = COLOR2FLOAT3(m_lights[i]->color);

		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("lights[%d].position", i)), &pos, SHADER_UNIFORM_VEC3);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("lights[%d].color", i)), &color, SHADER_UNIFORM_VEC3);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("lights[%d].type", i)), &m_lights[i]->lightType, SHADER_UNIFORM_INT);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("lights[%d].intensity", i)), &m_lights[i]->intensity, SHADER_UNIFORM_FLOAT);
	}
}