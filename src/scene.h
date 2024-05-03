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

// used to separate non-interacting objects
struct Layer {
	string name;
	std::vector<std::shared_ptr<Object>> objects;
};

class Scene {
private:
	static int SCN_ID;

	SceneConfig m_config;

	// viewport camera info
	Camera m_camera;
	float m_r, m_theta, m_phi;

	uint32_t m_id;
	std::vector<Layer> m_layers;
	std::vector<std::shared_ptr<Light>> m_lights;
	Shader m_marcher;
	std::map<string, int> m_uniform_cache;
	Color m_clear_color;
	
public:
	Scene(SceneConfig config, Shader marcher): m_config(config), m_camera(), m_r(5), m_theta(PI/2), m_phi(0), m_id(SCN_ID++), m_layers(1), 
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

	std::shared_ptr<Object> AddObject(std::shared_ptr<Object> object, uint64_t layerIndex = 0);
	std::shared_ptr<Light> AddLight(std::shared_ptr<Light> light);
	int CreateLayer(string name);
	void SetClearColor(Color clear) { m_clear_color = clear; };
	void PopulateCache();
	void Render();

private:
	void LoadLayerUniforms(uint64_t layerIndex);
	void ViewportCameraControls();
};