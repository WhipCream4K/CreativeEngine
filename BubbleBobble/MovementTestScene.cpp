#include "pch.h"
#include "MovementTestScene.h"
#include "PlayerCharacter.h"

MovementTestScene::MovementTestScene()
	: Scene("MovementTestScene")
{
}

void MovementTestScene::SceneInitialize()
{
	const auto playerCharacter{ CreateGameObject<PlayerCharacter>() };
	playerCharacter->GetTransform()->SetScale(15.0f, 15.0f);
}

void MovementTestScene::SetUpInputMappingGroup()
{
	const auto& inputManager{ GetSceneContext().pInputManager };
	auto& actionGroup = inputManager->AddInputActionGroup("Jump");
	actionGroup.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_SPACE));
	actionGroup.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_q));
	auto& horizontal = inputManager->AddInputAxisGroup("Horizontal");
	horizontal.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_LEFT),-1.0f);
	horizontal.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_RIGHT),1.0f);
	auto& vertical = inputManager->AddInputAxisGroup("Vertical");
	vertical.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_UP),1.0f);
	vertical.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_DOWN), -1.0f);
}

void MovementTestScene::Update()
{

}

void MovementTestScene::Render() const
{

}
