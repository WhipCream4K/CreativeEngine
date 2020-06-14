#pragma once
#include "CreativeStructs.h"

namespace dae
{
	class EngineContext
	{
	public:
		static const Window& GetWindowSettings() { return m_MainWindow; }
		static const glm::fvec4& GetApplicationBGColor() { return m_BGColor; }
		static void SetApplicationExit(bool value) { m_ApplicationExit = value; }
		static const bool& IsApplicationExit() { return m_ApplicationExit; }
		static void SetApplicationBGColor(const glm::fvec4& color) { m_BGColor = color; }
		static uint32_t GetMaxAudioChannels() { return m_MaxAudioChannels; }
		
	private:
		static bool m_ApplicationExit;
		static Window m_MainWindow;
		static glm::fvec4 m_BGColor;
		static uint32_t m_MaxAudioChannels;
	};

}


