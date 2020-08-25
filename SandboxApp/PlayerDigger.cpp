#include "pch.h"
#include "PlayerDigger.h"
#include "Sprite.h"
#include "InputComponent.h"
#include "Animator.h"
#include "BoxCollider2D.h"
#include "Digger.h"
#include "Movement.h"
#include "Bullet.h"
#include "Nobblin.h"

dae::EventDispatcher<int> PlayerDigger::OnDeath{};

PlayerDigger::PlayerDigger()
	: GameObject()
	, m_MovementSpeed()
	, m_MoveTimeLimitX(0.08f)
	, m_MoveTimeLimitY(0.08f)
	, m_PlayerFacing(PlayerDirection::Right)
	, m_TimeCountX()
	, m_TimeCountY()
	, m_HorizontalScale()
	, m_VerticalScale()
	, m_ReloadTime(6.0f)
	, m_ReloadTimeCount()
	, m_PlayerFacingRight(true)
	, m_PlayerFacingUp(true)
	, m_ShellEmpty()
	, m_HasMovedHorizontal()
	, m_HasMovedVertical()
	, m_IsDead()
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

glm::fvec2 PlayerDigger::GetPointForward()
{
	const auto& currPos{ GetTransform()->GetPosition() };
	glm::fvec2 forward{};
	switch (m_PlayerFacing)
	{
	case PlayerDirection::Right: forward = { currPos.x + m_PlayerActualSize.x / 2.0f,currPos.y }; break;
	case PlayerDirection::Left: forward = { currPos.x - m_PlayerActualSize.x / 2.0f,currPos.y }; break;
	case PlayerDirection::Down: forward = { currPos.x,currPos.y - m_PlayerActualSize.y / 2.0f }; break;
	case PlayerDirection::Up: forward = { currPos.x, currPos.y + m_PlayerActualSize.y / 2.0f }; break;
	}

	return forward;
}

glm::fvec2 PlayerDigger::GetForward()
{
	glm::fvec2 forward{};
	switch (m_PlayerFacing)
	{
	case PlayerDirection::Right: forward = { 1.0f,0.0f }; break;
	case PlayerDirection::Left: forward = { -1.0f,0.0f }; break;
	case PlayerDirection::Down: forward = { 0.0f,-1.0f }; break;
	case PlayerDirection::Up: forward = { 0.0f,1.0f }; break;
	}

	return forward;
}

void PlayerDigger::CallDeath()
{
	PlayDeath(0);
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
	m_pRefDeadAnimClip = playerDead;
	m_pRefMainIdleAnimClip = playerIdle;
	// Initialize AnimClip for Empty shell

	auto idle = AnimationClip::Create(GetShared<PlayerDigger>());
	idle->AddProperty(spriteRenderer, m_pWalkStraightX);
	idle->SetSampleRate(10);
	auto walkUp = AnimationClip::Create(GetShared<PlayerDigger>());
	walkUp->AddProperty(spriteRenderer, m_pWalkUpX);
	walkUp->SetSampleRate(10);
	auto walkDown = AnimationClip::Create(GetShared<PlayerDigger>());
	walkDown->AddProperty(spriteRenderer, m_PWalkDownX);
	walkDown->SetSampleRate(10);

	m_pRefShellEmptyIdleAnimClip = idle;


	m_pAnimator = CreateComponent<Animator>();
	const auto animator = m_pAnimator.lock();
	animator->AddTransition(playerIdle, "PlayerUp", playerWalkUp, false);
	animator->AddTransition(playerIdle, "PlayerDown", playerWalkDown, false);
	animator->AddTransition(playerIdle, "IsDead", playerDead, true);
	animator->SetDefaultAnimClip(playerIdle);

	animator->AddTransition(idle, "PlayerUp", walkUp, false);
	animator->AddTransition(idle, "PlayerDown", walkDown, false);
	animator->AddTransition(idle, "IsDead", playerDead, true);

	// Initialize Controller
	auto inputComponent = CreateComponent<InputComponent>();
	inputComponent->BindAxis("Horizontal", GetShared<PlayerDigger>(), &PlayerDigger::MoveHorizontal);
	inputComponent->BindAxis("Vertical", GetShared<PlayerDigger>(), &PlayerDigger::MoveVertical);
	inputComponent->BindAction("Shoot", InputEvent::IE_Pressed, GetShared<PlayerDigger>(), &PlayerDigger::Shoot);

	// Initialize Collider
	auto collider = CreateComponent<BoxCollider2D>();
	collider->SetSize({ 32.0f,30.0f });
	collider->EnableDebugDraw(true);

	const auto& scale{ m_pRefTransform.lock()->GetScale() };
	m_PlayerActualSize.x = scale.x * 32.0f;
	m_PlayerActualSize.y = scale.y * 30.0f;

	m_pRefDiggerScene = GetScene()->GetShared<Digger>();
	// Initialize Player speed one click per pixel

	//OnDeath.RegisterObserver<PlayerDigger>(GetShared<PlayerDigger>(), &PlayerDigger::PlayDeath);
}

