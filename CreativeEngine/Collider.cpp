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
	if (!collider.expired())
	{
		for (const auto& thisCollider : m_pCaches)
		{
			if (thisCollider.lock() == collider.lock())
				return;
		}

		m_pCaches.emplace_back(std::move(collider));
		//const auto& refCaches{ m_pCaches };
		//auto it = std::find_if(refCaches.cbegin(), refCaches.cend(), [&collider](const std::weak_ptr<Collider>& thisCollider)
		//	{
		//		if (thisCollider.expired())
		//			return false;
		//		return collider.lock() == thisCollider.lock();
		//	});

		//if (it == refCaches.cend())
		//	m_pCaches.emplace_back(std::move(collider));
	}
	
}

void dae::Collider::ClearCaches()
{
	if (m_IsTriggered)
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

void dae::Collider::Start()
{
	const auto& transform = GetGameObject()->GetTransform();
	m_ObjPos = &transform->GetPosition();
	m_ObjScale = &transform->GetScale();
}
