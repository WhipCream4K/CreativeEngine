#pragma once

//#include "CreativeTypeName.h"
#include "IInternalGameObject.h"
#include "CreativeTypeName.h"
#include "Scene.h"
#include "PhysicsScene.h"
#include "Collider.h"

namespace dae
{
	class Scene;
	class BaseComponent;
	class Transform;
	class IInternalComponent;
	class Collider;
	class PhysicsComponent;

	class GameObject : public IInternalGameObject, public std::enable_shared_from_this<GameObject>
	{
	public:

		GameObject();
		virtual ~GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		constexpr auto SetActive(bool value) noexcept ->void { m_IsActive = value; }

		template<typename T = GameObject, typename U = Scene>
		static constexpr std::shared_ptr<GameObjectType<T>> Create(std::weak_ptr<SceneType<U>> pScene);

		template<typename T = GameObject, typename U = Scene>
		static constexpr std::shared_ptr<GameObjectType<T>> Create(SceneType<U>* pScene);

		template<typename T>
		constexpr std::shared_ptr<GameComponent<T>> GetComponent();

		template<typename T>
		constexpr std::shared_ptr<GameComponent<T>> CreateComponent();

		//template<typename T>
		//constexpr auto CreateComponent() noexcept -> std::shared_ptr<ColliderType<T>>;

		template<typename T>
		constexpr auto GetShared() noexcept ->std::shared_ptr<GameObjectType<T>>;

		template<typename T>
		constexpr auto GetShared() const noexcept ->std::shared_ptr<GameObjectType<const T>>;

		auto GetScene() const noexcept -> std::shared_ptr<Scene> { return m_pRefScene.lock(); }

		auto GetTransform() const noexcept -> std::shared_ptr<Transform> { return m_pTransform.lock(); }

		virtual void OnBeginOverlap(const std::vector<std::weak_ptr<Collider>>&) {}


	protected:

		virtual void Render() const {}

		virtual void Awake() {}
		virtual void Start() {}

		virtual void Update() {}
		virtual void LateUpdate() {}

		virtual void OnDestroy() override {}

		void RegisterOwner(std::weak_ptr<Scene>&& pScene) override final { m_pRefScene = std::move(pScene); }
		//void RegisterOwner(std::shared_ptr<Scene>&& pScene) override final { m_pRefScene = std::move(pScene); }

	private:

		std::vector<std::shared_ptr<IInternalComponent>> m_pComponents;
		std::vector<std::shared_ptr<PhysicsComponent>> m_pPhysicsComponent;
		std::weak_ptr<Scene> m_pRefScene;
		std::weak_ptr<Transform> m_pTransform;
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

		void RegisterCollider(const std::shared_ptr<BaseComponent>& component);
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
			if (typeid(*component) == type)
				return std::static_pointer_cast<T>(component);
		}

		return nullptr;
	}

	template <typename T>
	constexpr std::shared_ptr<GameComponent<T>> GameObject::CreateComponent()
	{
		const auto component{ std::make_shared<T>() };
		m_pComponents.emplace_back(component);
		std::static_pointer_cast<IInternalComponent>(component)->RegisterOwner(shared_from_this());

		constexpr bool type{ std::is_base_of_v<Collider,T> };
		if (type)
			RegisterCollider(component);
		
		//if (typeid(T) == typeid(Collider))
			//GetScene()->GetPhysicsScene()->RegisterCollider(component);

		return component; // return strong ref to this component
	}

	template <typename T>
	constexpr auto GameObject::GetShared() noexcept -> std::shared_ptr<GameObjectType<T>>
	{
		return std::static_pointer_cast<T>(shared_from_this());
	}

	template <typename T>
	constexpr auto GameObject::GetShared() const noexcept -> std::shared_ptr<GameObjectType<const T>>
	{
		return std::static_pointer_cast<const T>(shared_from_this());
	}
}

