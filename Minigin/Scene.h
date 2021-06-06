#pragma once
#include "GameObject.h"
#include "SceneManager.h"


namespace engine
{
	class Scene
	{
		friend std::weak_ptr<Scene> SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object, size_t priority = 0);
		
		void Update();
		void Render() const;
		
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);
		
		std::string m_Name;
		std::vector <std::pair<std::shared_ptr<GameObject>, size_t>> m_Objects{};

		static unsigned int m_IdCounter;
		bool m_NeedsSorting = false;
	};

}
