#include "pch.h"
#include "Nobblin.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "AnimationClip.h"
#include "Animator.h"
#include "NavAgent.h"
#include "BoxCollider2D.h"
#include "PlayerDigger.h"

Nobblin::Nobblin()
	: m_RequestUpdateInderval(1.0f)
	, m_UpdateTimeCount()
	, m_TimeCountX()
	, m_TimeCountY()
	, m_MoveTimeLimitX(0.5f)
	, m_MoveTimeLimitY(1.0f)
	, m_JustAskForPath()
{
}

void Nobblin::Awake()
{
	using namespace dae;
	auto nobblinWalking = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Nobblin.tga", "Nobblin");
	m_pWalkingSprites = Sprite::CreateSpriteSheet(nobblinWalking, {}, { 32.0f,30.f }, 3, 0.0f);
	auto deadSprite = Sprite::CreateSpriteSheet(nobblinWalking, { 32.0f * 3.0f,0.0f }, { 32.0f,30.0f }, 1, 0.0f);
	m_pDeadSprite = std::move(deadSprite[0]);

	auto spriteRenderer = CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(m_pWalkingSprites[0], true);

	auto walkingClip = AnimationClip::Create(GetShared<Nobblin>());
	walkingClip->AddProperty(spriteRenderer, m_pWalkingSprites);
	walkingClip->SetSampleRate(10);

	auto animator = CreateComponent<Animator>();
	animator->SetDefaultAnimClip(walkingClip);

	auto navAgent = CreateComponent<NavAgent>();
	navAgent->SetCellDimension(Digger::CellSize);
	navAgent->SetCellRowNColumn(10, 15);
	navAgent->SetMaxBound(Digger::PlayArea);

	auto diggerScene = GetScene()->GetShared<Digger>();
	m_pRefDiggerScene = diggerScene;
	auto* cellSemantics{ diggerScene->GetCurrentCellSemantics() };
	std::array<bool, Digger::CellCount> cellType{};
	for (uint32_t i = 0; i < Digger::CellCount; ++i)
	{
		int semantics{ cellSemantics[i] };
		cellType[i] = semantics == 4 || semantics == 5 || semantics == 0;
	}
	navAgent->CreateCustomCells(cellType.data(), 15, 10);

	m_pRefNavAgent = navAgent;

	const auto& scale{ GetTransform()->GetScale() };
	auto collider2D = CreateComponent<BoxCollider2D>();
	collider2D->SetSize({ 32.0f / scale.x,30.0f / scale.y });
	collider2D->SetIsTrigger(true);
}

void Nobblin::Start()
{
	const auto player = GetScene()->GetGameObjectOfType<PlayerDigger>();
	if (player)
	{
		m_pRefMainPlayer = player;
		const auto& playerPos = player->GetTransform()->GetPosition();
		m_FutureWayPoints = m_pRefNavAgent.lock()->RequestPath(playerPos);
	}
}

void Nobblin::Update()
{
	const float deltaSeconds{ GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };

	m_UpdateTimeCount += m_RequestUpdateInderval;
	if (m_UpdateTimeCount >= m_RequestUpdateInderval)
	{
		m_UpdateTimeCount -= m_RequestUpdateInderval;

		//if (m_FutureWayPoints.valid())
		//{
		//	const auto newCellType{ GetNewCellUpdate() };
		//	const auto& playerPos{ m_pRefMainPlayer.lock()->GetTransform()->GetPosition() };
		//	auto navAgnet{ m_pRefNavAgent.lock() };
		//	navAgnet->ResetCellsState(newCellType.data(), MAX_CELL_COUNT);
		//	m_FutureWayPoints = navAgnet->RequestPath(playerPos);
		//}
	}


	// Walk along the path
	MoveToPath();
}

void Nobblin::OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& otherColliders)
{
	otherColliders;
}

std::array<bool, MAX_CELL_COUNT> Nobblin::GetNewCellUpdate()
{
	std::array<bool, MAX_CELL_COUNT> cellType{};
	auto scene = m_pRefDiggerScene.lock();
	const auto* cellSemantics{ scene->GetCurrentCellSemantics() };
	for (uint32_t i = 0; i < Digger::CellCount; ++i)
	{
		int semantics{ cellSemantics[i] };
		cellType[i] = semantics == 4 || semantics == 5 || semantics == 0;
	}
	return cellType;
}

void Nobblin::MoveToPath()
{
	const float deltaSeconds{ GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };

	if (m_FutureWayPoints.valid())
	{
		const auto path = m_FutureWayPoints.get();
		m_WayPoints = { path.begin(),path.end() };
		m_JustAskForPath = true;
	}

	else if (!m_JustAskForPath)
	{
		const auto newCellType{ GetNewCellUpdate() };
		const auto& playerPos{ m_pRefMainPlayer.lock()->GetTransform()->GetPosition() };
		auto navAgnet{ m_pRefNavAgent.lock() };
		navAgnet->ResetCellsState(newCellType.data(), MAX_CELL_COUNT);
		m_FutureWayPoints = navAgnet->RequestPath(playerPos);
	}

	if (m_JustAskForPath)
	{
		if(m_WayPoints.empty())
		{
			m_JustAskForPath = false;
			return;
		}
		glm::fvec2 wayPoint{ GetCurrentpath(m_WayPoints) };

		m_TimeCountX += deltaSeconds;
		if (m_TimeCountX >= m_MoveTimeLimitX)
		{
			m_TimeCountX -= m_MoveTimeLimitX;
			GetTransform()->SetPosition({ wayPoint.x, wayPoint.y, 0.0f });

			auto it = std::remove(m_WayPoints.begin(), m_WayPoints.end(), wayPoint);
			m_WayPoints.erase(it, m_WayPoints.end());
		}
	}
}

glm::fvec2 Nobblin::GetCurrentpath(const std::vector<glm::fvec2>& path)
{
	const glm::fvec2 currPos{ GetTransform()->GetPosition() };
	glm::fvec2 wayPoint{};
	// Bad
	for (const auto& point : path)
	{
		const float length{ glm::length((point - currPos)) };
		if (length > 1.0f)
		{
			wayPoint = point;
			break;
		}
	}

	return wayPoint;
}
