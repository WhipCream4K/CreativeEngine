#pragma once
#include <string>
#include <vector>
#include <memory>

#include "CreativeStructs.h"

#define SCENE_COMPONENT

namespace dae
{
	class GameObject;
	class SceneManager;
	class Scene : std::enable_shared_from_this<Scene>
	{
	public:
		Scene(const std::string& sceneName);

		virtual ~Scene() = default;
		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		void AddGameObject(std::shared_ptr<GameObject> pGameObject);
		
		// TODO: Make it so that you can instantiate from a class
		// TODO: Don't forget to add the rotation of the object
		// Instantiate the clone object of that class in the scene
		template<typename T>
		constexpr std::shared_ptr<typename std::enable_if<std::is_base_of_v<GameObject, T>, T>::type>
		Instantiate(const glm::fvec3& position);
		
		const SceneContext& GetSceneContext() const { return m_Context; }

	protected:
		
		virtual void SceneInitialize() = 0;
		virtual void SetUpInputAction() = 0; // set up all the input that will be used in the scene
		
	private:

		SceneContext m_Context;
		std::vector<std::shared_ptr<GameObject>> m_pGameObjects;
		std::string m_SceneName;
		bool m_IsInit;
		
		friend class SceneManager;
		void Render();
		void RootInitialize();
		void RootUpdate();
	};

	template <typename T>
	constexpr std::shared_ptr<typename std::enable_if<std::is_base_of_v<GameObject, T>, T>::type> Scene::Instantiate(
		const glm::fvec3& position)
	{
		
	}
}

