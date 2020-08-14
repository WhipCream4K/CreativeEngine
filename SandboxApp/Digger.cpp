#include "pch.h"
#include "Digger.h"

void Digger::SceneInitialize()
{
	// Load in Level sprite sheet
	// 0 ==> Level 1 Middle
	// 1 ==> Level 1 Left
	// 2 ==> Level 1 Right
	// 3 ==> Level 1 Top
	// 4 ==> Level 1 Bottom
	// 5 ==> Level 2 Middle
	// 6 ==> Level 2 Left
	// 7 ==> Level 2 Right
	// 8 ==> Level 2 Top
	// 9 ==> Level 2 Bottom
	// 10 ==> Level 3 Middle
	// 11 ==> Level 3 Left
	// 12 ==> Level 3 Right
	// 13 ==> Level 3 Top
	// 14 ==> Level 3 Bottom
	// 15 ==> Gold
	// 16 ==> Jewel
	// 17 ==> Rip
	
	const BlockId semantics{};
	switch (semantics)
	{
	case BlockId::None: 


		break;
	case BlockId::Level: break;
	case BlockId::Jewel: break;
	case BlockId::Gold: break;
	default: ;
	}
}
