#pragma once

class GameObject;

namespace engine
{
	enum class Event
	{
		GameObjectDestroyed
	};
	
	class Observer
	{
	public:
		virtual ~Observer() {};
		virtual  void OnNotify(const GameObject& gameObject, Event event) = 0;
	};
}