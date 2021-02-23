#pragma once
#include "Transform.h"
#include "Structs.h"

namespace engine
{
	class Component
	{
	public:
		Component(const ComponentType& type);
		inline const ComponentType& GetType() const { return m_ComponentType; };

		virtual void Update() = 0;
		virtual void Render(const engine::Transform& transform) = 0;

	private:
		ComponentType m_ComponentType;
	};
}

