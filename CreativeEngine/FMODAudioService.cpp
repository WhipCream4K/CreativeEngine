#include "pch.h"
#include "FMODAudioService.h"
#include "fmod_errors.h"
#include "AudioSource.h"
#include "AudioClip.h"

//std::shared_ptr<FMOD::System> dae::FMODAudioService::m_MainSystem{};

std::shared_ptr<dae::FMODAudioService> dae::FMODAudioService::Create(uint32_t maxChannels)
{
	FMOD::System* system{};
	auto result = FMOD::System_Create(&system);

	if (result != FMOD_OK)
		throw std::runtime_error(std::string("FMODAudioService::Create() Error Creating FMOD System: ") + FMOD_ErrorString(result));

	result = system->init(int(maxChannels), FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		throw std::runtime_error(std::string("FMODAudioService::Create() Error Initialing FMOD System: ") + FMOD_ErrorString(result));

	std::shared_ptr<FMOD::System> smartSystem{ std::shared_ptr<FMOD::System>{system,FMODDeleter()} };
	std::shared_ptr<FMODAudioService> service{ std::make_shared<FMODAudioService>(smartSystem) };

	return service;
}

int dae::FMODAudioService::PlaySound(const std::shared_ptr<AudioSource>& clip)
{
	int channelId{};
	FMOD::Sound* sound{};
	FMOD::Channel* channel{};
	bool isLooping{ clip->IsLooping() };
	isLooping;
	
	// TODO: Log the error
	auto result = m_MainSystem->createSound(clip->GetAudioClip()->GetPath().c_str(), FMOD_LOOP_OFF | FMOD_2D | FMOD_CREATESTREAM, nullptr, &sound);
	if (result != FMOD_OK)
		throw std::runtime_error(std::string("FMODAudioService::PlaySound() Error creating sound: ") + FMOD_ErrorString(result));

	channel->setVolume(clip->GetVolume());
	channel->setPitch(clip->GetPitch());
	m_MainSystem->playSound(sound, nullptr, false, &channel);
	channel->getIndex(&channelId);
	return channelId;
}

bool dae::FMODAudioService::IsPlaying(int channelId)
{
	FMOD::Channel* channel{};
	m_MainSystem->getChannel(channelId,&channel);
	bool result{};
	channel->isPlaying(&result);
	return result;
}

dae::FMODAudioService::FMODAudioService(std::shared_ptr<FMOD::System> fmodSystem)
	: m_MainSystem{ std::move(fmodSystem) }
{
}

//std::shared_ptr<FMOD::Sound> dae::FMODAudioService::CreateSound(const std::string& path)
//{
//	FMOD::Sound* sound{};
//	// TODO: for now the sound that got created will be 2D sound
//	auto result = m_MainSystem->createSound(path.c_str(), FMOD_CREATESTREAM | FMOD_2D, nullptr, &sound);
//	FMOD::Channel* test{};
//	std::shared_ptr<FMOD::Sound> smartSound = std::shared_ptr<FMOD::Sound>{ sound,FMODDeleter() };
//	return smartSound;
//}

