#include "pch.h"
#include "BaseAsset.h"
#include "ResourceManager.h"

const char* dae::BaseAsset::GetName()
{
	return ResourceManager::GetResourceName(weak_from_this());
}

dae::BaseAsset::BaseAsset(const std::string& assetPath)
	: m_Path{ assetPath }
{
}
