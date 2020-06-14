#include "pch.h"
#include "MainGame.h"

#include <vld.h>

#ifdef main
	#undef main
#endif

int main()
{
	const auto mainGame{ std::make_shared<dae::MainGame>() };
	mainGame->Run();
	return 0;
}