#include "pch.h"
#include "Digger.h"
#include "InputManager.h"
#include "PlayerDigger.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "BoxCollider2D.h"
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

	auto moneyBag = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/MoneyBag.tga", "MoneyBag");
	m_pMoneyBagSprite = Sprite::CreateSpriteSheet(moneyBag, {}, { 32.0f,30.0f }, 4, 0.0f);
	auto jewel = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Jewel.tga", "Jewel");
	m_pJewelSprite = Sprite::Create(jewel);

	auto cherry = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Cherry.tga", "Cherry");
	m_pCherrySprite = Sprite::Create(cherry);

	std::ifstream inStream{ "./Resources/Digger/Level_0.bin", std::ios::in | std::ios::binary };


	glm::fvec3 boundStart{ -750.0f / 2.0f, 440.0f / 2.0f,-2.0f };
	const glm::fvec3 offset{ 25.0f,-22.0f,0.0f };

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

			pSceneObject = CreateGameObject(boundStart + offset);
			pSpriteRenderer = pSceneObject->CreateComponent<SpriteRenderer>();
			pSpriteRenderer->SetSprite(m_pCherrySprite, true);

			break;
		case BlockId::Level: break;
		case BlockId::Gold:

			pSceneObject = CreateGameObject(boundStart + offset);
			pSpriteRenderer = pSceneObject->CreateComponent<SpriteRenderer>();
			pSpriteRenderer->SetSprite(m_pMoneyBagSprite[0], true);

			break;
		case BlockId::Jewel:

			pSceneObject = CreateGameObject(boundStart + offset);
			pSpriteRenderer = pSceneObject->CreateComponent<SpriteRenderer>();
			pSpriteRenderer->SetSprite(m_pJewelSprite, true);

			break;
		}

		if (countLineX % 14 == 0 && countLineX != 0)
		{
			countLineX = 0;
			boundStart.x = -375.0f;
			boundStart.y -= 44.0f;
		}
		else
		{
			boundStart.x += 50.0f;
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
	player->GetTransform()->SetScale(1.5f, 1.5f);

	glm::fvec3 position{-200.0f,100.0f,0.0f};
	for (int i = 0; i < 150; ++i)
	{
		if (i % 10 == 0 && i != 0)
		{
			position.y -= 60.0f;
			position.x = -200.0f;
		}
		auto collisionTest = CreateGameObject(position);
		auto collider = collisionTest->CreateComponent<BoxCollider2D>();
		collider->SetSize({ 50.0f,50.0f });

		position.x += 60.0f;
	}
	
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
