#pragma once
#include <array>

namespace dae
{
	class Collider;
	class PhysicsScene
	{
	public:
		PhysicsScene();
		void RegisterCollider(std::weak_ptr<Collider>&& collider) { m_pColliderObjects.emplace_back(std::move(collider)); }
		void Update();
		void Render() const;
	private:
		std::vector<std::weak_ptr<Collider>> m_pColliderObjects;
		//std::shared_ptr<Collider> m_pColliderObjects;
	};
}

