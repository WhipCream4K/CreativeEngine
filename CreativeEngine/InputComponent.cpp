#include "pch.h"
#include "InputComponent.h"

void dae::ActionHandle::ShouldExecute()
{
	// Get call once the state of the key is changed

	isPressedFlag = !isPressedFlag /*== false ? true : false*/;

	switch (inputEvent) {
	case InputEvent::IE_Pressed:

		isTriggered = isPressedFlag;

		break;
	case InputEvent::IE_Released:

		if (!isPressedFlag)
			isTriggered = true;

		break;
	case InputEvent::IE_Down:

		isTriggered = isPressedFlag;

		break;
	}
}

void dae::AxisHandle::ShouldExecute(float axisValue)
{
	isTriggered = true;
	totalAxisValue = axisValue;
}

void dae::InputComponent::Awake()
{
	GetScene()->GetSceneContext().pInputManager->RegisterObserver(GetShared<InputComponent>());
}

void dae::InputComponent::Update()
{
	for (auto& handle : m_ActionHandle)
	{
		if (handle.second.isTriggered)
		{
			if (handle.second.inputEvent == InputEvent::IE_Pressed)
				handle.second.isTriggered = false;

			if (handle.second.inputEvent == InputEvent::IE_Released)
				handle.second.isTriggered = false;

			handle.second.action->Invoke();
		}
	}

	for (auto& handle : m_AxisHandle)
	{
		if (handle.second.isTriggered)
		{
			handle.second.action->Invoke(handle.second.totalAxisValue);
			handle.second.isTriggered = false;
		}
	}
}
