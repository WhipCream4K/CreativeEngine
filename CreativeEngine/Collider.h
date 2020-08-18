#pragma once
#include "PhysicsComponent.h"
#include "BaseComponent.h"

namespace dae
{
	class Collider : public BaseComponent
	{
	public:

		Collider();
		
		constexpr auto IsEnableDebugDraw() const noexcept -> bool { return m_IsEnableDebugDraw; }
		constexpr auto EnableDebugDraw(bool state) noexcept -> void { m_IsEnableDebugDraw = state; }
		constexpr auto IsTriggered() const noexcept -> bool { return m_IsTriggered; }
		constexpr auto SetIsTrigger(bool state) noexcept -> void { m_IsATrigger = state; }
		void AddCaches(std::weak_ptr<Collider>&& collider);
		void ClearCaches() { m_pCaches.clear(); }
		const glm::fvec2& GetScale() const { return *m_ObjScale; }
		virtual void Resolve(const std::vector<std::weak_ptr<Collider>>& colliders) = 0;
		virtual void Resolve(const std::shared_ptr<Collider>& collider) = 0;
		//virtual void Resolve(std::weak_ptr<Collider>&& collider) = 0;
		
	protected:

		void Awake() override;
		
		std::vector<std::weak_ptr<Collider>> m_pCaches;
		const glm::fvec3* m_ObjPos;
		const glm::fvec2* m_ObjScale;
		bool m_IsATrigger;
		bool m_IsTriggered;

	private:
		
		bool m_IsEnableDebugDraw;
	};
}


