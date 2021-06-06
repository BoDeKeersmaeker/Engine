#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

using namespace engine;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object, size_t priority)
{
	m_Objects.push_back({ object, priority });
	m_NeedsSorting = true;
}

void Scene::Update()
{
	for(auto& object : m_Objects)
		object.first->Update();

	m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(), [](const std::pair<std::shared_ptr<GameObject>, size_t>& object) { return object.first->NeedsDestruction(); }), m_Objects.end());

	if (m_NeedsSorting)
	{
		std::sort(m_Objects.begin(), m_Objects.end(), [](const std::pair<std::shared_ptr<GameObject>, size_t>& object1, const std::pair<std::shared_ptr<GameObject>, size_t>& object2)
			{
				return object1.second > object2.second;
			});
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
		if (object.first)
			object.first->Render();
}