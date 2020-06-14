#pragma once

namespace dae
{
	class BaseAsset
	{
	public:

		BaseAsset(const std::string& assetPath,const std::string& assetName);
		
		BaseAsset(const BaseAsset&) = delete;
		BaseAsset& operator=(const BaseAsset&) = delete;
		BaseAsset(BaseAsset&&) = delete;
		BaseAsset& operator=(BaseAsset&&) = delete;
		
		const std::string& GetPath() const { return m_Path; }
		const std::string& GetName() const { return m_Name; }

	private:
		std::string m_Name;
		std::string m_Path;
	};
}

