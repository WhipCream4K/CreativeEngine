#include "pch.h"
#include "AudioClip.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "FMODAudioService.h"

dae::AudioClip::AudioClip(const std::string& assetPath, const std::string& assetName)
	: BaseAsset(assetPath,assetName)
{
	
}

std::shared_ptr<dae::AudioClip> dae::AudioClip::Create(const std::string& assetPath, const std::string& assetName)
{
	// TODO: For now there's nothing to do
	return ResourceManager::Load<AudioClip>(assetPath, assetName);
}
