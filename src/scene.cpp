#include "raylib.h"
#include "scene.h"

#include "utils.h"
#include <stdexcept>

int Scene::SCN_ID = 0;

std::shared_ptr<Object> Scene::AddObject(std::shared_ptr<Object> object, uint64_t layerIndex) {
	Layer &l = m_layers[layerIndex];
	if (l.objects.size() >= MAX_OBJECTS) {
		throw std::runtime_error(TextFormat("Maximum number of objects (%d) reached", MAX_OBJECTS));
		return object;
	}
	l.objects.push_back(object);
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

int Scene::CreateLayer(string name) {
	Layer l;
	l.name = name;
	m_layers.push_back(l);
	return m_layers.size() - 1;
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
	ViewportCameraControls();

	uint64_t layers = m_layers.size();

	BeginTextureMode(m_render_texture);
		ClearBackground(m_clear_color);
	EndTextureMode();

	for (uint64_t i = 0; i < layers; i++) {
		this->LoadLayerUniforms(i);

		BeginTextureMode(m_render_texture);
			BeginShaderMode(m_marcher);
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
			EndShaderMode();
		EndTextureMode();
	}

	DrawTextureRec(m_render_texture.texture, {0, 0, (float)m_render_texture.texture.width, -(float)m_render_texture.texture.height}, {0, 0}, WHITE);
}

void Scene::WindowResized() {
	UnloadRenderTexture(m_render_texture);
	m_render_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}

// Manages the movement of the camera in the viewport
void Scene::ViewportCameraControls() {
	Vector3 fwd = Vector3Normalize(Vector3Subtract(m_camera.target, m_camera.position));
	// right handed system
	Vector3 right = Vector3Normalize(Vector3CrossProduct(fwd, m_camera.up));
	Vector3 up = Vector3Normalize(Vector3CrossProduct(right, fwd));

	Vector2 mmove = GetMouseDelta();
	float dt = GetFrameTime();

	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
		m_theta -= mmove.x * dt;
		m_phi -= mmove.y * dt;
	}

	Vector3 delta = {0, 0, 0};	
	float scroll = GetMouseWheelMove() * 5;

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		delta = Vector3Add(Vector3Add(Vector3Scale(right, -mmove.x), Vector3Scale(up, mmove.y)), delta);
	}

	delta = Vector3Add(Vector3Scale(fwd, scroll), delta);
	delta = Vector3Scale(delta, dt * 2); // speed factor
	m_camera.position = Vector3Add(m_camera.position, delta);
	m_camera.target = Vector3Add(m_camera.target, delta);

	m_theta = fmod(m_theta, 2 * PI);
	m_phi = Clamp(m_phi, -PI/2 + 0.1, PI/2 - 0.1);

	// calculate new position
	m_camera.position.x = -m_r * sin(m_theta) * cos(m_phi) + m_camera.target.x;
	m_camera.position.y = -m_r * sin(m_phi) + m_camera.target.y;
	m_camera.position.z = -m_r * cos(m_theta) * cos(m_phi) + m_camera.target.z;
}

void Scene::LoadLayerUniforms(uint64_t layerIndex) {
	float res[2] = {(float)GetScreenWidth(), (float)GetScreenHeight()};

	Layer &l = m_layers[layerIndex];

	uint64_t obj_size = l.objects.size();
	uint64_t light_size = m_lights.size();

	SetShaderValue(m_marcher, m_uniform_cache.at("resolution"), &res, SHADER_UNIFORM_VEC2);
	SetShaderValue(m_marcher, m_uniform_cache.at("viewEye"), Vector3ToFloatV(m_camera.position).v, SHADER_UNIFORM_VEC3);
	SetShaderValue(m_marcher, m_uniform_cache.at("viewCenter"), Vector3ToFloatV(m_camera.target).v, SHADER_UNIFORM_VEC3);
	SetShaderValue(m_marcher, m_uniform_cache.at("objectCount"), &obj_size, SHADER_UNIFORM_INT);
	SetShaderValue(m_marcher, m_uniform_cache.at("lightCount"), &light_size, SHADER_UNIFORM_INT);

	float clear[3] = COLOR2FLOAT3(m_clear_color);
	SetShaderValue(m_marcher, m_uniform_cache.at("clearColor"), clear, SHADER_UNIFORM_VEC3);

	for (uint64_t i = 0; i < obj_size; i++) {
		float color[3] = COLOR2FLOAT3(l.objects[i]->color);
		float size[3] = {l.objects[i]->size.x, l.objects[i]->size.y, l.objects[i]->size.z};

		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].type", i)), &l.objects[i]->type, SHADER_UNIFORM_INT);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].combineType", i)), &l.objects[i]->combineType, SHADER_UNIFORM_INT);
		SetShaderValue(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].color", i)), &color, SHADER_UNIFORM_VEC3);
		SetShaderValueMatrix(m_marcher, m_uniform_cache.at(TextFormat("objects[%d].invTransform", i)), MatrixInvert(l.objects[i]->transform));
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