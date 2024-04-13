#include "raylib.h"
#include "scene.h"

int Scene::SCN_ID = 0;

void Scene::AddObject(std::unique_ptr<Object> object) {
	m_objects.push_back(std::move(object));
}

static void AddToCache(Shader &shad, std::map<string, int> &cache, const string &name) {
	cache[name] = GetShaderLocation(shad, name.c_str());
}

void Scene::PopulateCache() {
	AddToCache(m_marcher, m_uniform_cache, "resolution");
	AddToCache(m_marcher, m_uniform_cache, "viewEye");
	AddToCache(m_marcher, m_uniform_cache, "viewCenter");
	AddToCache(m_marcher, m_uniform_cache, "objectCount");

	for (int i = 0; i < MAX_OBJECTS; i++) {
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].type", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].transform", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].color", i));
	}
}

void Scene::Render() const {
	BeginShaderMode(m_marcher);
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
	EndShaderMode();
}

void Scene::Update() {
	UpdateCamera(&m_camera, CAMERA_FREE);

	float res[2] = {(float)GetScreenWidth(), (float)GetScreenHeight()};
	uint64_t size = m_objects.size();

	SetShaderValue(m_marcher, m_uniform_cache.at("resolution"), &res, SHADER_UNIFORM_VEC2);
	SetShaderValue(m_marcher, m_uniform_cache.at("viewEye"), Vector3ToFloatV(m_camera.position).v, SHADER_UNIFORM_VEC3);
	SetShaderValue(m_marcher, m_uniform_cache.at("viewCenter"), Vector3ToFloatV(m_camera.target).v, SHADER_UNIFORM_VEC3);
	SetShaderValue(m_marcher, m_uniform_cache.at("objectCount"), &size, SHADER_UNIFORM_INT);

	for (uint64_t i = 0; i < size; i++) {
		float color[3] = {m_objects[i]->color.r, m_objects[i]->color.g, m_objects[i]->color.b};

		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].type", i)), &m_objects[i]->type, SHADER_UNIFORM_INT);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].color", i)), &color, SHADER_UNIFORM_VEC3);
		SetShaderValueMatrix(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].transform", i)), m_objects[i]->transform);
	}
}