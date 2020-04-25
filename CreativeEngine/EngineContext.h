
#pragma once
#include <glm.hpp>
#include <string>

#include "CreativeStructs.h"

namespace dae
{
	class CreativeCore;
	class EngineContext
	{
	public:
		static const Window& GetWindowSettings() { return m_MainWindow; }
		static void SetApplicationExit(bool value) { m_ApplicationExit = value; }
		static const bool& IsApplicationExit() { return m_ApplicationExit; }
	private:
		static bool m_ApplicationExit;
		static Window m_MainWindow;
	};

}