void PlayerDigger::Update()
{
	const float deltaSeconds{ GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };

	if (m_IsDead)
	{
		if (m_pRefDeadAnimClip.lock()->IsFinishedPlaying())
			OnDeath.Broadcast(0);
		return;
	}

	if (m_ShellEmpty)
	{
		m_ReloadTimeCount += deltaSeconds;
		if (m_ReloadTimeCount >= m_ReloadTime)
		{
			m_ReloadTimeCount -= m_ReloadTime;
			m_pAnimator.lock()->SetDefaultAnimClip(m_pRefMainIdleAnimClip.lock());
			m_ShellEmpty = false;
		}
	}


	if (m_HasMovedHorizontal)
	{
		m_TimeCountX += deltaSeconds;
		const auto& currPos = m_pRefTransform.lock()->GetPosition();
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

			if (!m_pRefDiggerScene.lock()->IsInBetweenCellsY(currPos))
				m_pRefTransform.lock()->SetRelativePosition({ Digger::CellSize.x / 4.0f * m_HorizontalScale,0.0f,0.0f });
			else
			{
				// continue to the direction that player currently facing vertically
				float dirScale{ m_PlayerFacingUp ? 1.0f : -1.0f };
				m_pRefTransform.lock()->SetRelativePosition({ 0.0f, Digger::CellSize.y / 6.0f * dirScale, 0.0f });
			}

		}

		m_pRefDiggerScene.lock()->UpdatePlayerMovement(currPos);
		return;
	}

	if (m_HasMovedVertical)
	{
		m_TimeCountY += deltaSeconds;
		const auto& currPos = m_pRefTransform.lock()->GetPosition();
		if (m_TimeCountY >= m_MoveTimeLimitY)
		{
			// check for cell collision
			m_TimeCountY -= m_MoveTimeLimitY;
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
				const float dirScale{ m_PlayerFacingRight ? 1.0f : -1.0f };
				m_pRefTransform.lock()->SetRelativePosition({ Digger::CellSize.x / 4.0f * dirScale,0.0f,0.0f });
			}
		}

		m_pRefDiggerScene.lock()->UpdatePlayerMovement(currPos);
	}
}

void PlayerDigger::LateUpdate()
{
	ClampIfGold();

	// Clamp player to the play area
	auto transform = GetTransform();
	const glm::fvec3& currPos{ transform->GetPosition() };
	//m_pRefDiggerScene.lock()->UpdatePlayerMovement(currPos);
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
		transform->SetPosition(currPos.x, newPos.y, currPos.z);

	//const auto& currPos{ GetTransform()->GetPosition() };
}

void PlayerDigger::OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& otherColliders)
{
	if (!m_IsDead)

	{
		for (const auto& collider : otherColliders)
		{
			// TODO: Add gold collision when dropped
			if (std::dynamic_pointer_cast<Nobblin>(collider.lock()->GetGameObject()))
			{
				m_IsDead = true;
				auto animator = m_pAnimator.lock();
				animator->SetBool("PlayerUp", false);
				animator->SetBool("PlayerDown", false);
				animator->SetTrigger("IsDead");
			}
		}
	}

}

void PlayerDigger::MoveHorizontal(float value)
{
	//const glm::fvec3 worldRight{ 1.0f,0.0f,0.0f };

	//const float deltaSeconds{ GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };
	//m_pRefTransform.lock()->SetRelativePosition({ Digger::CellSize.x / 4.0f * worldRight.x * value,0.0f,0.0f });

	if (m_IsDead)
		return;

	if (!m_HasMovedHorizontal)
		m_HasMovedHorizontal = true;

	m_HorizontalScale = value;

	const bool isFacingLeft{ value < 1.0f };

	m_PlayerFacing = isFacingLeft ? PlayerDirection::Left : PlayerDirection::Right;

	m_pSpriteRenderer.lock()->SetFlipY(isFacingLeft);
	const auto animator = m_pAnimator.lock();
	animator->SetBool("PlayerUp", false);
	animator->SetBool("PlayerDown", false);
}

