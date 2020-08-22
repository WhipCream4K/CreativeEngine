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

void dae::PhysicsScene::ClearObjers()
{
	auto it = std::remove_if(m_pColliderObjects.begin(), m_pColliderObjects.end(), [](const std::weak_ptr<Collider>& collider)
		{
			return collider.expired();
		});

	m_pColliderObjects.erase(it,m_pColliderObjects.end());
}

bool dae::PhysicsScene::IsResultValid() const
{
	bool result{ true };
	if (!m_ColliderResolvers.empty())
	{
		result = std::all_of(m_ColliderResolvers.begin(), m_ColliderResolvers.end(), [](const std::future<void>& resolver)
			{
				return resolver.valid();
			});
	}

	return result;
}

void dae::PhysicsScene::Update()
{
	// prevent the start of new worker thread
	bool isAllAnswerValid{};
	if (!m_ColliderResolvers.empty())
	{
		isAllAnswerValid = std::all_of(m_ColliderResolvers.begin(), m_ColliderResolvers.end(), [](const std::future<void>& resolver)
			{
				return resolver.valid();
			});
		
		// go to next frame
		if (!isAllAnswerValid)
			return;

		for (const auto& triggered : m_pColliderObjects)
		{
			triggered.lock()->OnBeginOverlap();
			triggered.lock()->ClearCaches();
		}

		m_ColliderResolvers.clear();
		return;
	}

	// Bad practice
	{
		std::vector<std::weak_ptr<Collider>> pCaches{};
		for (const auto& object : m_pColliderObjects)
		{
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
				std::async(std::launch::async, [](const std::shared_ptr<Collider>& collider, std::vector<std::weak_ptr<Collider>>&& otherColliders)
					{
						collider->Resolve(std::move(otherColliders));
					}, object.lock(), pOtherColliders)
			);

		}
	}

}

void dae::PhysicsScene::Render() const
{
}
