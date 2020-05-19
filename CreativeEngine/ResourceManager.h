#pragma once
#include "Singleton.h"

#include <unordered_map>

namespace dae
{
	class BaseAsset;
	template<typename T>
	using Asset = typename std::enable_if<std::is_base_of_v<BaseAsset, T>,T>::type;
	
	class Texture2D;
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:

		template<typename T>
		constexpr static std::shared_ptr<Asset<T>> Load(const std::string& assetPath, const std::string& resourceName)
		{
			return GetInstance().LoadImpl<Asset<T>>(assetPath, resourceName);
		}

		static const char* GetResourceName(std::weak_ptr<BaseAsset> pAsset)
		{
			return GetInstance().GetResourceNameImpl(std::move(pAsset));
		}

	private:

		// Implement Functions
		template<typename T>
		std::shared_ptr<T> LoadImpl(const std::string& assetPath, const std::string& resourceName);
		const char* GetResourceNameImpl(std::weak_ptr<BaseAsset> pAsset);

		// Data members
		std::unordered_map<std::string, std::shared_ptr<BaseAsset>> m_pAssets;
	};

	template <typename T>
	std::shared_ptr<T> ResourceManager::LoadImpl(const std::string& assetPath, const std::string& resourceName)
	{
		// find if this resource is already exist
		const auto& resource{ m_pAssets.find(resourceName) };

		if (resource != m_pAssets.end())
			return std::static_pointer_cast<T, BaseAsset>(resource->second);

		std::shared_ptr<T> newLoadedResources{ std::make_shared<T>(assetPath) };
		m_pAssets.try_emplace(resourceName, newLoadedResources);
		return newLoadedResources;
	}
}

