#include "pch.h"
#include "Gold.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "BoxCollider2D.h"
#include "PlayerDigger.h"
#include "PickUps.h"
#include "Digger.h"
#include "Animator.h"
#include "Movement.h"

void Gold::Awake()
{
	using namespace dae;
	auto moneybag = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/MoneyBag.tga", "MoneyBag");
	auto moneybagDestroyed = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/MoneyBag_Destroyed.tga", "MoneyBag_Destroyed");

	auto idle = Sprite::CreateSpriteSheet(moneybag, {}, { 32.0f,30.0f }, 4, 0.0f);
	m_pMoneyBagIdleSprite = std::move(idle[0]);
	m_pMoneyBagDropSprite = { std::move(idle[3]) };

	m_pMoneyBagShakingSprites = Sprite::CreateSpriteSheet(moneybag, { 32.0f,0.0f }, { 32.0f,30.0f }, 2, 0.0f);

	m_pMoneyDestroyedSprites = Sprite::CreateSpriteSheet(moneybagDestroyed, {}, { 32.0f,30.0f }, 3, 0.0f);

	auto spriteRenderer = CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(m_pMoneyBagIdleSprite, true);
	m_pRefSpriteRenderer = spriteRenderer;

	auto shakingClip = AnimationClip::Create(GetShared<Gold>());
	shakingClip->AddProperty(spriteRenderer, m_pMoneyBagShakingSprites);
	shakingClip->SetSampleRate(2);
	m_pRefMoneyBagShakingAnimClip = shakingClip;

	auto shattered = AnimationClip::Create(GetShared<Gold>());
	shattered->AddProperty(spriteRenderer, m_pMoneyDestroyedSprites);
	shattered->SetSampleRate(4);
	m_pRefMoneyBagShatteredAnimClip = shattered;

	auto dropping = AnimationClip::Create(GetShared<Gold>());
	dropping->AddProperty(spriteRenderer, m_pMoneyBagDropSprite);
	m_pRefMoneyBagDropAnimClip = dropping;
	
	auto animator = CreateComponent<Animator>();
	animator->SetDefaultAnimClip(shakingClip);
	animator->AddTransition(shakingClip, "IsDropping", dropping, false);
	animator->AddTransition(dropping, "IsImpact", shattered, true);
	
	animator->SetActive(false);
	m_pRefAnimator = animator;

	auto pickUps = CreateComponent<PickUps>();
	pickUps->SetGainScore(500);
	m_pRefPickUps = pickUps;

	const auto& scale{ GetTransform()->GetScale() };
	m_ActualSize = { scale.x * 32.0f,scale.y * 30.f };

	m_pRefDiggerScene = GetScene()->GetShared<Digger>();

	auto movement = CreateComponent<Movement>();
	movement->SetAcceleration(0.0f);
	movement->SetInitialVelocity(400.0f);
}

void Gold::Start()
{
	const auto mainPlayer = GetScene()->GetGameObjectOfType<PlayerDigger>();
	if (mainPlayer)
	{
		m_pRefMainPlayer = mainPlayer;
	}
}

void Gold::Update()
{
	switch (m_State)
	{
	case GoldState::CheckingForDrop:	CheckForBelowCellAndPlayer();	break;
	case GoldState::IsCountingDown:		CountDownDropping();			break;
	case GoldState::IsDropping:			PlayDropping();					break;
	case GoldState::IsShattering:		PlayShattering();				break;
	}
	//CheckForBelowCellAndPlayer();
	//CountDownDropping();
	//PlayDropping();
	//PlayShattering();
}

void Gold::OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& colliders)
{
	colliders;
	//auto player = std::static_pointer_cast<GameObject>(m_pRefMainPlayer.lock());
	//for (const auto& collider : colliders)
	//{
	//	if (collider.lock()->GetGameObject() == player)
	//	{
	//		GetTransform()->SetRelativePosition({ 30.0f,0.0f,0.0f });
	//	}
	//}
}

void Gold::CountDownDropping()
{
	//if (!m_ReadyForDrop) return;

	const float deltaSeconds{ m_pRefDiggerScene.lock()->GetSceneContext().pGameTime->GetDeltaSeconds() };

	if (m_IsPlayerBelowMe)
	{
		if (!m_pRefAnimator.expired())
		{
			auto animator = m_pRefAnimator.lock();
			animator->SetActive(true);
		}

		m_TimeCount += deltaSeconds;
		if (m_TimeCount >= m_TimeBeforeDrop)
		{
			m_TimeCount -= m_TimeBeforeDrop;
			m_IsDropping = true;
			m_pRefAnimator.lock()->SetBool("IsDropping", true);
			m_State = GoldState::IsDropping;
		}
	}
	else
	{
		m_State = GoldState::IsDropping;
		auto animator = m_pRefAnimator.lock();
		animator->SetActive(true);
		animator->SetBool("IsDropping", true);
	}

}

