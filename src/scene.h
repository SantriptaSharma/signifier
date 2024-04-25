#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <map>
#include <string>

#include "object.h"

#define MAX_OBJECTS 128
#define MAX_LIGHTS 8

using string = std::string;

struct SceneConfig {};

class Scene {
private:
	static int SCN_ID;

	SceneConfig m_config;
	Camera m_camera;
	uint32_t m_id;
	std::vector<std::shared_ptr<Object>> m_objects;
	std::vector<std::shared_ptr<Light>> m_lights;
	Shader m_marcher;
	std::map<string, int> m_uniform_cache;
	Color m_clear_color;
	
public:
	Scene(SceneConfig config, Camera cam, Shader marcher): m_config(config), m_camera(cam), m_id(SCN_ID++), m_objects(), m_marcher(marcher), m_uniform_cache() 
		{ PopulateCache(); m_clear_color = BLACK;};
	~Scene() {
		UnloadShader(m_marcher);
	};

	std::shared_ptr<Object> AddObject(std::shared_ptr<Object> object);
	std::shared_ptr<Light> AddLight(std::shared_ptr<Light> light);
	void SetClearColor(Color clear) { m_clear_color = clear; };
	void PopulateCache();
	void Render();

private:
	void Update();
};