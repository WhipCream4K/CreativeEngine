#include "pch.h"
#include "Digger.h"
#include "InputManager.h"
#include "PlayerDigger.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Gold.h"
#include "Jewel.h"
#include "Text.h"
#include "TextRenderer.h"
#include "ScoreManager.h"
#include "Nobblin.h"
#include <fstream>
#include <iostream>

#include "EnemySpawnManager.h"

//#define BINARY_WRITER

const glm::fvec2 Digger::PlayArea{ 975.0f - 60.0f,572 - 40.0f };
const glm::fvec2 Digger::CellSize{ PlayArea.x / 15.0f , PlayArea.y / 10.0f };
const glm::fvec2 Digger::ObjectRelativeScaling{ CellSize.x / 32.0f,CellSize.y / 30.0f };
uint32_t Digger::PlayerLives{ 2 };

Digger::Digger()
	: Scene("Digger")
	, m_pBigSprites()
	, m_pCellSemantics()
{
}

Digger::~Digger()
{
	if (m_pCellSemantics)
	{
		delete[] m_pCellSemantics;
		m_pCellSemantics = nullptr;
	}
}

bool Digger::IsInBetweenCellsX(const glm::fvec3& position)
{
	const float remapPos{ position.x + (PlayArea.x / 2.0f) };
	const int cellCol{ int(remapPos / CellSize.x) };
	const float actualCellPosX{ (CellSize.x * float(cellCol)) + CellSize.x / 2.0f };
	return abs(actualCellPosX - remapPos) > 0.01f;
}

bool Digger::IsInBetweenCellsY(const glm::fvec3& position)
{
	const float remapPos{ PlayArea.y / 2.0f + position.y };
	const int cellRow{ int(remapPos / CellSize.y) };
	const float actualCellPosY{ CellSize.y * float(cellRow) + CellSize.y / 2.0f };
	return abs(actualCellPosY - remapPos) > 0.01f;
}

void Digger::UpdatePlayerMovement(const glm::fvec2& pos)
{
	const int currCellCol{ int((pos.x + (PlayArea.x / 2.0f)) / CellSize.x) };
	const int currCellRow{ int(((PlayArea.y / 2.0f) - pos.y) / CellSize.y) };
	// Check if this cell is a level cell
	const uint32_t index{ uint32_t(currCellRow * int(MaxCellCol) + currCellCol) };
	const BlockId semantics{ BlockId(m_pCellSemantics[index]) };
	if (semantics != BlockId::None && semantics != BlockId::PlayerStart)
	{
		m_pCellSemantics[index] = char(BlockId::None);
		glm::fvec2 cellPos{};
		const glm::fvec2 offset{ CellSize.x / 2.0f,-CellSize.y / 2.0f };
		cellPos.x = (float(currCellCol) * CellSize.x + offset.x) - PlayArea.x / 2.0f;
		cellPos.y = PlayArea.y / 2.0f - (float(currCellRow) * CellSize.y - offset.y);

		InstantiatePath({ cellPos.x,cellPos.y,-2.0f }, ObjectRelativeScaling);
	}
}

Digger::BlockId Digger::GetBlockIdFromWorldPos(const glm::fvec3& position)
{
	uint32_t index{ GetBlockIndexFromWorldPos(position) };
	BlockId blockId{ BlockId(m_pCellSemantics[index]) };
	return blockId;
}

uint32_t Digger::GetBlockIndexFromWorldPos(const glm::fvec3& position)
{
	const int currCellCol{ int((position.x + (PlayArea.x / 2.0f)) / CellSize.x) };
	const int currCellRow{ int((PlayArea.y / 2.0f - position.y) / CellSize.y) };

	uint32_t index{ uint32_t(currCellRow * int(MaxCellCol) + currCellCol) };

	index = index > CellCount ? CellCount : index;
	return index;
}

glm::fvec3 Digger::GetWorldPosFromIndex(uint32_t index)
{
	int currRow{ int(index / MaxCellCol) - 1 };
	int currCol{ int(index % MaxCellCol) - 1};

	currRow = currRow < 0 ? 0 : currRow;
	currRow = currRow > MaxCellRow ? int(MaxCellRow) : currRow;

	currCol = currCol < 0 ? 0 : currCol;
	currCol = currCol > MaxCellCol ? int(MaxCellCol) : currCol;
	
	const glm::fvec2 offset{ CellSize.x / 2.0f, -CellSize.y / 2.0f };
	glm::fvec3 cellPos{};
	cellPos.z = -2.0f;
	cellPos.x = (float(currCol) * CellSize.x + offset.x) - PlayArea.x / 2.0f;
	cellPos.y = PlayArea.y / 2.0f - (float(currRow) * CellSize.y - offset.y);

	return cellPos;
}

