#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include <SDL.h>
#include <stdexcept>
#include <string>

void dae::SceneManager::Render() const
{
	if(m_pCurrentScene.lock())
		m_pCurrentScene.lock()->Render();
}

void dae::SceneManager::Update()
{
	if(m_pCurrentScene.lock())
		m_pCurrentScene.lock()->RootUpdate();
}

void dae::SceneManager::AddScene(std::shared_ptr<Scene> pScene)
{
	// Check if this scene already exists
	const auto it{ std::find(m_pScenes.begin(),m_pScenes.end(),pScene) };

	if (it != m_pScenes.end())
	{
		m_pScenes.push_back(pScene);

		if (pScene->m_IsInit)
			pScene->RootInitialize();

		if (!m_pCurrentScene.lock())
			m_pCurrentScene = pScene;
	}
}
