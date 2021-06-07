#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Singleton.h"

namespace engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		std::weak_ptr<Scene> CreateScene(const std::string& name);

		void Update();
		void Render();
		void SetActiveScene(const std::string& name);
	
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::map<std::string, std::shared_ptr<Scene>> m_pScenes;
		std::weak_ptr<Scene> m_pActiveScene = std::shared_ptr<Scene>(nullptr);
	};
}