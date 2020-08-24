#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Movement : public BaseComponent
	{
	public:

		Movement();
		void AddMovementInput(const glm::fvec2& dir, float scale);
		const glm::fvec2& GetVelocity2D() const { return m_Velocity2D; }

		float GetMaxVelocity() const { return m_MaxVelocity; }
		void SetMaxVelocity(float value) { m_MaxVelocity = value; }
		// For projectiles
		void SetInitialVelocity(float velocity);
		void SetAcceleration(float value) { m_Acceleration = value; }
		float GetAcceleration() const { return m_Acceleration; }

	protected:

		void Awake() override;
		void LateUpdate() override;
		
	private:

		std::weak_ptr<Transform> m_pRefTransform;
		std::weak_ptr<GameTime> m_pGameTime;
		glm::fvec2 m_Velocity2D;
		glm::fvec2 m_MaxVelocity2D;
		float m_AccelerationTime;
		float m_MaxVelocity;
		float m_MoveVelocity;
		float m_Acceleration;
		bool m_IsMovingPreviousFrame;
	};
}


