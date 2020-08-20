#pragma once
#include <array>
#include <future>

namespace dae
{
	class IInternalComponent;
	class Collider;
	class PhysicsScene
	{
	public:
		PhysicsScene();
		void RegisterCollider(std::weak_ptr<Collider>&& collider);
		void Update();
		void Render() const;
	private:
		std::vector<std::weak_ptr<Collider>> m_pColliderObjects;
		std::vector<std::future<void>> m_ColliderResolvers;
		//std::shared_ptr<Collider> m_pColliderObjects;
	};
}

