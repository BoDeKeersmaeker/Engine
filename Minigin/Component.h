#pragma once
#include <memory>

#include "Transform.h"

namespace engine
{
	class GameObject;
	
	class Component
	{
	public:
		Component(const std::shared_ptr<GameObject>& owner);
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
		
		virtual void Update() = 0;
		virtual void Render(const engine::Transform& transform) = 0;

		std::weak_ptr<GameObject> GetOwner() const { return m_pOwner; };
	
	protected:
		std::weak_ptr<GameObject> m_pOwner;
	};
}

