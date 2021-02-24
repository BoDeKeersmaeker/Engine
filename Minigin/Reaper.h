#pragma once
#include "Observer.h"

namespace engine
{
	class Reaper final : public Observer
	{
	public:
		Reaper();
		virtual void OnNotify(const GameObject& gameObject, Event event) override;

	private:
		
	};
}

