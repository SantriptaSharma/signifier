#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <map>
#include <string>

#include "rlgl.h"

#include "object.h"

#define MAX_OBJECTS 128
#define MAX_LIGHTS 8

using string = std::string;

struct SceneConfig {
	Color clear_color;
	float light_zero_point;
};

struct LayerConfig {
	Color specColor;
	float specStrength;
};

#define DEFAULT_LAYER_CONFIG LayerConfig{.specColor = WHITE, .specStrength = 80}

// used to separate non-interacting objects
struct Layer {
	string name;
	std::vector<std::shared_ptr<Object>> objects;
	LayerConfig config;
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

	RenderTexture2D m_render_texture;
	
public:
	Scene(SceneConfig config, Shader marcher): m_config(config), m_camera(), m_r(0.5f), m_theta(PI/2), m_phi(0), m_id(SCN_ID++), m_layers(1), 
	m_marcher(marcher), m_uniform_cache() { 
		rlEnableDepthMask();
		rlEnableDepthTest();

		m_camera = {0};
    	m_camera.position = Vector3{0, 0, m_r};
    	m_camera.target = Vector3{0, 0, 0};
    	m_camera.up = Vector3{0, 1, 0};
    	m_camera.projection = CAMERA_PERSPECTIVE;; 
		
		m_render_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

		PopulateCache();
	};

	~Scene() {
		UnloadShader(m_marcher);
		UnloadRenderTexture(m_render_texture);
	};

	std::shared_ptr<Object> AddObject(std::shared_ptr<Object> object, uint64_t layerIndex = 0);
	std::shared_ptr<Light> AddLight(std::shared_ptr<Light> light);
	uint64_t CreateLayer(string name);

	// no bounds checking for layers
	void SetLayerConfig(LayerConfig config, uint64_t layerIndex = 0) { m_layers[layerIndex].config = config; }
	LayerConfig GetLayerConfig(uint64_t layerIndex = 0) { return m_layers[layerIndex].config; }
	
	void PopulateCache();
	void Render();
	void WindowResized();

private:
	void LoadLayerUniforms(uint64_t layerIndex);
	void ViewportCameraControls();
};