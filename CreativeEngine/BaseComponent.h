#pragma once
#include <memory>
#include <utility>

#include "CreativeTypeName.h"
#include "IInternalComponent.h"

namespace dae
{
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

		std::shared_ptr<GameObject> GetGameObject() const;

		template<typename T>
		constexpr auto GetShared() const noexcept->std::shared_ptr<GameComponent<const T>>;

		template<typename T>
		constexpr auto GetShared() noexcept->std::shared_ptr<GameComponent<T>>;

		auto GetScene() const noexcept->std::shared_ptr<Scene>;

	protected:

		void Render() const override {}

		void Awake() override {};
		void Start() override {}

		void Update() override {};
		void LateUpdate() override {}

		void RegisterOwner(std::weak_ptr<GameObject>&& pOwner) override final { m_pGameObject = std::move(pOwner); }

	private:

		std::weak_ptr<GameObject> m_pGameObject;
		bool m_IsActive;

	};

	template <typename T>
	constexpr auto BaseComponent::GetShared() const noexcept -> std::shared_ptr<GameComponent<const T>>
	{
		std::shared_ptr<const BaseComponent> temp{ shared_from_this() };
		return std::static_pointer_cast<const T>(temp);
	}

	template <typename T>
	constexpr auto BaseComponent::GetShared() noexcept -> std::shared_ptr<GameComponent<T>>
	{
		std::shared_ptr<BaseComponent> temp{ shared_from_this() };
		return std::static_pointer_cast<T>(temp);
	}
}

