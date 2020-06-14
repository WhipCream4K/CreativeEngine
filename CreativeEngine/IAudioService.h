#pragma once
#include <any>
#include <variant>

namespace dae
{
	class AudioSource;
	class IAudioService
	{
	public:
		virtual ~IAudioService() = default;
		virtual int PlaySound(const std::shared_ptr<AudioSource>& clip) = 0;
		virtual bool IsPlaying(int channelId) = 0;
	};
}

