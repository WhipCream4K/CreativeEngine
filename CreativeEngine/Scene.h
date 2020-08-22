#pragma once

#include "CreativeTypeName.h"
#include "CreativeStructs.h"
//#include "Collider.h"

#define SCENE_COMPONENT

namespace dae
{
	class PhysicsScene;
	class SceneManager;
	class Scene : public IInternalSceneObject, public std::enable_shared_from_this<Scene>
	{
	public:
		Scene(const std::string& sceneName);

		virtual ~Scene() override = default;
		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		// CreateGameObject the clone object of that class in the scene
		template<typename T = GameObject>
		constexpr std::shared_ptr<GameObjectType<T>> CreateGameObject(const glm::fvec3& position = glm::fvec3{ 0.0f,0.0f,0.0f },
			const glm::fvec3& rotation = { 0.0f,0.0f,0.0f }, const glm::fvec2& scale = { 1.0f,1.0f });

		template<typename T = GameObject>
		constexpr std::shared_ptr<GameObjectType<T>> Instantiate(const glm::fvec3& position = glm::fvec3{ 0.0f,0.0f,0.0f },
			const glm::fvec3& rotation = { 0.0f,0.0f,0.0f }, const glm::fvec2& scale = { 1.0f,1.0f });


		const SceneContext& GetSceneContext() const { return m_Context; }

		std::shared_ptr<PhysicsScene> GetPhysicsScene() const { return m_pPhysicsScene; }

		template<typename UserScene>
		constexpr auto GetShared() noexcept ->std::shared_ptr<SceneType<UserScene>>;

		template<typename UserScene>
		constexpr auto GetShared() const noexcept -> std::shared_ptr<const SceneType<UserScene>>;

		auto Destroy(const std::shared_ptr<IInternalGameObject>& ref) noexcept -> void;

		// Return the first reference of the define GameObject
		template<typename T>
		constexpr auto GetGameObjectOfType() const noexcept -> std::shared_ptr<GameObjectType<T>>;
		
	protected:

		virtual void SceneInitialize() = 0;
		virtual void SetUpInputMappingGroup() {}
		virtual void Update() = 0;
		virtual void Render() const {}

	private:

		SceneContext m_Context;
		std::shared_ptr<PhysicsScene> m_pPhysicsScene;
		//std::vector<std::shared_ptr<GameObject>> m_pGameObjects;
		std::vector<std::shared_ptr<IInternalGameObject>> m_pGameObjects;
		std::vector<std::shared_ptr<IInternalGameObject>> m_pPendingAdd;
		std::vector<std::weak_ptr<Collider>> m_pColliderObjects;

		std::string m_SceneName;
		bool m_IsInit;

		void RootRender() const override final;
		void RootInitialize() override final;
		void RootUpdate() override final;
		void DestroyPendingGameObject();
		void AddNewPendingGameObjects();
		bool m_CallForDelete;
	};

	template <typename T>
	constexpr std::shared_ptr<GameObjectType<T>> Scene::CreateGameObject(const glm::fvec3& position,
		const glm::fvec3& rotation, const glm::fvec2& scale)
	{
		const auto gameObject{ std::make_shared<T>() };
		m_pGameObjects.emplace_back(gameObject);
		std::static_pointer_cast<IInternalGameObject>(gameObject)->RegisterOwner(GetShared<Scene>());

		auto transform = gameObject->GetTransform();
		transform->SetPosition(position);
		transform->SetRotation(rotation);
		transform->SetScale(scale);

		return gameObject;
	}

	template <typename T>
	constexpr std::shared_ptr<GameObjectType<T>> Scene::Instantiate(const glm::fvec3& position,
		const glm::fvec3& rotation, const glm::fvec2& scale)
	{
		const auto gameObject{ std::make_shared<T>() };
		m_pPendingAdd.emplace_back(gameObject);
		const auto temp = std::static_pointer_cast<IInternalGameObject>(gameObject);
		temp->RegisterOwner(GetShared<Scene>());

		//// really bad
		//temp->RootAwake();
		//temp->RootStart();

		auto transform{ gameObject->GetTransform() };
		transform->SetPosition(position);
		transform->SetRotation(rotation);
		transform->SetScale(scale);

		return gameObject;
	}

	template <typename UserScene>
	constexpr auto Scene::GetShared() noexcept -> std::shared_ptr<SceneType<UserScene>>
	{
		return std::static_pointer_cast<UserScene>(shared_from_this());
	}

	template <typename UserScene>
	constexpr auto Scene::GetShared() const noexcept -> std::shared_ptr<const SceneType<UserScene>>
	{
		return std::static_pointer_cast<const UserScene>(shared_from_this());
	}

	template <typename T>
	constexpr auto Scene::GetGameObjectOfType() const noexcept -> std::shared_ptr<GameObjectType<T>>
	{
		const auto& type = typeid(T);
		for (const auto& gameObject : m_pGameObjects)
		{
			if (gameObject && type == typeid(*gameObject))
				return std::static_pointer_cast<T>(gameObject);
		}

		return nullptr;
	}
}

