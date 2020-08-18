#pragma once
#include "CreativeTypeName.h"

namespace dae
{
	class PhysicsComponent : public std::enable_shared_from_this<PhysicsComponent>
	{
	public:

		PhysicsComponent() = default;
		PhysicsComponent(const PhysicsComponent&) = delete;
		PhysicsComponent(PhysicsComponent&&) = delete;
		PhysicsComponent& operator=(const PhysicsComponent&) = delete;
		PhysicsComponent& operator=(PhysicsComponent&&) = delete;
		virtual ~PhysicsComponent() = default;
		
		virtual void Update() {}
		virtual void Render() const {}

		std::shared_ptr<GameObject> GetGameObject() const { return m_pRefGameObject.lock(); }
		std::shared_ptr<Scene> GetScene() const { return m_pRefSceneObject.lock(); }

		void RegisterOwner(std::weak_ptr<GameObject>&& owner);

		template<typename T>
		constexpr auto GetShared() noexcept -> std::shared_ptr<PhysicsObject<T>>;

		template<typename T>
		constexpr auto GetShared() const noexcept -> std::shared_ptr<const PhysicsObject<T>>;

	private:

		std::weak_ptr<GameObject> m_pRefGameObject;
		std::weak_ptr<Scene> m_pRefSceneObject;
	};

	template <typename T>
	constexpr auto PhysicsComponent::GetShared() noexcept -> std::shared_ptr<PhysicsObject<T>>
	{
		return std::static_pointer_cast<T>(shared_from_this());
	}

	template <typename T>
	constexpr auto PhysicsComponent::GetShared() const noexcept -> std::shared_ptr<const PhysicsObject<T>>
	{
		return std::static_pointer_cast<const T>(shared_from_this());
	}
}