void Digger::SceneInitialize()
{
	using namespace dae;

	// Background ->	Layer -3
	// Path ->			Layer -2
	// PickUps ->		Layer -1
	// Dynamic ->		Layer 0

	// Initialize Dark sky
	{
		auto blackPixel = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Black.png", "Black");
		m_pBackgroundSprite = Sprite::Create(blackPixel);
		auto backGround = CreateGameObject({ 0.0f,0.0f,-3.0f }, {}, { 1280.0f,720.0f });
		auto backgroundRenderer = backGround->CreateComponent<SpriteRenderer>();
		backgroundRenderer->SetSprite(m_pBackgroundSprite, true);
	}

	// Initialize background
	{
		auto level0background = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Level_0_BG.tga", "Level_0_BG");
		auto backgroundSprite = Sprite::Create(level0background);
		m_pBigSprites.try_emplace(0u, backgroundSprite);
		auto background = CreateGameObject({ 0.0f,0.0f,-3.0f });
		background->GetTransform()->SetScale(1.3f, 1.3f);
		auto renderer = background->CreateComponent<SpriteRenderer>();
		renderer->SetSprite(backgroundSprite, true);
	}

	// Test Text
	{
		auto fontData = ResourceManager::Load<DefaultFontData>("./Resources/Digger/Font/Diggerfont-Regular.ttf", "DiggerFont");
		//m_pDiggerFont = Text::Create(fontData, "HELLO_WORLD_", 100);
		//auto gameObject = CreateGameObject({ -100.0f,300.0f,-1.0f });
		//auto textRenderer = gameObject->CreateComponent<TextRenderer>();
		//textRenderer->SetText(m_pDiggerFont);
		//textRenderer->SetForegroundColor({ 0.0f,1.0f,0.0f,1.0f });
	}

	// Initialize walk path
	{
		std::string walkDir{};
		for (int i = 0; i < 5; ++i)
		{
			switch (PathDirection(i))
			{
			case PathDirection::None:	walkDir = "Normal";	break;
			case PathDirection::Right:	walkDir = "Right";	break;
			case PathDirection::Left:	walkDir = "Left";	break;
			case PathDirection::Up:		walkDir = "Up";		break;
			case PathDirection::Down:	walkDir = "Down";	break;
			}

			auto texture = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/WalkPath/WalkPath_" + walkDir + ".tga", "WalkPath_" + walkDir);
			auto sprites = Sprite::Create(texture);
			m_pWalkSprites.try_emplace(PathDirection(i), sprites);
		}
	}


	std::ifstream inStream{ "./Resources/Digger/Level_0.bin", std::ios::in | std::ios::binary };


	const glm::fvec2 maxBound{ PlayArea.x,PlayArea.y };
	const glm::fvec2 cellSize{ maxBound.x / 15.0f,maxBound.y / 10.0f };
	glm::fvec3 boundStart{ -maxBound.x / 2.0f, maxBound.y / 2.0f,-1.0f };
	const glm::fvec3 cellOffSet{ cellSize.x / 2.0f,-cellSize.y / 2.0f,0.0f };
	const glm::fvec2 relativeScaling{ cellSize.x / 32.0f,cellSize.y / 30.0f };

	std::shared_ptr<GameObject> pSceneObject;
	std::shared_ptr<SpriteRenderer> pSpriteRenderer;
	int countLineX{};

	// read in level file
	if (inStream.is_open())
	{
		m_pCellSemantics = new char[CellCount];
		inStream.seekg(0);
		inStream.read(m_pCellSemantics, CellCount);
	}

	inStream.close();

	// Initialize Enemy SpawnManager
	auto spawnManager{ CreateGameObject<EnemySpawnManager>() };
	spawnManager->SetSpawnInterval(6.0f);
	spawnManager->SetSpawnLimit(5);

	for (uint32_t i = 0; i < CellCount; ++i)
	{
		BlockId semantics{ BlockId(m_pCellSemantics[i]) };

		switch (semantics)
		{
		case BlockId::None: SpawnPath(boundStart + cellOffSet, relativeScaling);

			//pSceneObject = CreateGameObject(boundStart + cellOffSet);
			//pSceneObject->GetTransform()->SetRelativePosition({ 0.0f,0.0f,-2.0f });
			//pSceneObject->GetTransform()->SetScale(relativeScaling.x, relativeScaling.y);
			//pSpriteRenderer = pSceneObject->CreateComponent<SpriteRenderer>();
			//pSpriteRenderer->SetSprite(m_pWalkSprites.at(PathDirection::None), true);

			break;
		case BlockId::Level:																break;
		case BlockId::Gold:

			pSceneObject = CreateGameObject<Gold>(boundStart + cellOffSet);
			pSceneObject->GetTransform()->SetScale(relativeScaling.x - 0.4f, relativeScaling.y - 0.4f);

			break;
		case BlockId::Jewel:

			pSceneObject = CreateGameObject<Jewel>(boundStart + cellOffSet);
			pSceneObject->GetTransform()->SetScale(relativeScaling.x - 0.4f, relativeScaling.y - 0.4f);

			break;

		case BlockId::PlayerStart:

			SpawnPath(boundStart + cellOffSet, relativeScaling);
			pSceneObject = CreateGameObject<PlayerDigger>(boundStart + cellOffSet);
			m_pRefMainPlayer = std::static_pointer_cast<PlayerDigger>(pSceneObject);
			pSceneObject->GetTransform()->SetScale(relativeScaling.x, relativeScaling.y);
			m_pRefMainPlayerPos = &pSceneObject->GetTransform()->GetPosition();

			break;

		case BlockId::EnemySpawn:

			SpawnPath(boundStart + cellOffSet, relativeScaling);
			spawnManager->SetSpawnPoint(boundStart + cellOffSet);
			
			break;
		}

		if (countLineX % 14 == 0 && countLineX != 0)
		{
			countLineX = 0;
			boundStart.x = -maxBound.x / 2.0f;
			boundStart.y -= cellSize.y;
		}
		else
		{
			boundStart.x += cellSize.x;
			++countLineX;
		}
	}

#ifdef BINARY_WRITER

	std::string outName{ "./Resources/Digger/Level_0_Test.bin" };
	std::string inName{ "./Resources/Digger/Test.csv" };

	std::ifstream insStream{ inName };
	std::ofstream offStream{ outName,std::ios::out | std::ios::binary };

	if (insStream.is_open())
	{
		std::string line{};
		while (std::getline(insStream, line, '\n'))
		{
			std::stringstream resolver{};
			resolver << line;
			while (std::getline(resolver, line, ','))
			{
				if (offStream.is_open())
				{
					const int semantic{ std::stoi(line) };
					offStream.write(reinterpret_cast<const char*>(&semantic), 1);
				}
			}
		}
	}

	insStream.close();
	offStream.close();

#endif


	//auto player = CreateGameObject<PlayerDigger>();
	//player->GetTransform()->SetPosition(0.0f, -150.0f, 0.0f);
	//player->GetTransform()->SetScale(relativeScaling.x, relativeScaling.y);

	auto scoreManager = CreateGameObject<ScoreManager>();
	//auto nobblin = CreateGameObject<Nobblin>();

	//glm::fvec3 position{-200.0f,100.0f,0.0f};
	//for (int i = 0; i < 150; ++i)
	//{
	//	if (i % 10 == 0 && i != 0)
	//	{
	//		position.y -= 60.0f;
	//		position.x = -200.0f;
	//	}
	//	auto collisionTest = CreateGameObject(position);
	//	auto collider = collisionTest->CreateComponent<BoxCollider2D>();
	//	collider->SetSize({ 50.0f,50.0f });

	//	position.x += 60.0f;
	//}

	//auto collisionTest = CreateGameObject({ -150.0f,0.0f,0.0f });
	//auto collider = collisionTest->CreateComponent<BoxCollider2D>();
	//collider->SetSize({ 50.0f,50.0f });

	//collisionTest = CreateGameObject({ 150.0f,0.0f,0.0f });
	//collider = collisionTest->CreateComponent<BoxCollider2D>();
	//collider->SetSize({ 50.0f,50.0f });
}

