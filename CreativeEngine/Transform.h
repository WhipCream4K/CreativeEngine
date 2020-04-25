#pragma once
#include "BaseComponent.h"

#include <glm.hpp>

namespace dae
{
	class Transform : public BaseComponent
	{
	public:

		const glm::fvec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::fvec3& position);
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetRotation(const glm::fvec3& rotation);

		void Start() override {}
		void Update() override {}

	private:

		glm::fvec3 m_Position;
		glm::fvec3 m_Rotation; // rotation in x,y,z axis
	};
}

