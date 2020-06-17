#pragma once
#include "Singleton.h"

#include <unordered_map>

namespace dae
{
	class EngineAsset;
	class BaseAsset;
	template<typename T>
	using Asset = typename std::enable_if<std::is_base_of_v<BaseAsset,T>, T>::type;
	template<typename T>
	using InAsset = std::enable_if_t<std::is_base_of_v<EngineAsset, T>, T>;

	class Texture2D;
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:

		template<typename T = BaseAsset>
		constexpr static std::shared_ptr<Asset<T>> Load(const std::string& assetPath, const std::string& assetName)
		{
			return GetInstance()->LoadImpl<Asset<T>>(assetPath, assetName);
		}

		//template<typename T>
		//constexpr static std::shared_ptr<Asset<T>> Load(std::shared_ptr<Asset<T>> asset)
		//{
		//	return GetInstance()->LoadImpl<Asset<T>>(asset);
		//}

		static const char* GetResourceName(std::weak_ptr<BaseAsset> pAsset)
		{
			return GetInstance()->GetResourceNameImpl(std::move(pAsset));
		}

		template<typename T = BaseAsset>
		constexpr static std::shared_ptr<Asset<T>> GetAsset(const std::string& assetName)
		{
			return GetInstance()->GetAssetImpl<Asset<T>>(assetName);
		}

		//template<typename T = EngineAsset>
		//constexpr static std::shared_ptr<InAsset<T>> Store(std::shared_ptr<InAsset<T>> engineAsset)
		//{
		//	return GetInstance()->StoreImpl(engineAsset);
		//}

		template<typename T = EngineAsset>
		constexpr static std::shared_ptr<InAsset<T>> Store(const std::string& name)
		{
			return GetInstance()->StoreImpl(name);
		}

	private:

		// Implement Functions
		template<typename T>
		std::shared_ptr<T> LoadImpl(const std::string& assetPath, const std::string& assetName);
		template<typename T>
		std::shared_ptr<T> LoadImpl(std::shared_ptr<T> asset);
		const char* GetResourceNameImpl(std::weak_ptr<BaseAsset> pAsset);
		template<typename T>
		std::shared_ptr<T> GetAssetImpl(const std::string& assetName);
		//template<typename T>
		//std::shared_ptr<T> StoreImpl(std::shared_ptr<T> engineAsset);
		template<typename T>
		std::shared_ptr<T> StoreImpl(const std::string& name);

		// Data members
		std::unordered_map<std::string, std::shared_ptr<BaseAsset>> m_pAssets;
		std::unordered_map<std::string, std::shared_ptr<EngineAsset>> m_pEngineAssets;
	};

	template <typename T>
	std::shared_ptr<T> ResourceManager::LoadImpl(const std::string& assetPath, const std::string& assetName)
	{
		// find if this resource is already exist
		const auto& resource{ m_pAssets.find(assetName) };

		if (resource != m_pAssets.end())
			return std::static_pointer_cast<T, BaseAsset>(resource->second);

		std::shared_ptr<T> newLoadedResources{ std::make_shared<T>(assetPath,assetName) };
		m_pAssets.try_emplace(assetName, newLoadedResources);
		return newLoadedResources;
	}

	template <typename T>
	std::shared_ptr<T> ResourceManager::LoadImpl(std::shared_ptr<T> asset)
	{
		if (asset)
		{
			const auto& assetName{ asset->GetName() };
			const auto& resource{ m_pAssets.find(assetName) };
			if (resource == m_pAssets.end())
				m_pAssets.try_emplace(assetName, asset);
		}
		return asset;
	}

	template <typename T>
	std::shared_ptr<T> ResourceManager::GetAssetImpl(const std::string& assetName)
	{
		const auto& resources{ m_pAssets.find(assetName) };
		return	resources != m_pAssets.end() ? std::static_pointer_cast<T>(resources->second) : nullptr;
	}


	//template <typename T>
	//std::shared_ptr<T> ResourceManager::StoreImpl(std::shared_ptr<T> engineAsset)
	//{
	//	const std::string& assetName = engineAsset->GetName();
	//	const auto& resource{ m_pEngineAssets.find(assetName) };

	//	if (resource == m_pEngineAssets.end())
	//		m_pEngineAssets.try_emplace(assetName, engineAsset);

	//	return resource->second;
	//}

	template <typename T>
	std::shared_ptr<T> ResourceManager::StoreImpl(const std::string& name)
	{
		const auto& resource = m_pEngineAssets.find(name);
		if (resource != m_pEngineAssets.end())
			return std::static_pointer_cast<T>(resource->second);

		std::shared_ptr<T> newEngineAsset{ std::make_shared<T>(name) };
		m_pEngineAssets.try_emplace(name, newEngineAsset);
		return newEngineAsset;
	}
}

