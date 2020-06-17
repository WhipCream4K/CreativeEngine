#include "pch.h"
#include "CharacterController.h"

dae::CharacterController::CharacterController()
{
	// Set default value
	if (!m_pInputComponent)
		m_pInputComponent = CreateComponent<InputComponent>();	
}

