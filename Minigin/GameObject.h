#pragma once
#include "Transform.h"
#include "Component.h"
#include <unordered_map>
#include <iostream>
#include "Structs.h"

namespace engine
{
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject & other) = delete;
		GameObject(GameObject && other) = delete;
		GameObject& operator=(const GameObject & other) = delete;
		GameObject& operator=(GameObject && other) = delete;
		
		void Update();
		void Render() const;

		Float2 GetPosition();
		void SetPosition(float x, float y);
		void SetPosition(Float2 pox);
		template <typename T>
		void AddComponent(const std::weak_ptr<Component>& component);

		void Destroy() { m_NeedsDestruction = true; };
		bool NeedsDestruction() const { return m_NeedsDestruction; };
		
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
		const Transform& GetTransform();

	private:
		Transform m_Transform;
		std::vector<std::shared_ptr<Component>> m_pComponents;
		bool m_NeedsDestruction = false;
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

