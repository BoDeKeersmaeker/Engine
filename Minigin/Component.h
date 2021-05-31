#pragma once
#include <memory>

#include "Transform.h"

namespace engine
{
	class GameObject;
	
	class Component
	{
	public:
		virtual ~Component() = default;
		Component(std::shared_ptr<GameObject> owner);

		std::weak_ptr<GameObject> GetOwner() const { return m_pOwner; };
		
		virtual void Update() = 0;
		virtual void Render(const engine::Transform& transform) = 0;

	protected:
		std::weak_ptr<GameObject> m_pOwner;
	};
}

