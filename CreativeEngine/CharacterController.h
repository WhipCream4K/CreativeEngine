#pragma once
#include "GameObject.h"
#include "InputComponent.h"

namespace dae
{
	class InputComponent;
	class CharacterController : public GameObject
	{
	public:
		
		CharacterController();		
	protected:
		std::shared_ptr<InputComponent> m_pInputComponent;
	};
}

