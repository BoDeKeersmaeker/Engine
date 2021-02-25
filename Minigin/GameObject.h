#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Component.h"
#include <unordered_map>

namespace engine
{
	class Texture2D;

	class GameObject final : public SceneObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject & other) = delete;
		GameObject(GameObject && other) = delete;
		GameObject& operator=(const GameObject & other) = delete;
		GameObject& operator=(GameObject && other) = delete;
		
		void Update() override;
		void Render() const override;

		void SetPosition(float x, float y);
		template <typename T>
		void AddComponent(const std::weak_ptr<Component>& component);

		template <typename T>
		std::weak_ptr<T> GetComponent()
		{
			std::weak_ptr<T> temp;
			for (std::shared_ptr<Component> component : m_pComponents)
			{
				temp = std::dynamic_pointer_cast<T>(component);
				if (temp.lock() != nullptr)
					return temp;
			}
			return temp;
		}

	private:
		Transform m_Transform;
		std::vector<std::shared_ptr<Component>> m_pComponents;
	};

	template <typename T>
	void GameObject::AddComponent(const std::weak_ptr<Component>& component)
	{
		if (component.expired())
			std::cout << "Component is expired!\n";
		else if (component.use_count() != 1)
			std::cout << "Component is already added to another gameObject!\n";
		else if (GetComponent<T>().lock() != nullptr)
			std::cout << "Component of the same type already added!\n";
		else
			m_pComponents.push_back(component.lock());
	}
}

