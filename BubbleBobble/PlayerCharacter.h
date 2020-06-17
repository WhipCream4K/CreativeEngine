#pragma once
#include "CharacterController.h"

namespace dae
{
	class SpriteRenderer;
}

class PlayerCharacter : public dae::CharacterController
{
public:
	
	PlayerCharacter();
	
	void SetUpInputComponent();
	void JumpTest();

protected:

	void Awake() override;
	void MoveVertical(float value) override;
	void MoveHorizontal(float value) override;
	void LateUpdate() override;
	
private:
	
	// Container
	std::vector<std::shared_ptr<dae::Sprite>> m_pPlayerSpriteSheet;
	std::shared_ptr<dae::SpriteRenderer> m_pSpriteRenderer;
	bool m_IsMoving;
};

