#pragma once
#include "CharacterController.h"

class PlayerCharacter : public dae::CharacterController
{
public:
	
	PlayerCharacter();
	
	void Awake() override;
	void SetUpInputComponent();
	void JumpTest();
	
private:
	// Container
	std::vector<std::shared_ptr<dae::Sprite>> m_pPlayerSpriteSheet;
	bool m_Ismoving;
};

