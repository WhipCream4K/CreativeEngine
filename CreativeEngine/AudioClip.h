#pragma once
#include "EngineAsset.h"
#include "BaseAsset.h"

namespace dae
{
	class AudioClip : public BaseAsset
	{
	public:

		AudioClip(const std::string& assetPath, const std::string& assetName);		
	};
}

