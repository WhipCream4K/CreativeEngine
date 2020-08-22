#include "pch.h"
#include "Scene.h"
#include "InputManager.h"
#include <future>
#include "Collider.h"
#include "PhysicsScene.h"


dae::Scene::Scene(const std::string& sceneName)
	: m_Context{}
	, m_SceneName{ sceneName }
	, m_IsInit{}
	, m_CallForDelete()
{
}

auto dae::Scene::Destroy(const std::shared_ptr<IInternalGameObject>& ref) noexcept -> void
{
	for (auto& gameObject : m_pGameObjects)
	{
		if (ref == gameObject)
		{
			gameObject->m_ToBeDestroyed = true;
			m_CallForDelete = true;
			gameObject->OnDestroy();
		}
	}
}

void dae::Scene::RootRender() const
{
	Render();

	for (const auto& gameObject : m_pGameObjects)
	{
		gameObject->RootRender();
	}

	m_pPhysicsScene->Render();
}

void dae::Scene::RootInitialize()
{
	if (m_IsInit)
		return;

	m_Context.pInputManager = std::make_shared<InputManager>();
	m_Context.pGameTime = std::make_shared<GameTime>();
	m_pPhysicsScene = std::make_shared<PhysicsScene>();

	m_Context.pGameTime->Reset();
	m_Context.pGameTime->Stop();

	SceneInitialize();
	SetUpInputMappingGroup();

	m_Context.pGameTime->Start();

#ifdef ASYNC

	std::vector<std::future<void>> futures;

	for (const auto& gameObject : m_pGameObjects)
	{
		futures.push_back(std::async(std::launch::async, [](const std::shared_ptr<IInternalGameObject>& obj)
			{
				obj->RootAwake();
			}, gameObject));
	}

	for (auto& future : futures)
	{
		future.get();
	}

	futures.clear();

	for (const auto& gameObject : m_pGameObjects)
	{
		futures.push_back(std::async(std::launch::async, [](const std::shared_ptr<IInternalGameObject>& obj)
			{
				obj->RootStart();
			}, gameObject));
	}

	for (auto& future : futures)
	{
		future.get();
	}

#else

	for (const auto& gameObject : m_pGameObjects)
	{
		gameObject->RootAwake();
	}

	for (const auto& gameObject : m_pGameObjects)
	{
		gameObject->RootStart();
	}

#endif

	m_IsInit = true;
}

void dae::Scene::RootUpdate()
{
	m_Context.pGameTime->Update();
	m_Context.pInputManager->ReadInputs();

	// 1. User Update as in update what user want to do
	// 2. Physics Object Update

	Update();

	for (const auto& gameObject : m_pGameObjects)
	{
		gameObject->RootUpdate();
	}

	m_pPhysicsScene->Update();


	for (const auto& gameObject : m_pGameObjects)
	{
		gameObject->RootLateUpdate();
	}

	AddNewPendingGameObjects();
	DestroyPendingGameObject();
}

void dae::Scene::DestroyPendingGameObject()
{
	if(!m_CallForDelete)
		return;
	if (!m_pPhysicsScene->IsResultValid())
		return;

	const auto result = std::remove_if(m_pGameObjects.begin(), m_pGameObjects.end(), [](const std::shared_ptr<IInternalGameObject>& ref)
		{
			return ref->m_ToBeDestroyed;
		});

	if (result != m_pGameObjects.end())
		m_pGameObjects.erase(result,m_pGameObjects.end());
	m_CallForDelete = false;
	m_pPhysicsScene->ClearObjers();
}

void dae::Scene::AddNewPendingGameObjects()
{
	if (!m_pPhysicsScene->IsResultValid())
		return;

	for (auto& pending : m_pPendingAdd)
	{
		pending->RootAwake();
		pending->RootStart();
		m_pGameObjects.emplace_back(std::move(pending));
	}

	m_pPendingAdd.clear();
}
