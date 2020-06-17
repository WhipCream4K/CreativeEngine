#include "pch.h"
#include "CharacterController.h"

dae::CharacterController::CharacterController()
	: m_MaxSpeed{}
{
	m_MaxSpeed = 50.0f;
}

void dae::CharacterController::AddMovementInput(const glm::fvec3& direction,float scale)
{
	AddWorldMovementInput(glm::normalize(direction), Clamp(scale,-1.0f,1.0f),false);
}

void dae::CharacterController::AddWorldMovementInput(const glm::fvec3& direction, float scale, bool frameDependent)
{
	auto& transform{ GetTransform() };
	float deltaTime = frameDependent ? 1.0f : GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds();
	m_Velocity = direction * scale * deltaTime * m_MaxSpeed;
	transform.SetRelativePosition(m_Velocity);
}

