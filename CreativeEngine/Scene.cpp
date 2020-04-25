#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameTime.h"
#include "InputManager.h"

dae::Scene::Scene(const std::string& sceneName)
	: m_SceneName{ sceneName }
	, m_Context{}
	, m_IsInit{}
{
}

void dae::Scene::AddGameObject(std::shared_ptr<GameObject> pGameObject)
{
	if (pGameObject)
	{
		pGameObject->m_pRefScene = shared_from_this();
		m_pGameObjects.push_back(pGameObject); // create a copy of the gameObject result in increasing the ref count
	}
}

void dae::Scene::Render()
{
	for (auto& gameObject : m_pGameObjects)
	{
		gameObject->Render();
	}
}

void dae::Scene::RootInitialize()
{
	m_Context.pInputManager = std::make_shared<InputManager>();
	m_Context.pGameTime = std::make_shared<GameTime>();

	m_Context.pGameTime->Start();
	
	for (auto& gameObject : m_pGameObjects)
	{
		gameObject->Awake();
		gameObject->Start();
	}
}

void dae::Scene::RootUpdate()
{
	m_Context.pGameTime->Update();
	m_Context.pInputManager->ReadInputs();
	
	for (auto& gameObject : m_pGameObjects)
	{
		gameObject->Update();
		gameObject->LateUpdate();
	}
}
