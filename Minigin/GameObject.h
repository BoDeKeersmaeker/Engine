#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Component.h"
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
		std::shared_ptr<Component> GetComponent(ComponentType type);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform;
		std::shared_ptr<Texture2D> m_Texture{};

		std::unordered_map<ComponentType, std::shared_ptr<Component>> m_pComponents;
	};
}
