#pragma once

#include "CreativeTypeName.h"
#include "CreativeStructs.h"

#define SCENE_COMPONENT

namespace dae
{
	class SceneManager;
	class Scene : public IInternalSceneObject, public std::enable_shared_from_this<Scene>
	{
	public:
		Scene(const std::string& sceneName);

		virtual ~Scene() = default;
		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		// CreateGameObject the clone object of that class in the scene
		template<typename T = GameObject>
		constexpr std::shared_ptr<GameObjectType<T>> CreateGameObject(const glm::fvec3& position = glm::fvec3{ 0.0f,0.0f,0.0f },
			const glm::fvec3& rotation = { 0.0f,0.0f,0.0f }, const glm::fvec2& scale = { 1.0f,1.0f });

		// TODO: Do GameObject Instantiation (Create GameObject During Runtime)

		const SceneContext& GetSceneContext() const { return m_Context; }

		template<typename UserScene>
		constexpr auto GetShared() noexcept->std::shared_ptr<SceneType<UserScene>>;

	protected:

		virtual void SceneInitialize() = 0;
		virtual void SetupInputAction() {}
		virtual void Update() = 0;
		virtual void Render() const {}

	private:

		SceneContext m_Context;
		//std::vector<std::shared_ptr<GameObject>> m_pGameObjects;
		std::vector<std::shared_ptr<IInternalGameObject>> m_pGameObjects;
		std::string m_SceneName;
		bool m_IsInit;
		
		void RootRender() const override final;
		void RootInitialize() override final;
		void RootUpdate() override final;
	};

	template <typename T>
	constexpr std::shared_ptr<GameObjectType<T>> Scene::CreateGameObject(const glm::fvec3& position,
		const glm::fvec3& rotation, const glm::fvec2& scale)
	{
		const auto gameObject{ std::make_shared<T>() };
		m_pGameObjects.emplace_back(gameObject);
		std::static_pointer_cast<IInternalGameObject>(gameObject)->RegisterOwner(weak_from_this());
		Transform& transform{ gameObject->GetTransform() };
		transform.SetPosition(position);
		transform.SetRotation(rotation);
		transform.SetScale(scale);
		return gameObject;
	}

	template <typename UserScene>
	constexpr auto Scene::GetShared() noexcept -> std::shared_ptr<SceneType<UserScene>>
	{
		auto temp{ std::shared_ptr<Scene>(this,[](Scene*) {}) };
		return std::static_pointer_cast<UserScene>(temp);
	}
}

