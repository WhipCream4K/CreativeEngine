#include "pch.h"
#include "AudioClip.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "FMODAudioService.h"

dae::AudioClip::AudioClip(const std::string& assetPath, const std::string& assetName)
	: BaseAsset(assetPath,assetName)
{
	
}
