#include "pch.h"
#include "PlayerDigger.h"
#include "Sprite.h"
#include "InputComponent.h"
#include "Animator.h"
#include "BoxCollider2D.h"

PlayerDigger::PlayerDigger()
	: GameObject()
	, m_IsShellEmpty()
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
		m_pWalkUp = Sprite::CreateSpriteSheet(player1Sprite, { 0.0f,30.0f }, { 32.0f,30.0f }, 3, 0.0f);
		m_pWalkDown = Sprite::CreateSpriteSheet(player1Sprite, { 0.0f,60.0f }, { 32.0f,30.0f }, 3, 0.0f);
	}

	{
		m_pWalkStraightX = Sprite::CreateSpriteSheet(player1Sprite, { 0.0f,90.0f }, { 32.0f,30.0f }, 3, 0.0f);
		m_pWalkUpX = Sprite::CreateSpriteSheet(player1Sprite, { 0.0f,120.0f }, { 32.0f,30.0f }, 3, 0.0f);
		m_PWalkDownX = Sprite::CreateSpriteSheet(player1Sprite, { 0.0f,150.0f }, { 32.0f,30.0f }, 3, 0.0f);
	}

	m_pDeadSprite = Sprite::CreateSpriteSheet(player1DeadSprite, {}, { 32.0f,30.0f }, 4, 0.0f);


	auto spriteRenderer = CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(m_pWalkStraight[0], true);
	m_pSpriteRenderer = spriteRenderer;

	// Create Animation Clips and initialize animator
	auto playerIdle = AnimationClip::Create(GetShared<PlayerDigger>());
	playerIdle->AddProperty(spriteRenderer, m_pWalkStraight);
	playerIdle->SetSampleRate(10);
	auto playerWalkUp = AnimationClip::Create(GetShared<PlayerDigger>());
	playerWalkUp->AddProperty(spriteRenderer, m_pWalkUp);
	playerWalkUp->SetSampleRate(10);
	auto playerWalkDown = AnimationClip::Create(GetShared<PlayerDigger>());
	playerWalkDown->AddProperty(spriteRenderer, m_pWalkDown);
	playerWalkDown->SetSampleRate(10);
	auto playerDead = AnimationClip::Create(GetShared<PlayerDigger>());
	playerDead->AddProperty(spriteRenderer, m_pDeadSprite);
	playerDead->SetSampleRate(4);

	m_pAnimator = CreateComponent<Animator>();
	const auto animator = m_pAnimator.lock();
	animator->AddTransition(playerIdle, "PlayerUp", playerWalkUp, false);
	animator->AddTransition(playerIdle, "PlayerDown", playerWalkDown, false);

	animator->SetDefaultAnimClip(playerIdle);

	// Initialize Controller
	auto inputComponent = CreateComponent<InputComponent>();
	inputComponent->BindAxis("Horizontal", GetShared<PlayerDigger>(), &PlayerDigger::MoveHorizontal);
	inputComponent->BindAxis("Vertical", GetShared<PlayerDigger>(), &PlayerDigger::MoveVertical);

	// Initialize Collider
	auto collider = CreateComponent<BoxCollider2D>();
	collider->SetSize({ 32.0f,30.0f });
	collider->EnableDebugDraw(true);
}

void PlayerDigger::MoveHorizontal(float value)
{
	float deltaSec{ GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };
	glm::fvec3 worldRight{ 1.0f,0.0f,0.0f };
	auto vel = value * worldRight * deltaSec * 200.0f;
	GetTransform()->SetRelativePosition(vel);

	m_pSpriteRenderer.lock()->SetFlipY(value < 1.0f ? true : false);
	const auto animator = m_pAnimator.lock();
	animator->SetBool("PlayerUp", false);
	animator->SetBool("PlayerDown", false);
}

void PlayerDigger::MoveVertical(float value)
{
	float deltaSec{ GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };
	glm::fvec3 worldUp{ 0.0f,1.0f,0.0f };
	auto vel = value * worldUp * deltaSec * 200.0f;
	GetTransform()->SetRelativePosition(vel);
	const auto animator = m_pAnimator.lock();
	const bool playerGoesUp{ value > 0.0f };
	animator->SetBool("PlayerUp", playerGoesUp);
	animator->SetBool("PlayerDown", !playerGoesUp);
}
