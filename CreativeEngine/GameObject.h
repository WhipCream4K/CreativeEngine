#pragma once

//#include "CreativeTypeName.h"
#include "IInternalGameObject.h"
#include "Transform.h"

namespace dae
{
	class Scene;
	class BaseComponent;
	class GameObject : public IInternalGameObject, public std::enable_shared_from_this<GameObject>
	{
	public:

		GameObject();
		virtual ~GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		void SetActive(bool value) { m_IsActive = value; }

		template<typename T = GameObject, typename U = Scene>
		static constexpr std::shared_ptr<GameObjectType<T>> Create(std::weak_ptr<SceneType<U>> pScene);

		template<typename T = GameObject, typename U = Scene>
		static constexpr std::shared_ptr<GameObjectType<T>> Create(SceneType<U>* pScene);

		template<typename T>
		constexpr std::shared_ptr<GameComponent<T>> GetComponent();

		template<typename T>
		constexpr std::shared_ptr<GameComponent<T>> CreateComponent();

		template<typename T>
		constexpr auto GetShared() noexcept ->std::shared_ptr<GameObjectType<T>>;

		template<typename T>
		constexpr auto GetShared() const noexcept ->std::shared_ptr<GameObjectType<const T>>;

		std::shared_ptr<Scene> GetScene() const { return m_pRefScene.lock(); }

		constexpr auto GetTransform() -> Transform& { return m_Transform; }
		

	protected:

		virtual void Render() const {}

		virtual void Awake() {}
		virtual void Start() {}

		virtual void Update() {}
		virtual void LateUpdate() {}

		void RegisterOwner(std::weak_ptr<Scene>&& pScene) override final { m_pRefScene = std::move(pScene); }
		
	private:

		Transform m_Transform;
		std::vector<std::shared_ptr<IInternalComponent>> m_pComponents;
		std::weak_ptr<Scene> m_pRefScene;
		std::string m_Tag;
		bool m_IsStatic;
		bool m_IsActive;
		bool m_IsInit;

		void RootRender() const override final;
		void RootAwake() override final;
		void RootStart() override final;
		void RootUpdate() override final;
		void RootLateUpdate() override final;
		void RootInitialize() override final {}
	};

	template <typename T, typename U>
	constexpr std::shared_ptr<GameObjectType<T>> GameObject::Create(std::weak_ptr<SceneType<U>> pScene)
	{
		return pScene.lock()->template CreateGameObject<T>();
	}

	template <typename T, typename U>
	constexpr std::shared_ptr<GameObjectType<T>> GameObject::Create(SceneType<U>* pScene)
	{
		return pScene->template CreateGameObject<T>();
	}

	template <typename T>
	constexpr std::shared_ptr<GameComponent<T>> GameObject::GetComponent()
	{
		const auto& type = typeid(T);
		for (auto& component : m_pComponents)
		{
			if (typeid(component) == type)
				return std::static_pointer_cast<T, BaseComponent>(component);
		}

		return nullptr;
	}

	template <typename T>
	constexpr std::shared_ptr<GameComponent<T>> GameObject::CreateComponent()
	{
		const auto component{ std::make_shared<T>() };
		m_pComponents.emplace_back(component);
		const auto owner{ GetShared<GameObject>() };
		std::static_pointer_cast<IInternalComponent>(component)->RegisterOwner(std::shared_ptr<GameObject>(this,[](GameObject*){}));
		return component; // return strong ref to this component
	}

	template <typename T>
	constexpr auto GameObject::GetShared() noexcept -> std::shared_ptr<GameObjectType<T>>
	{
		std::shared_ptr<GameObject> temp{ std::shared_ptr<GameObject>(this,[](GameObject*) {}) };
		return std::static_pointer_cast<T>(temp);
	}

	template <typename T>
	constexpr auto GameObject::GetShared() const noexcept -> std::shared_ptr<GameObjectType<const T>>
	{
		std::shared_ptr<const GameObject> temp{ std::shared_ptr<const GameObject>(this,[](const GameObject*) {}) };
		return std::static_pointer_cast<const T>(temp);
	}
}

