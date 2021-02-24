#pragma once

namespace engine
{
	enum class Event;
	class GameObject;
	class Observer;
	
	class SceneObject
	{
	public:
		virtual void Update() = 0;
		virtual void Render() const = 0;

		SceneObject() = default;
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;

		virtual void AddObserver(std::shared_ptr<Observer> observer);
		virtual void RemoveObserver(std::shared_ptr<Observer> observer);

	protected:
		void Notify(const GameObject& entity, Event event);

	private:
		std::vector<std::shared_ptr<Observer>> m_pObservers;
	};
}
