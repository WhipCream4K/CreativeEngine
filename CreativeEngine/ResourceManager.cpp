#include "pch.h"
#include "ResourceManager.h"
#include "BaseAsset.h"

const char* dae::ResourceManager::GetResourceNameImpl(std::weak_ptr<BaseAsset> pAsset)
{
	const auto it = m_pAssets.find(pAsset.lock()->GetName());

	if(it != m_pAssets.end())
	{
		for (const auto& asset : m_pAssets)
		{
			if (asset.second == pAsset.lock())
				return asset.first.c_str();
		}
	}
	
	return nullptr;
}
