#pragma once
#include "IAudioService.h"

namespace dae
{
	class FMODAudioService : public IAudioService
	{
	public:
		
		FMODAudioService(std::shared_ptr<FMOD::System> fmodSystem);
		
		static std::shared_ptr<FMODAudioService> Create(uint32_t maxChannels);
		int PlaySound(const std::shared_ptr<AudioSource>& clip) override;
		bool IsPlaying(int channelId) override;
		//std::shared_ptr<FMOD::Sound> CreateSound(const std::string& path);
		
	private:

		std::shared_ptr<FMOD::System> m_MainSystem;
	};
}

