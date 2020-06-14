#include "pch.h"
#include "EngineContext.h"

dae::Window dae::EngineContext::m_MainWindow{};
bool dae::EngineContext::m_ApplicationExit{};
glm::fvec4 dae::EngineContext::m_BGColor{0.47f,0.53f,0.6f,1.0f};
uint32_t dae::EngineContext::m_MaxAudioChannels{ 512 };

