#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void engine::SceneManager::Update()
{
	m_pActiveScene.lock()->Update();
	//for(auto& scene : m_pScenes)
	//{
	//	scene->Update();
	//}
}

void engine::SceneManager::Render()
{
	m_pActiveScene.lock()->Render();
	//for (const auto& scene : m_pScenes)
	//{
	//	scene->Render();
	//}
}

void engine::SceneManager::SetActiveScene(const std::string& name)
{
	m_pActiveScene = m_pScenes[name];
}

std::weak_ptr<engine::Scene> engine::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_pScenes[name] = scene;
	if (m_pActiveScene.expired())
		m_pActiveScene = scene;
	return scene;
}
