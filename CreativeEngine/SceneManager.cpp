#include "pch.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "IInternalSceneObject.h"
#include "Scene.h"



void dae::SceneManager::Render() const
{
	if (m_pCurrentScene.lock())
		m_pCurrentScene.lock()->RootRender();

	Renderer::Render();
	
#ifdef _DEBUG
	Debug::Render();
#endif
	
}

void dae::SceneManager::AddSceneImpl(const std::shared_ptr<Scene>& pScene)
{
	// Check if this scene already exists
	const auto it{ std::find(m_pScenes.begin(),m_pScenes.end(),pScene) };

	if (it == m_pScenes.end())
	{
		m_pScenes.push_back(pScene);
		std::static_pointer_cast<IInternalSceneObject>(pScene)->RootInitialize();

		if (!m_pCurrentScene.lock())
			m_pCurrentScene = pScene;
	}
}

void dae::SceneManager::Update()
{
	if(m_pCurrentScene.lock())
		m_pCurrentScene.lock()->RootUpdate();
}

void dae::SceneManager::AddSceneImpl(std::shared_ptr<Scene>&& pScene)
{
	AddSceneImpl(pScene);
	// Check if this scene already exists
	//const auto it{ std::find(m_pScenes.begin(),m_pScenes.end(),pScene) };

	//if (it == m_pScenes.end())
	//{
	//	m_pScenes.push_back(pScene);
	//	std::static_pointer_cast<IInternalSceneObject>(pScene)->RootInitialize();

	//	if (!m_pCurrentScene.lock())
	//		m_pCurrentScene = pScene;
	//}
}
