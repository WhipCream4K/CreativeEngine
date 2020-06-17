#include "pch.h"
#include "InputComponent.h"

void dae::InputComponent::Awake()
{
	GetScene()->GetSceneContext().pInputManager->RegisterObserver(GetShared<InputComponent>());
}

void dae::InputComponent::Update()
{
	for (auto& handle : m_ActionHandle)
	{
		if(handle.second.isActive)
		{
			handle.second.action->Invoke();
			handle.second.isActive = false;
		}
	}

	for (auto& handle : m_AxisHandle)
	{
		if(handle.second.isActive)
		{
			handle.second.action->Invoke();
			handle.second.isActive = false;
		}
	}
}