void PlayerDigger::MoveVertical(float value)
{
	//glm::fvec3 worldUp{ 0.0f,1.0f,0.0f };
	//m_pMovement.lock()->AddMovementInput(worldUp, value);

	if (m_IsDead)
		return;

	if (!m_HasMovedVertical)
		m_HasMovedVertical = true;

	m_VerticalScale = value;

	const auto animator = m_pAnimator.lock();
	const bool playerGoesUp{ value > 0.0f };

	m_PlayerFacing = playerGoesUp ? PlayerDirection::Up : PlayerDirection::Down;

	animator->SetBool("PlayerUp", playerGoesUp);
	animator->SetBool("PlayerDown", !playerGoesUp);
}

void PlayerDigger::Shoot()
{
	if (m_IsDead) return;
	
	if (m_ShellEmpty)
		return;

	const auto& position{ GetTransform()->GetPosition() };
	auto bullet = m_pRefDiggerScene.lock()->Instantiate<Bullet>(position, {}, { 1.3f,1.3f });
	auto nobblin = m_pRefDiggerScene.lock()->Instantiate<Nobblin>();
	glm::fvec2 dir{};

	// Please don't ask why i did this
	switch (m_PlayerFacing)
	{
	case PlayerDirection::Right:	dir = { 1.0f,0.0f };	break;
	case PlayerDirection::Left:		dir = { -1.0f,0.0f };	break;
	case PlayerDirection::Down:		dir = { 0.0f,-1.0f };	break;
	case PlayerDirection::Up:		dir = { 0.0f,1.0f };	break;
	}

	bullet->SetLaunchDirection(dir);
	m_ShellEmpty = true;
	m_pAnimator.lock()->SetDefaultAnimClip(m_pRefShellEmptyIdleAnimClip.lock());
}

void PlayerDigger::PlayDeath(int)
{
	m_IsDead = true;
	auto animator = m_pAnimator.lock();
	animator->SetBool("PlayerUp", false);
	animator->SetBool("PlayerDown", false);
	animator->SetTrigger("IsDead");
}


bool PlayerDigger::IsTouchingRimPlayArea()
{
	return false;
}

void PlayerDigger::ClampIfGold()
{
	if (!m_pRefDiggerScene.expired())
	{
		if (m_PlayerFacing == PlayerDirection::Down
			|| m_PlayerFacing == PlayerDirection::Left
			|| m_PlayerFacing == PlayerDirection::Right)
		{
			auto scene = m_pRefDiggerScene.lock();
			const auto& currPos{ GetTransform()->GetPosition() };
			auto forward{ glm::fvec3{GetForward(),currPos.z} };
			//const float offset{ 1.1f };
			//forward.y -= offset;
			const glm::fvec3 pointInfront{ currPos + forward * Digger::CellSize.x };
			const auto blockId{ scene->GetBlockIdFromWorldPos(pointInfront) };
			if (blockId == Digger::BlockId::Gold)
			{
				const auto blockIndex{ scene->GetBlockIndexFromWorldPos(pointInfront) };
				const auto blockPos{ scene->GetWorldPosFromIndex(blockIndex) };
				const glm::fvec3 invertForward{ -forward.x,-forward.y,forward.z };
				glm::fvec3 clampPos{ blockPos + invertForward * Digger::CellSize.x };
				clampPos.z = currPos.z;
				GetTransform()->SetPosition(clampPos);
			}
		}
		//if (m_PlayerFacing == PlayerDirection::Down)
		//{
		//	auto scene = m_pRefDiggerScene.lock();
		//	auto forward{ glm::fvec3{GetPointForward(),0.0f} };
		//	const float offset{ 1.0f };
		//	forward.y -= offset;

		//	const auto blockId{ scene->GetBlockIdFromWorldPos(forward) };
		//	if (blockId == Digger::BlockId::Gold)
		//	{
		//		const auto blockIndex{ scene->GetBlockIndexFromWorldPos(forward) };
		//		auto blockPos{ scene->GetWorldPosFromIndex(blockIndex) };
		//		blockPos.y += Digger::CellSize.y;
		//		blockPos.z = 0.0f;
		//		GetTransform()->SetPosition(blockPos);
		//	}
		//}
	}

}
