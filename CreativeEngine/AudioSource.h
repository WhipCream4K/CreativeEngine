#pragma once
#include "BaseComponent.h"

#include <fmod.hpp>

namespace dae
{
	class AudioClip;
	class AudioSource : public BaseComponent
	{
	public:

		AudioSource();
		
		void Play();
		bool IsPlaying();

		float GetVolume() const { return m_Volume; }
		bool IsLooping() const { return m_IsLoop; }
		float GetPitch() const { return m_Pitch; }
		
		void SetAudioClip(std::weak_ptr<AudioClip> clip) { m_AudioClip = std::move(clip); }
		void SetLoop(bool value) { m_IsLoop = value; }
		void SetVolume(float value) { m_Volume = value; }
		void SetPitch(float value) { m_Pitch = value; }
		std::shared_ptr<AudioClip> GetAudioClip() const { return m_AudioClip.lock(); }

	private:

		std::weak_ptr<AudioClip> m_AudioClip;
		float m_Volume;
		float m_Pitch;
		int m_ChannelId;
		bool m_IsLoop;
	};
}

