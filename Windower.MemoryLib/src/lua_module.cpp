#include "WindowerInterface.h"

extern "C" __declspec(dllexport) int luaopen_memorylib(lua_State *L)
{
	return memorylib::WindowerInterface::registerInterface(L);
}