void Digger::SetUpInputMappingGroup()
{
	using namespace dae;
	auto& horizontalGroup = GetSceneContext().pInputManager->AddInputAxisGroup("Horizontal");
	horizontalGroup.AddKey(Key(Device::D_Keyboard, SDLK_LEFT), -1.0f);
	horizontalGroup.AddKey(Key(Device::D_Keyboard, SDLK_RIGHT), 1.0f);
	auto& verticalGroup = GetSceneContext().pInputManager->AddInputAxisGroup("Vertical");
	verticalGroup.AddKey(Key(Device::D_Keyboard, SDLK_UP), 1.0f);
	verticalGroup.AddKey(Key(Device::D_Keyboard, SDLK_DOWN), -1.0f);
	auto& shoot = GetSceneContext().pInputManager->AddInputActionGroup("Shoot");
	shoot.AddKey(Key(Device::D_Keyboard, SDLK_SPACE));
}

void Digger::Update()
{

}

void Digger::SpawnPath(const glm::fvec3& position, const glm::fvec2& scale)
{
	auto object = CreateGameObject(position);
	object->GetTransform()->SetRelativePosition({ 0.0f,0.0f,-2.0f });
	object->GetTransform()->SetScale(scale.x, scale.y);
	auto spriteRenderer = object->CreateComponent<dae::SpriteRenderer>();
	spriteRenderer->SetSprite(m_pWalkSprites.at(PathDirection::None), true);
}

void Digger::InstantiatePath(const glm::fvec3& position, const glm::fvec2& scale)
{
	auto object = Instantiate(position, {}, scale);
	//object->GetTransform()->SetRelativePosition({ 0.0f,0.0f,-2.0f });
	auto spriteRenderer = object->CreateComponent<dae::SpriteRenderer>();
	spriteRenderer->SetSprite(m_pWalkSprites.at(PathDirection::None), true);
}

void Digger::AddPathEdges()
{

}
