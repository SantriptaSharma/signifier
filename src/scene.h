#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <map>
#include <string>

#include "object.h"

#define MAX_OBJECTS 128

using string = std::string;

struct SceneConfig {};

class Scene {
private:
	static int SCN_ID;

	SceneConfig m_config;
	Camera m_camera;
	uint32_t m_id;
	std::vector<std::unique_ptr<Object>> m_objects;
	Shader m_marcher;
	std::map<string, int> m_uniform_cache;
	
public:
	Scene(SceneConfig config, Camera cam, Shader marcher): m_config(config), m_camera(cam), m_id(SCN_ID++), m_objects(), m_marcher(marcher), m_uniform_cache() 
		{ PopulateCache(); };
	~Scene() {
		UnloadShader(m_marcher);
	};

	void AddObject(std::unique_ptr<Object> object);
	void PopulateCache();
	void Render() const;
	void Update();

};