#pragma once

#include "Singleton.h"

#include <vector>
#include <memory>

struct SDL_Window;
struct SDL_Renderer;
namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		
		void AddScene(std::shared_ptr<Scene> pScene);
		
	private:

		friend class CreativeCore;
		void Update();
		void Render() const;
	
		std::vector<std::shared_ptr<Scene>> m_pScenes;
		std::weak_ptr<Scene> m_pCurrentScene;
	};
}


