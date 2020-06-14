#pragma once

#include "Singleton.h"
#include "CreativeTypeName.h"

#include <vector>
#include <memory>

struct SDL_Window;
struct SDL_Renderer;
namespace dae
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		
		template<typename T = Scene>
		static constexpr void AddScene(std::shared_ptr<SceneType<T>>&& pScene)
		{
			GetInstance()->AddSceneImpl(std::move(pScene));
		}

		template<typename T = Scene>
		static constexpr void AddScene(const std::shared_ptr<SceneType<T>>& pScene)
		{
			GetInstance()->AddSceneImpl(pScene);
		}
		
	private:

		void AddSceneImpl(std::shared_ptr<Scene>&& pScene);
		void AddSceneImpl(const std::shared_ptr<Scene>& pScene);
		
		friend class CreativeCore;
		void Update();
		void Render() const;
	
		//std::vector<std::shared_ptr<IInternalSceneObject>> m_pScenes;
		std::vector<std::shared_ptr<IInternalSceneObject>> m_pScenes;
		std::weak_ptr<IInternalSceneObject> m_pCurrentScene;
	};
}


