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

	// viewport camera info
	Camera m_camera;
	float m_r, m_theta, m_phi;

	uint32_t m_id;
	std::vector<std::shared_ptr<Object>> m_objects;
	std::vector<std::shared_ptr<Light>> m_lights;
	Shader m_marcher;
	std::map<string, int> m_uniform_cache;
	Color m_clear_color;
	
public:
	Scene(SceneConfig config, Shader marcher): m_config(config), m_camera(), m_r(10), m_theta(PI/2), m_phi(0), m_id(SCN_ID++), m_objects(), 
	m_marcher(marcher), m_uniform_cache() { 
		m_camera = {0};
    	m_camera.position = Vector3{0, 0, m_r};
    	m_camera.target = Vector3{0, 0, 0};
    	m_camera.up = Vector3{0, 1, 0};
    	m_camera.projection = CAMERA_PERSPECTIVE;; 
		
		PopulateCache();
		m_clear_color = BLACK;
	};

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
	void ViewportCameraControls();
};