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
#include <fstream>
#include <iostream>

//#define BINARY_WRITER

const uint32_t Digger::m_CellCount{ 150 };

Digger::Digger()
	: Scene("Digger")
	, m_pBigSprites()
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

void Digger::SceneInitialize()
{
	using namespace dae;

	// Background -> Layer -2
	// PickUps -> Layer -1
	// Dynamic -> Layer 0

	// Initialize Dark sky
	{
		auto blackPixel = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Black.png", "Black");
		m_pBackgroundSprite = Sprite::Create(blackPixel);
		auto backGround = CreateGameObject({0.0f,0.0f,-3.0f}, {}, { 1280.0f,720.0f });
		auto backgroundRenderer = backGround->CreateComponent<SpriteRenderer>();
		backgroundRenderer->SetSprite(m_pBackgroundSprite,true);
	}

	// Initialize background
	{
		auto level0background = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Level_0_BG.tga", "Level_0_BG");
		auto backgroundSprite = Sprite::Create(level0background);
		m_pBigSprites.try_emplace(0u, backgroundSprite);
		auto background = CreateGameObject({0.0f,0.0f,-2.0f});
		background->GetTransform()->SetScale(1.8f, 1.8f);
		auto renderer = background->CreateComponent<SpriteRenderer>();
		renderer->SetSprite(backgroundSprite,true);
	}

	// Test Text
	{
		auto fontData = ResourceManager::Load<DefaultFontData>("./Resources/Digger/Font/Diggerfont-Regular.ttf", "DiggerFont");
		m_pDiggerFont = Text::Create(fontData, "HELLO_WORLD_", 100);
		auto gameObject = CreateGameObject({ -100.0f,300.0f,-1.0f });
		auto textRenderer = gameObject->CreateComponent<TextRenderer>();
		textRenderer->SetText(m_pDiggerFont);
		textRenderer->SetForegroundColor({ 0.0f,1.0f,0.0f,1.0f });
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


	const glm::fvec2 maxBound{ 480.0f,300.0f };
	const glm::fvec2 cellSize{ 36.0f,36.0f };
	glm::fvec3 boundStart{ -480.0f / 2.0f, 300.0f / 2.0f,-1.0f };
	const glm::fvec3 cellOffSet{ 16.0f,-15.0f,0.0f };

	std::shared_ptr<GameObject> pSceneObject;
	std::shared_ptr<SpriteRenderer> pSpriteRenderer;
	int countLineX{};

	// read in level file
	if (inStream.is_open())
	{
		m_pCellSemantics = new char[m_CellCount];
		inStream.seekg(0);
		inStream.read(m_pCellSemantics, m_CellCount);
	}

	inStream.close();

	for (uint32_t i = 0; i < m_CellCount; ++i)
	{
		BlockId semantics{ BlockId(m_pCellSemantics[i]) };

		switch (semantics)
		{
		case BlockId::None:

			pSceneObject = CreateGameObject(boundStart + cellOffSet);
			pSceneObject->GetTransform()->SetScale(1.8f, 1.8f);
			pSpriteRenderer = pSceneObject->CreateComponent<SpriteRenderer>();
			pSpriteRenderer->SetSprite(m_pWalkSprites.at(PathDirection::None), true);

			break;
		case BlockId::Level:															break;
		case BlockId::Gold:		CreateGameObject<Gold>(boundStart + cellOffSet);		break;
		case BlockId::Jewel:	CreateGameObject<Jewel>(boundStart + cellOffSet);	break;
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

	std::string outName{ "./Resources/Digger/Test.bin" };
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

	auto player = CreateGameObject<PlayerDigger>();
	//player->GetTransform()->SetScale(1.5f, 1.5f);
	player->GetTransform()->SetPosition(0.0f, -150.0f, 0.0f);
	player->GetTransform()->SetScale(1.8f, 1.8f);

	
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
}

void Digger::Update()
{
}
