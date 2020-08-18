#include "pch.h"
#include "BoxCollider2D.h"


glm::fvec2 dae::BoxCollider2D::GetCenter() const
{
	//const auto objPos{ GetGameObject()->GetTransform()->GetPosition() };
	glm::fvec2 center{ glm::fvec2(m_ObjPos->x + m_Center.x,m_ObjPos->y + m_Center.y) };
	return center;
}

void dae::BoxCollider2D::Resolve(const std::vector<std::weak_ptr<Collider>>& colliders)
{
	//TODO: Only supports AABB
	for (const auto& collider : colliders)
	{
		auto boxCollider{ std::dynamic_pointer_cast<BoxCollider2D>(collider.lock()) };
		if (boxCollider)
		{
			const auto& objLeftCenter{ boxCollider->GetCenter() };
			const auto& objLeftSize{ boxCollider->GetSize() };
			const auto& objScaling{ boxCollider->GetScale() };

			const glm::fvec2 minObjLeft{ objLeftCenter - (objLeftSize / 2.0f * objScaling) };
			const glm::fvec2 maxObjLeft{ objLeftCenter + (objLeftSize / 2.0f * objScaling) };

			const auto& thisScaling{ *m_ObjScale };
			const glm::fvec2 minObjRight{ GetCenter() - (m_Size / 2.0f * thisScaling) };
			const glm::fvec2 maxObjRight{ GetCenter() + (m_Size / 2.0f * thisScaling) };

			const bool r1 = minObjRight.x < maxObjLeft.x;
			const bool r2 = maxObjRight.x > minObjLeft.x;
			const bool r3 = minObjRight.y < maxObjLeft.y;
			const bool r4 = maxObjRight.y > minObjLeft.y;

			const bool isOverlap = r1 && r2 && r3 && r4;
			if (isOverlap)
			{
				boxCollider->SetIsTrigger(isOverlap);
				boxCollider->AddCaches(GetShared<Collider>());
				AddCaches(boxCollider);
			}
		}
	}

	m_IsTriggered = !m_pCaches.empty();
}

void dae::BoxCollider2D::Resolve(const std::shared_ptr<Collider>& collider)
{
	
	//TODO: Only supports AABB
	auto boxCollider{ std::dynamic_pointer_cast<BoxCollider2D>(collider) };
	if (boxCollider)
	{
		const auto& objLeftCenter{ boxCollider->GetCenter() };
		const auto& objLeftSize{ boxCollider->GetSize() };
		const auto& objScaling{ boxCollider->GetGameObject()->GetTransform()->GetScale() };

		const glm::fvec2 minObjLeft{ objLeftCenter - (objLeftSize / 2.0f * objScaling) };
		const glm::fvec2 maxObjLeft{ objLeftCenter + (objLeftSize / 2.0f * objScaling) };

		const auto& thisScaling{ GetGameObject()->GetTransform()->GetScale() };
		const glm::fvec2 minObjRight{ GetCenter() - (m_Size / 2.0f * thisScaling) };
		const glm::fvec2 maxObjRight{ GetCenter() + (m_Size / 2.0f * thisScaling) };

		const bool r1 = minObjRight.x < maxObjLeft.x;
		const bool r2 = maxObjRight.x > minObjLeft.x;
		const bool r3 = minObjRight.y < maxObjLeft.y;
		const bool r4 = maxObjRight.y > minObjLeft.y;

		const bool isOverlap = r1 && r2 && r3 && r4;
		if (isOverlap)
			m_pCaches.emplace_back(boxCollider);
	}

	m_IsTriggered = !m_pCaches.empty();
}

//void dae::BoxCollider2D::Update()
//{
//	//const auto& ownerTransform{ GetGameObject()->GetTransform() };
//	//m_Center = ownerTransform->GetPosition();
//
//	const auto& allPhysicsObject = GetScene()->GetColliderObjects<BoxCollider2D>();
//	for (const auto& object : allPhysicsObject)
//	{
//		if (object != GetShared<BoxCollider2D>())
//		{
//			const auto& objLeftCenter{ object->GetCenter() };
//			const auto& objLeftSize{ object->GetSize() };
//			const auto& objScaling{ object->GetGameObject()->GetTransform()->GetScale() };
//
//			const glm::fvec2 minObjLeft{ objLeftCenter - (objLeftSize / 2.0f * objScaling) };
//			const glm::fvec2 maxObjLeft{ objLeftCenter + (objLeftSize / 2.0f * objScaling) };
//
//			const auto& thisScaling{ GetGameObject()->GetTransform()->GetScale() };
//			const glm::fvec2 minObjRight{ GetCenter() - (m_Size / 2.0f * thisScaling) };
//			const glm::fvec2 maxObjRight{ GetCenter() + (m_Size / 2.0f * thisScaling) };
//
//			const bool r1 = minObjRight.x < maxObjLeft.x;
//			const bool r2 = maxObjRight.x > minObjLeft.x;
//			const bool r3 = minObjRight.y < maxObjLeft.y;
//			const bool r4 = maxObjRight.y > minObjLeft.y;
//
//			m_IsTriggered = r1 && r2 && r3 && r4;
//
//			if(m_IsTriggered)
//				break;
//			
//			//float d1x = minObjRight.x - maxObjLeft.x;
//			//float d1y = minObjRight.y - maxObjLeft.y;
//			//float d2x = minObjLeft.x - maxObjRight.x;
//			//float d2y = minObjLeft.y - maxObjRight.y;
//
//			//if (d1x > 0.0f || d1y > 0.0f)
//			//{
//			//	m_IsTriggered = false;
//			//	continue;
//			//}
//			//if (d2x > 0.0f || d2y > 0.0f)
//			//{
//			//	m_IsTriggered = false;
//			//	continue;
//			//}
//
//			//m_IsTriggered = true;
//			//break;
//		}
//	}
//}

void dae::BoxCollider2D::Render() const
{
	if (IsEnableDebugDraw())
	{
		auto objPos{ GetGameObject()->GetTransform()->GetPosition() };
		const auto& objScaling{ GetGameObject()->GetTransform()->GetScale() };
		objPos.x += m_Center.x;
		objPos.y += m_Center.y;

		glm::fvec4 color = m_IsTriggered ? glm::fvec4{ 1.0f,0.0f,0.0f,1.0f } : glm::fvec4{ 0.0f,1.0f,0.0f,1.0f };

		Debug::DrawRect(objPos, m_Size * objScaling, color);
	}
}
