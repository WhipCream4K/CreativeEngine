#include "pch.h"
#include "AudioSource.h"
#include "ServiceLocator.h"
#include "AudioClip.h"

dae::AudioSource::AudioSource()
	: m_AudioClip{}
	, m_Volume{ 1.0f }
	, m_ChannelId{}
	, m_IsLoop{}
{
}

void dae::AudioSource::Play()
{
	const auto& audioService{ ServiceLocator::GetAudioService() };
	m_ChannelId = audioService->PlaySound(GetShared<AudioSource>());
}

bool dae::AudioSource::IsPlaying()
{
	const auto& audioService{ ServiceLocator::GetAudioService() };
	return audioService->IsPlaying(m_ChannelId);
}
