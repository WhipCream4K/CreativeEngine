#include "pch.h"
#include "PhysicsScene.h"
#include "Collider.h"


dae::PhysicsScene::PhysicsScene()
	: m_pColliderObjects{}
{
}

void dae::PhysicsScene::RegisterCollider(std::weak_ptr<Collider>&& collider)
{
	m_pColliderObjects.emplace_back(std::move(collider));
}

void dae::PhysicsScene::Update()
{
	std::for_each(m_pColliderObjects.begin(), m_pColliderObjects.end(), [](const std::weak_ptr<Collider>& other)
		{
			other.lock()->ClearCaches();
		});

	// Bad practice
	{
		std::vector<std::weak_ptr<Collider>> pCaches{};
		for (const auto& object : m_pColliderObjects)
		{
			// prevent the start of new worker thread
			bool isAllAnswerValid{};
			if (m_ColliderResolvers.size() == m_pColliderObjects.size() && !m_ColliderResolvers.empty())
			{
				isAllAnswerValid = std::all_of(m_ColliderResolvers.begin(), m_ColliderResolvers.end(), [](const std::future<void>& resolver)
					{
						return resolver.valid();
					});

				if (!isAllAnswerValid)
					continue;

				for (const auto& triggered : m_pColliderObjects)
				{
					triggered.lock()->OnBeginOverlap();
				}

				m_ColliderResolvers.clear();
			}
			
			pCaches.emplace_back(object);
			std::vector<std::weak_ptr<Collider>> pOtherColliders{};
			pOtherColliders.reserve(m_pColliderObjects.size() - pCaches.size());

			for (uint32_t i = 0; i < m_pColliderObjects.size(); ++i)
			{
				const auto& collider = m_pColliderObjects[i];
				if (collider.lock() != object.lock())
				{
					if (i >= pCaches.size())
						pOtherColliders.emplace_back(collider);
					else
					{
						if (collider.lock() != pCaches[i].lock())
						{
							pOtherColliders.emplace_back(collider);
						}
					}
				}
			}


			m_ColliderResolvers.emplace_back(
				std::async(std::launch::async, [](const std::shared_ptr<Collider>& collider,std::vector<std::weak_ptr<Collider>>&& otherColliders)
					{
						collider->Resolve(std::move(otherColliders));
					}, object.lock(),pOtherColliders)
			);

		}
	}

}

void dae::PhysicsScene::Render() const
{
}