void Gold::PlayDropping()
{
	//if (!m_IsDropping) return;

	if (!m_pRefMovement.expired())
	{
		auto movement = m_pRefMovement.lock();
		movement->AddMovementInput({ 0.0f,-1.0f }, 1.0f);
		m_ReadyForDrop = false;
	}

	// Check for collision with the level
	// 1. if we only drop for 1 cell then everything is fine
	// else the moneybag is exploded and then become pick-able
	if (!m_pRefDiggerScene.expired())
	{
		auto scene = m_pRefDiggerScene.lock();
		const auto& currPos{ GetTransform()->GetPosition() };

		bool hasTouchedTheGround{};
		// Check first if the next one is out of play area
		const bool isOutOfBound{ currPos.y - m_ActualSize.y / 2.0f < -Digger::PlayArea.y / 2.0f };

		if(!isOutOfBound)
		{
			const auto bottomBlockId{ scene->GetBlockIdFromWorldPos({currPos.x,currPos.y + m_ActualSize.y / 2.0f,1.0f}) };
			if (bottomBlockId == Digger::BlockId::Level)
				hasTouchedTheGround = true;
		}

		if(isOutOfBound || hasTouchedTheGround)
		{
			if(!m_pRefAnimator.expired() && !m_pRefSpriteRenderer.expired())
			{
				if(m_ShouldSurvive)
				{
					auto animator = m_pRefAnimator.lock();
					animator->SetActive(false);
					auto spriteRenderer = m_pRefSpriteRenderer.lock();
					spriteRenderer->SetSprite(m_pMoneyBagIdleSprite,false);
				}
				else
				{
					auto animator = m_pRefAnimator.lock();
					animator->SetActive(true);
					animator->SetTrigger("IsImpact");
					m_State = GoldState::IsShattering;
				}
			}

			if (isOutOfBound)
				ClampToPlayArea();
			else
				ClampToTheGround();
		}
	}

}

void Gold::CheckForBelowCellAndPlayer()
{
	//if (m_ReadyForDrop) return;

	if (!m_pRefDiggerScene.expired())
	{
		auto scene = m_pRefDiggerScene.lock();
		const auto& currPos{ GetTransform()->GetPosition() };

		// find the bottom node of the gold
		// one row below the current

		const uint32_t currIndex{ scene->GetBlockIndexFromWorldPos(currPos) };
		const uint32_t bottomIndex{ currIndex + Digger::MaxCellCol };
		const uint32_t nextBottomIndex{ bottomIndex + Digger::MaxCellCol };

		auto* cellsemantics{ scene->GetCurrentCellSemantics() };

		const auto nextBottomCellId{ Digger::BlockId(cellsemantics[nextBottomIndex]) };
		m_ShouldSurvive = nextBottomCellId == Digger::BlockId::Level ? true : false;

		Digger::BlockId bottomCellSemantics{ Digger::BlockId(cellsemantics[bottomIndex]) };
		if (bottomCellSemantics == Digger::BlockId::None)
		{
			// Start the drop animation

			// 1. drop immediately if there's no player underneath
			// 2. start shaking animation then drop

			if (!m_pRefMainPlayer.expired())
			{
				auto player = m_pRefMainPlayer.lock();
				const auto& playerPos{ player->GetTransform()->GetPosition() };
				const glm::fvec2 bottomCellPos{ currPos.x,currPos.y - Digger::CellSize.y };
				bool isNearXPlane{ abs(playerPos.x - bottomCellPos.x) < 0.01f };
				bool isNearYPlane{ abs(playerPos.y - bottomCellPos.y) < 0.01f };

				if (isNearXPlane && isNearYPlane)
				{
					// Start Dropping
					m_IsPlayerBelowMe = true;
				}
			}

			m_State = GoldState::IsCountingDown;
		}
	}
}

void Gold::PlayShattering()
{
	if(m_pRefMoneyBagShatteredAnimClip.lock()->IsFinishedPlaying())
	{
		m_pRefAnimator.lock()->SetActive(false);
		m_IsPickable = true;
	}
}

void Gold::ClampToPlayArea()
{
	// Clamp player to the play area
	auto transform = GetTransform();
	const glm::fvec3& currPos{ transform->GetPosition() };
	glm::fvec3 newPos{};
	newPos.z = currPos.z;
	const float playAreaX{ Digger::PlayArea.x / 2.0f };
	const float playAreaY{ Digger::PlayArea.y / 2.0f };
	const float halfPlayerSizeX{ m_ActualSize.x / 2.0f };
	const float halfPlayerSizeY{ m_ActualSize.y / 2.0f };

	const bool isOutLeft{ currPos.x - halfPlayerSizeX < -playAreaX };
	const bool isOutRight{ currPos.x + halfPlayerSizeX > playAreaX };
	const bool isOutTop{ currPos.y + halfPlayerSizeY > playAreaY };
	const bool isOutBottom{ currPos.y - halfPlayerSizeY < -playAreaY };

	if (!isOutLeft && !isOutRight && !isOutTop && !isOutBottom)
		return;

	newPos.x = ((playAreaX - halfPlayerSizeX) * float(isOutLeft) * -1) + ((playAreaX - halfPlayerSizeX) * float(isOutRight));

	newPos.y = ((playAreaY - halfPlayerSizeY) * float(isOutBottom) * -1) + ((playAreaY - halfPlayerSizeY) * float(isOutTop));

	if (isOutLeft || isOutRight)
		transform->SetPosition(newPos.x, currPos.y, currPos.z);
	if (isOutTop || isOutBottom)
		transform->SetPosition(currPos.x, newPos.y, currPos.z);
}

void Gold::ClampToTheGround()
{
	const auto& currPos{ GetTransform()->GetPosition() };
	auto scene = m_pRefDiggerScene.lock();
	// Clamp to the ground
	const auto currBlockIndex{ scene->GetBlockIndexFromWorldPos(currPos) };
	const auto currBlockPos{ scene->GetWorldPosFromIndex(currBlockIndex) };
	m_ReadyForDrop = false;
	m_IsDropping = false;
	GetTransform()->SetPosition({ currBlockPos.x,currBlockPos.y,currPos.z });
}
