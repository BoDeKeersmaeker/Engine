#pragma once
#include "Transform.h"
#include "Structs.h"

namespace dae
{
	class Component
	{
	public:
		Component(const ComponentType& type);
		inline const ComponentType& GetType() const { return m_ComponentType; };

		virtual void Update() = 0;
		virtual void Render(const dae::Transform& transform) = 0;

	private:
		ComponentType m_ComponentType;
	};
}

