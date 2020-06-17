#include "pch.h"
#include "ITextureEntity.h"

dae::ITextureEntity::ITextureEntity(const std::string& assetPath, const std::string& assetName)
	: BaseAsset(assetPath, assetName)
	, m_HasQueryDimension{}
{
}

const glm::fvec2& dae::ITextureEntity::GetActualDimension()
{
	if(!m_HasQueryDimension)
	{
		m_HasQueryDimension = true;
		m_ActualDimension = GetActualDimensionAfterLoad();
	}

	return m_ActualDimension;
}
