#pragma once
#include <memory>
#include <utility>

#include "CreativeTypeName.h"
#include "IInternalComponent.h"

namespace dae
{
	class Transform;
	class GameObject;
	class BaseComponent : public IInternalComponent, public std::enable_shared_from_this<BaseComponent>
	{
		
	public:

		BaseComponent();
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) noexcept = delete;

		void SetActive(bool value) { m_IsActive = value; }
		bool IsActive() const { return m_IsActive; }

		std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject.lock(); }

		template<typename T>
		constexpr std::shared_ptr<const GameComponent<T>> GetShared() const;

		template<typename T>
		constexpr std::shared_ptr<GameComponent<T>> GetShared();
		
	protected:
		
		void Render() const override{}
		
		void Awake() override {}
		void Start() override {}
		
		void Update() override {}
		void LateUpdate() override {}


	private:
		
		std::weak_ptr<GameObject> m_pGameObject;
		bool m_IsActive;
		
		void SetOwner(std::weak_ptr<GameObject> pOwner) override final{ m_pGameObject = std::move(pOwner); }
	};

	template <typename T>
	constexpr std::shared_ptr<const GameComponent<T>> BaseComponent::GetShared() const
	{
		return std::static_pointer_cast<const T>(shared_from_this());
	}

	template <typename T>
	constexpr std::shared_ptr<GameComponent<T>> BaseComponent::GetShared()
	{
		return std::static_pointer_cast<T>(shared_from_this());
	}
}

