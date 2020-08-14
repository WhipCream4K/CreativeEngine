#include "pch.h"
#include "PlayerDigger.h"
#include "Sprite.h"
#include "InputComponent.h"

PlayerDigger::PlayerDigger()
{
}

void PlayerDigger::Awake()
{
	// Initialize All needed sprites
	using namespace dae;
	auto player1Sprite = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Player1.tga", "Player1");
	auto player1DeadSprite = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Player1_Dead.tga", "Player1_Dead");

	{
		m_pWalkStraight = Sprite::CreateSpriteSheet(player1Sprite, {}, { 32.0f,30.0f }, 3, 0.0f);
		m_pWalkUp = Sprite::CreateSpriteSheet(player1Sprite, {0.0f,30.0f}, { 32.0f,30.0f }, 3, 0.0f);
		m_pWalkDown = Sprite::CreateSpriteSheet(player1Sprite, {0.0f,60.0f}, { 32.0f,30.0f }, 3, 0.0f);
	}

	{
		m_pWalkStraightX = Sprite::CreateSpriteSheet(player1Sprite, {0.0f,90.0f}, { 32.0f,30.0f }, 3, 0.0f);
		m_pWalkUpX = Sprite::CreateSpriteSheet(player1Sprite, { 0.0f,120.0f }, { 32.0f,30.0f }, 3, 0.0f);
		m_PWalkDownX = Sprite::CreateSpriteSheet(player1Sprite, { 0.0f,150.0f }, { 32.0f,30.0f }, 3, 0.0f);
	}

	m_pDeadSprite = Sprite::CreateSpriteSheet(player1DeadSprite, {}, { 32.0f,30.0f }, 4, 0.0f);

	// Initialize Controller
	auto inputComponent = CreateComponent<InputComponent>();
}
