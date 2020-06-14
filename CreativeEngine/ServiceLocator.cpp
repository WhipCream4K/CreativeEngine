#include "pch.h"
#include "ServiceLocator.h"

std::shared_ptr<dae::IAudioService> dae::ServiceLocator::m_AudioService{};