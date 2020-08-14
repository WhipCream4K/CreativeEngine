

#include "pch.h"
#include "Sandbox.h"

#ifdef main
#undef main
#endif

int main()
{
	const auto mainGame{ std::make_shared<Sandbox>() };
	mainGame->Run();
	return 0;
}
