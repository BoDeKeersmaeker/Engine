#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Component.h"
#include "TextureComponent.h"
#include <unordered_map>

namespace dae
{
	class Texture2D;

	class GameObject final : public SceneObject
	{
	public:
		void Update() override;
		void Render() const override;

		void SetPosition(float x, float y);
		void AddComponent(std::shared_ptr<Component> component);

		template <typename T>
		inline std::weak_ptr<T> GetComponent()
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

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform;
		std::vector<std::shared_ptr<Component>> m_pComponents;
	};
}
