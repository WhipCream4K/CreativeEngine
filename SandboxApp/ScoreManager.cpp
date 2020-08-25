#include "pch.h"
#include "ScoreManager.h"

#include "AudioClip.h"
#include "TextRenderer.h"
#include "Text.h"
#include "PickUps.h"
#include "AudioSource.h"

ScoreManager::ScoreManager()
	: m_TotalScore()
{
}

void ScoreManager::UpdateScore(int score)
{
	m_TotalScore += score;
	auto textRenderer = m_pTextRenderer.lock();
	if (textRenderer)
	{
		const std::string scoreText{ std::to_string(m_TotalScore) };
		const auto textsize{ scoreText.size() };
		if (scoreText.size() < m_InitScoreShown)
		{
			std::string replacement{};
			replacement.append(size_t(m_InitScoreShown) - textsize,'0');
			std::string test{ replacement + scoreText };
			textRenderer->SetText(replacement.append(scoreText));
			if (!m_pRefPickUpSound.lock()->IsPlaying())
				m_pRefPickUpSound.lock()->Play();
		}
	}
}

void ScoreManager::Awake()
{
	using namespace dae;
	auto textRenderer = CreateComponent<TextRenderer>();
	m_pTextRenderer = textRenderer;

	auto fontEntity = ResourceManager::GetAsset<DefaultFontData>("DiggerFont");
	m_pText = Text::Create(fontEntity, "00000", 70);
	textRenderer->SetText(m_pText);
	textRenderer->SetFontAsset(fontEntity);
	textRenderer->SetForegroundColor({ 0.0f,1.0f,0.0f,1.0f });

	GetTransform()->SetPosition({ -400.0f,310.0f,-1.0f });

	PickUps::OnScoreUpdate.RegisterObserver<ScoreManager>(GetShared<ScoreManager>(), &ScoreManager::UpdateScore);

	auto sound = ResourceManager::Load<AudioClip>("./Resources/Digger/PickUp.wav", "PickUpSound");
	auto soundMaker = CreateComponent<AudioSource>();
	soundMaker->SetAudioClip(sound);
	m_pRefPickUpSound = soundMaker;
}

void ScoreManager::Start()
{
}
