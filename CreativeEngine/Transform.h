#pragma once
#include "BaseComponent.h"

//#include <glm.hpp>

namespace dae
{
	class Transform : public BaseComponent
	{
	public:

		Transform();
		const glm::fvec3& GetPosition() const { return m_Position; }
		const glm::fvec3& GetRotation() const { return m_Rotation; }
		const glm::fvec2& GetScale() const { return m_Scale; }

		void SetPosition(const glm::fvec3& position);
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetRotation(const glm::fvec3& rotation);
		void SetScale(const glm::fvec2& scale) { m_Scale = scale; }
		void SetScale(float x, float y);

	private:

		glm::fvec3 m_Position;
		glm::fvec3 m_Rotation; // rotation in x,y,z axis

		// for now there's only scaling on x and y because scaling on z axis doesn't make sense at the moment
		glm::fvec2 m_Scale;
	};
}



