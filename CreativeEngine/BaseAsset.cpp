#include "pch.h"
#include "BaseAsset.h"

dae::BaseAsset::BaseAsset(const std::string& assetPath, const std::string& assetName)
	: m_Name{ assetName }
	, m_Path{ assetPath }
{
}
