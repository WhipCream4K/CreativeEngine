#pragma once
#include "GameObject.h"
#include "InputComponent.h"

namespace dae
{
	class InputComponent;
	class CharacterController : public GameObject
	{
	public:

		CharacterController();

		constexpr auto GetMaxSpeed() const noexcept -> float { return m_MaxSpeed; }
		constexpr auto SetMaxSpeed(float speed) noexcept -> void { m_MaxSpeed = speed; }

	protected:

		std::shared_ptr<InputComponent> m_pInputComponent;

		virtual void MoveHorizontal(float) {}
		virtual void MoveVertical(float) {}
		
		void AddMovementInput(const glm::fvec3& direction, float scale);
		constexpr auto GetVelocity() const noexcept -> const glm::fvec3& { return m_Velocity; }


	private:

		glm::fvec3 m_Velocity;
		float m_MaxSpeed;
		void AddWorldMovementInput(const glm::fvec3& direction, float scale, bool frameDependent = false);
	};
}

