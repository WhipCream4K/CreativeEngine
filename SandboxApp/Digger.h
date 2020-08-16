#pragma once
#include <Scene.h>
#include <unordered_map>


class Digger : public dae::Scene
{
public:

	Digger();
	
	enum class BlockId
	{
		None,
		Level,
		Gold,
		Jewel,
	};

	~Digger();
	
protected:

	void SceneInitialize() override;
	void SetUpInputMappingGroup() override;
	void Update() override;
	
private:
	
	std::unordered_map<uint32_t, std::shared_ptr<dae::Sprite>> m_pBigSprites;
	std::vector<std::shared_ptr<dae::Sprite>> m_pMoneyBagSprite;
	std::shared_ptr<dae::Sprite> m_pJewelSprite;
	std::shared_ptr<dae::Sprite> m_pCherrySprite;
	char* m_pCellSemantics;
};

