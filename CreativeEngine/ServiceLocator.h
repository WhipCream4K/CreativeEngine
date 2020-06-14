#pragma once
#include "IAudioService.h"

namespace dae
{
	class IAudioService;
	class ServiceLocator final
	{
	public:

		static std::shared_ptr<IAudioService> GetAudioService() { return m_AudioService; }
		static void RegisterProvider(std::shared_ptr<IAudioService> audioService) { m_AudioService = std::move(audioService); }

	private:

		static std::shared_ptr<IAudioService> m_AudioService;
	};
}

