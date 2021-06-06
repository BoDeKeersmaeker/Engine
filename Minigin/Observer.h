#pragma once
#include <memory>

namespace engine
{
	class GameObject;
	
	enum class Event
	{
		PlayerDied,
		ScoreChanged
	};
	
	class Observer
	{
	public:
		virtual ~Observer() =default;
		virtual void OnNotify(Event event, std::weak_ptr<GameObject> gameObject) = 0;
		virtual void OnNotify(Event event, int number) = 0;
	};
}
