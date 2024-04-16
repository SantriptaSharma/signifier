#include "raylib.h"
#include "scene.h"

#include "utils.h"

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
	AddToCache(m_marcher, m_uniform_cache, "clearColor");

	for (int i = 0; i < MAX_OBJECTS; i++) {
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].type", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].invTransform", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].color", i));
		AddToCache(m_marcher, m_uniform_cache, TextFormat("objects[%d].size", i));
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

	float clear[3] = COLOR2FLOAT3(m_clear_color);
	SetShaderValue(m_marcher, m_uniform_cache.at("clearColor"), &clear, SHADER_UNIFORM_VEC3);

	// Object Update Time (hardcoded for demoscene for now)
	float r = 1.8f + sin(GetTime()) * 0.6;
	float u = r * cos(GetTime());
	float v = r * sin(GetTime());
	m_objects[1]->transform = MatrixTranslate(u, 0, v);


	for (uint64_t i = 0; i < size; i++) {
		float color[3] = COLOR2FLOAT3(m_objects[i]->color);
		float size[3] = {m_objects[i]->size.x, m_objects[i]->size.y, m_objects[i]->size.z};

		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].type", i)), &m_objects[i]->type, SHADER_UNIFORM_INT);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].color", i)), &color, SHADER_UNIFORM_VEC3);
		SetShaderValueMatrix(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].invTransform", i)), MatrixInvert(m_objects[i]->transform));
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].size", i)), &size, SHADER_UNIFORM_VEC3);
	}
}