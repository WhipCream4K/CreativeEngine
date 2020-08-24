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
		void ClearObjers();
		bool IsResultValid() const;
		void Update();
		void Render() const;
		void SetCallForDelete(bool value) { m_CallForDelete = value; }
	private:
		std::vector<std::weak_ptr<Collider>> m_pColliderObjects;
		std::vector<std::future<void>> m_ColliderResolvers;
		bool m_CallForDelete;
		//std::shared_ptr<Collider> m_pColliderObjects;
	};
}

