#include "pch.h"
#include "PhysicsScene.h"
#include "Collider.h"
#include <future>


dae::PhysicsScene::PhysicsScene()
	: m_pColliderObjects{}
{
}

void dae::PhysicsScene::Update()
{

	std::for_each(m_pColliderObjects.begin(), m_pColliderObjects.end(), [](const std::weak_ptr<Collider>& other)
		{
			other.lock()->ClearCaches();
		});
	
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
		

		auto future = std::async(std::launch::async, [&pOtherColliders](const std::shared_ptr<Collider>& collider)
			{
				collider->Resolve(pOtherColliders);
			}, object.lock());
	}
}

void dae::PhysicsScene::Render() const
{
	for (const auto& object : m_pColliderObjects)
	{
		std::static_pointer_cast<IInternalComponent>(object.lock())->Render();
	}
}
