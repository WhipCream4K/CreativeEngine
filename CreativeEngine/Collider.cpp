#include "pch.h"
#include "Collider.h"

dae::Collider::Collider()
	: m_IsATrigger{}
	, m_IsTriggered{}
	, m_IsEnableDebugDraw{ true }
{
}

void dae::Collider::AddCaches(std::weak_ptr<Collider>&& collider)
{
	auto it = std::find_if(m_pCaches.begin(), m_pCaches.end(), [&collider](const std::weak_ptr<Collider>& other)
	{
			return other.lock() == collider.lock();
	});
	
	if (it == m_pCaches.end())
		m_pCaches.emplace_back(std::move(collider));
}

void dae::Collider::ClearCaches()
{
	if (!m_IsTriggered)
		m_pCaches.clear();
}

void dae::Collider::OnBeginOverlap()
{
	if(m_IsTriggered)
	{	
		GetGameObject()->OnBeginOverlap(m_pCaches);
		m_pCaches.clear();
	}
}

void dae::Collider::Awake()
{
	const auto& transform = GetGameObject()->GetTransform();
	m_ObjPos = &transform->GetPosition();
	m_ObjScale = &transform->GetScale();
}
