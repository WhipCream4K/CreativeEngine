#pragma once

//#include <string>

namespace dae
{
	class BaseAsset : public std::enable_shared_from_this<BaseAsset>
	{
	public:

		BaseAsset(const std::string& assetPath);
		
		BaseAsset(const BaseAsset&) = delete;
		BaseAsset& operator=(const BaseAsset&) = delete;

		const std::string& GetPath() const { return m_Path; }
		const char* GetName();

	private:
		std::string m_Path;
	};
}

