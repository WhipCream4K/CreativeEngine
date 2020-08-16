#include "pch.h"
#include "Digger.h"
#include "InputManager.h"
#include "PlayerDigger.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include <fstream>
#include <iostream>

//#define BINARY_WRITER

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

	std::ifstream inStream{ "./Resources/Digger/Level_0.bin", std::ios::in | std::ios::binary | std::ios::ate };


	glm::fvec3 boundStart{ -750.0f / 2.0f, 440.0f / 2.0f,-2.0f };
	const glm::fvec3 offset{ 25.0f,-22.0f,0.0f };
	uint32_t cellCount{};

	std::shared_ptr<GameObject> pSceneObject;
	std::shared_ptr<SpriteRenderer> pSpriteRenderer;
	int countLineX{};

	// read in level file
	if (inStream.is_open())
	{
		cellCount = uint32_t(inStream.tellg());
		m_pCellSemantics = new char[cellCount];
		inStream.seekg(0);
		inStream.read(m_pCellSemantics, cellCount);
	}

	inStream.close();

	for (uint32_t i = 0; i < cellCount; ++i)
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
}

void Digger::SetUpInputMappingGroup()
{
	using namespace dae;
	auto& horizontalGroup = GetSceneContext().pInputManager->AddInputAxisGroup("Horizontal");
	horizontalGroup.AddKey(Key(Device::D_Keyboard, SDLK_LEFT), -1.0f);
	horizontalGroup.AddKey(Key(Device::D_Keyboard, SDLK_RIGHT), 1.0f);
}

void Digger::Update()
{
}
