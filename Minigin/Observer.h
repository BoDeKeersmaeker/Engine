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
		Observer() = default;
		virtual ~Observer() =default;
		
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;
		
		virtual void OnNotify(Event event, const std::weak_ptr<GameObject>& gameObject) = 0;
		virtual void OnNotify(Event event, int number) = 0;
	};
}
