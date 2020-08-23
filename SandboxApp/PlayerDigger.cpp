#include "pch.h"
#include "PlayerDigger.h"
#include "Sprite.h"
#include "InputComponent.h"
#include "Animator.h"
#include "BoxCollider2D.h"
#include "Digger.h"
#include "Movement.h"

PlayerDigger::PlayerDigger()
	: GameObject()
	, m_MovementSpeed()
	, m_MoveTimeLimitX(0.08f)
	, m_MoveTimeLimitY(0.08f)
	, m_PlayerFacingRight(true)
	, m_PlayerFacingUp(true)
	, m_TimeCountX()
	, m_TimeCountY()
	, m_HorizontalScale()
	, m_VerticalScale()
	, m_IsShellEmpty()
	, m_HasMovedHorizontal()
	, m_HasMovedVertical()
{
}

void PlayerDigger::SetPlayerStart(const glm::fvec3& position)
{
	if (m_pRefTransform.expired())
	{
		auto transform = GetTransform();
		m_pRefTransform = transform;
	}

	m_pRefTransform.lock()->SetPosition(position);
}

void PlayerDigger::Awake()
{
	// Initialize All needed sprites
	using namespace dae;

	m_pRefTransform = GetTransform();

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
	inputComponent->BindAction("Shoot", InputEvent::IE_Pressed,GetShared<PlayerDigger>(),&PlayerDigger::Shoot);

	// Initialize Collider
	auto collider = CreateComponent<BoxCollider2D>();
	collider->SetSize({ 32.0f,30.0f });
	collider->EnableDebugDraw(true);

	const auto& scale{ m_pRefTransform.lock()->GetScale() };
	m_PlayerActualSize.x = scale.x * 32.0f;
	m_PlayerActualSize.y = scale.y * 30.0f;

	m_pRefDiggerScene = GetScene()->GetShared<Digger>();
	// Initialize Player speed one click per pixel
}

void PlayerDigger::Update()
{
	const float deltaSeconds{ GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };

	if (m_HasMovedHorizontal)
	{
		m_TimeCountX += deltaSeconds;
		if (m_TimeCountX >= m_MoveTimeLimitX)
		{
			m_TimeCountX -= m_MoveTimeLimitX;
			m_HasMovedHorizontal = false;

			const bool playerGoesRight{ m_HorizontalScale > 0.0f };
			if (playerGoesRight && !m_PlayerFacingRight
				|| !playerGoesRight && m_PlayerFacingRight)
			{
				m_PlayerFacingRight = playerGoesRight;
				return;
			}

			const auto& currPos = m_pRefTransform.lock()->GetPosition();
			if (!m_pRefDiggerScene.lock()->IsInBetweenCellsY(currPos))
				m_pRefTransform.lock()->SetRelativePosition({ Digger::CellSize.x / 4.0f * m_HorizontalScale,0.0f,0.0f });
			else
			{
				// continue to the direction that player currently facing vertically
				float dirScale{ m_PlayerFacingUp ? 1.0f : -1.0f };
				m_pRefTransform.lock()->SetRelativePosition({ 0.0f, Digger::CellSize.y / 6.0f * dirScale, 0.0f });
			}
		}
		return;
	}

	if (m_HasMovedVertical)
	{
		m_TimeCountY += deltaSeconds;
		if (m_TimeCountY >= m_MoveTimeLimitY)
		{
			// check for cell collision
			m_TimeCountY -= m_MoveTimeLimitY;
			const auto& currPos = m_pRefTransform.lock()->GetPosition();
			m_HasMovedVertical = false;

			const bool playerGoesUp{ m_VerticalScale > 0.0f };
			if (playerGoesUp && !m_PlayerFacingUp
				|| !playerGoesUp && m_PlayerFacingUp)
			{
				m_PlayerFacingUp = playerGoesUp;
				return;
			}

			if (!m_pRefDiggerScene.lock()->IsInBetweenCellsX(currPos))
				m_pRefTransform.lock()->SetRelativePosition({ 0.0f,Digger::CellSize.y / 6.0f * m_VerticalScale,0.0f });
			else
			{
				// continue to the direction that player currently facing horizontally
				float dirScale{ m_PlayerFacingRight ? 1.0f : -1.0f };
				m_pRefTransform.lock()->SetRelativePosition({ Digger::CellSize.x / 4.0f * dirScale,0.0f,0.0f });
			}
		}
	}
}

void PlayerDigger::LateUpdate()
{
	// Clamp player to the play area
	auto transform = GetTransform();
	glm::fvec3 currPos{ transform->GetPosition() };
	glm::fvec3 newPos{};
	newPos.z = currPos.z;
	const float playAreaX{ Digger::PlayArea.x / 2.0f };
	const float playAreaY{ Digger::PlayArea.y / 2.0f };
	const float halfPlayerSizeX{ m_PlayerActualSize.x / 2.0f };
	const float halfPlayerSizeY{ m_PlayerActualSize.y / 2.0f };

	bool isOutLeft{ currPos.x - halfPlayerSizeX < -playAreaX };
	bool isOutRight{ currPos.x + halfPlayerSizeX > playAreaX };
	bool isOutTop{ currPos.y + halfPlayerSizeY > playAreaY };
	bool isOutBottom{ currPos.y - halfPlayerSizeY < -playAreaY };

	if (!isOutLeft && !isOutRight && !isOutTop && !isOutBottom)
		return;

	newPos.x = ((playAreaX - halfPlayerSizeX) * float(isOutLeft) * -1) + ((playAreaX - halfPlayerSizeX) * float(isOutRight));

	newPos.y = ((playAreaY - halfPlayerSizeY) * float(isOutBottom) * -1) + ((playAreaY - halfPlayerSizeY) * float(isOutTop));

	if (isOutLeft || isOutRight)
		transform->SetPosition(newPos.x, currPos.y, currPos.z);
	if (isOutTop || isOutBottom)
	{
		auto animator = m_pAnimator.lock();
		transform->SetPosition(currPos.x, newPos.y, currPos.z);
	}
}

void PlayerDigger::MoveHorizontal(float value)
{
	//const glm::fvec3 worldRight{ 1.0f,0.0f,0.0f };

	//const float deltaSeconds{ GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };
	//m_pRefTransform.lock()->SetRelativePosition({ Digger::CellSize.x / 4.0f * worldRight.x * value,0.0f,0.0f });

	if (!m_HasMovedHorizontal)
		m_HasMovedHorizontal = true;

	m_HorizontalScale = value;

	const bool isFacingLeft{ value < 1.0f };

	m_pSpriteRenderer.lock()->SetFlipY(isFacingLeft);
	const auto animator = m_pAnimator.lock();
	animator->SetBool("PlayerUp", false);
	animator->SetBool("PlayerDown", false);
}

void PlayerDigger::MoveVertical(float value)
{
	//glm::fvec3 worldUp{ 0.0f,1.0f,0.0f };
	//m_pMovement.lock()->AddMovementInput(worldUp, value);
	if (!m_HasMovedVertical)
		m_HasMovedVertical = true;

	m_VerticalScale = value;

	const auto animator = m_pAnimator.lock();
	const bool playerGoesUp{ value > 0.0f };

	animator->SetBool("PlayerUp", playerGoesUp);
	animator->SetBool("PlayerDown", !playerGoesUp);
}

void PlayerDigger::Shoot()
{
	
}


bool PlayerDigger::IsTouchingRimPlayArea()
{
	return false;
}
