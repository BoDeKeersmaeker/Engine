#pragma once
#include "BaseCommand.h"
namespace engine
{
	class GameObject;
	
	class Kill : public engine::BaseCommand
	{
	public:
		Kill(std::weak_ptr<GameObject> target);
		virtual ~Kill() = default;
		void Execute() override;

	private:
		std::weak_ptr<GameObject> m_pTarget;
	};
}

