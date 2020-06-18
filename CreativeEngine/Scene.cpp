#include "pch.h"
#include "Scene.h"
#include "InputManager.h"
#include <future>


dae::Scene::Scene(const std::string& sceneName)
	: m_Context{}
	, m_SceneName{ sceneName }
	, m_IsInit{}
{
}

auto dae::Scene::Destroy(const std::shared_ptr<IInternalGameObject>& ref) noexcept -> void
{
	for (auto& gameObject : m_pGameObjects)
	{
		if (ref == gameObject)
			gameObject->m_ToBeDestroyed = true;
	}
}

void dae::Scene::RootRender() const
{
	Render();

	for (const auto& gameObject : m_pGameObjects)
	{
		gameObject->RootRender();
	}
}

void dae::Scene::RootInitialize()
{
	if (m_IsInit)
		return;

	m_Context.pInputManager = std::make_shared<InputManager>();
	m_Context.pGameTime = std::make_shared<GameTime>();

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
	AddNewPendingGameObjects();
	DestroyPendingGameObject();
	
	m_Context.pGameTime->Update();
	m_Context.pInputManager->ReadInputs();

	Update();

	for (const auto& gameObject : m_pGameObjects)
	{
		gameObject->RootUpdate();
	}

	for (const auto& gameObject : m_pGameObjects)
	{
		gameObject->RootLateUpdate();
	}
}

void dae::Scene::DestroyPendingGameObject()
{
	const auto result = std::remove_if(m_pGameObjects.begin(), m_pGameObjects.end(), [](const std::shared_ptr<IInternalGameObject>& ref)
		{
			return ref->m_ToBeDestroyed;
		});

	if(result != m_pGameObjects.end())
		m_pGameObjects.erase(result);
}

void dae::Scene::AddNewPendingGameObjects()
{
	for (auto & pending : m_pPendingAdd)
	{
		m_pGameObjects.emplace_back(std::move(pending));
	}

	m_pPendingAdd.clear();
}
