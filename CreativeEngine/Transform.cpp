#include "pch.h"
#include "Transform.h"

dae::Transform::Transform()
	: m_Position{}
	, m_Rotation{}
	, m_Scale{ 1.0f,1.0f }
{
}

void dae::Transform::SetPosition(const glm::fvec3& position)
{
	m_Position = position;
}

void dae::Transform::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::SetRotation(float x, float y, float z)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

void dae::Transform::SetRotation(const glm::fvec3& rotation)
{
	m_Rotation = rotation;
}

void dae::Transform::SetScale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}
