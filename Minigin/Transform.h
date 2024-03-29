#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include "../3rdParty/glm/glm/glm.hpp"
#pragma warning(pop)

namespace engine
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_Position;
	};
}
