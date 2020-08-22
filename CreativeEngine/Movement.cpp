#include "pch.h"
#include "Movement.h"
#include <iostream>

dae::Movement::Movement()
	: m_pRefTransform()
	, m_pGameTime()
	, m_Velocity2D()
	, m_AccelerationTime(0.3f)
	, m_MaxVelocity(200.0f)
	, m_Acceleration(m_MaxVelocity / m_AccelerationTime)
	, m_IsMovingPreviousFrame()
{
}

void dae::Movement::AddMovementInput(const glm::fvec2& dir, float scale)
{
	const float deltaTime{ m_pGameTime.lock()->GetDeltaSeconds() };
	auto transform = m_pRefTransform.lock();
	if (transform)
	{
		const float accel = m_Acceleration * deltaTime;
		const float clampScale = Clamp(scale, -1.0f, 1.0f);

		m_MoveVelocity += accel;
		if (m_MoveVelocity >= m_MaxVelocity)
			m_MoveVelocity = m_MaxVelocity;
		
		m_Velocity2D = dir * clampScale * m_MoveVelocity;
		transform->SetRelativePosition({ m_Velocity2D.x * deltaTime,m_Velocity2D.y * deltaTime,0.0f });
		m_IsMovingPreviousFrame = true;
	}
}

void dae::Movement::Awake()
{
	m_pRefTransform = GetGameObject()->GetTransform();
	m_pGameTime = GetScene()->GetSceneContext().pGameTime;
}
void dae::Movement::LateUpdate()
{
	if (!m_IsMovingPreviousFrame)
	{
		m_MoveVelocity = 0.0f;
		m_Velocity2D = {};
	}
	else
		m_IsMovingPreviousFrame = false;
}
