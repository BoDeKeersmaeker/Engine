#pragma once

namespace engine
{
	class GameObject;
	
	enum class Event
	{
		PlayerDied
	};
	
	class Observer
	{
	public:
		virtual ~Observer() =default;
		virtual void OnNotify(std::weak_ptr<GameObject> gameObject, Event event) = 0;
	};
}