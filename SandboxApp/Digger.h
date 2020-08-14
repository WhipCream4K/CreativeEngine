#pragma once
#include <Scene.h>
#include <unordered_map>


class Digger : public dae::Scene
{
public:
	
	enum class BlockId
	{
		None,
		Level,
		Jewel,
		Gold,
		End
	};

	
	
protected:

	void SceneInitialize() override;
	
private:
	
	std::unordered_map<uint32_t, std::shared_ptr<dae::Sprite>> m_pBigSprites;
};

