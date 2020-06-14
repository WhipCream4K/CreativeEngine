#pragma once

namespace dae
{
	// This class is for any assets that is used in the engine and shared around but has no parents to hold an object to.
	// Every thing will be owned by resource manager
	class EngineAsset : public std::enable_shared_from_this<EngineAsset>
	{
	public:
		EngineAsset(const std::string& assetName);
		virtual ~EngineAsset() = default;
		const std::string& GetName() const { return m_Name; }
		
	protected:

		template<typename T>
		std::shared_ptr<std::enable_if_t<std::is_base_of_v<EngineAsset,T>,T>> GetShared()
		{ return std::static_pointer_cast<T>(shared_from_this()); }
		
		std::string m_Name;
	};
}

