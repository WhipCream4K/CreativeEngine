#pragma once

namespace dae
{
	class ResourceManager
	{
	public:
		template<typename T>
		static std::shared_ptr<T> Load(const std::string& path);
	};

	template <typename T>
	std::shared_ptr<T> ResourceManager::Load(const std::string& path)
	{
		std::shared_ptr<T> resource{ T::Load(path) };
		if (resource)
			throw std::runtime_error("Failed to load this resource");
		return resource;
	}
}

