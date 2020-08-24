#include "pch.h"
#include "Nobblin.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "AnimationClip.h"
#include "Animator.h"
#include "NavAgent.h"
#include "BoxCollider2D.h"
#include "PlayerDigger.h"

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
	collider2D->SetSize({ scale.x * 32.0f,scale.y * 30.0f });
	collider2D->SetIsTrigger(true);
}

void Nobblin::Start()
{
	const auto player = GetScene()->GetGameObjectOfType<PlayerDigger>();
	if (player)
		m_pRefMainPlayer = player;
	
	//auto path = m_pRefNavAgent.lock()->RequestPath(player->GetTransform()->GetPosition());
}

void Nobblin::Update()
{
	
}
