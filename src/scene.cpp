#include "raylib.h"
#include "scene.h"

int Scene::SCN_ID = 0;

void Scene::AddObject(std::unique_ptr<Object> object) {
	m_objects.push_back(std::move(object));
}

void Scene::PopulateCache() {
	
}

void Scene::Render() const {
	
}

void Scene::Update() {
	UpdateCamera(&m_camera, CAMERA_FREE);
}