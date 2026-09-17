#include "WindowerInterface.h"
#include "MemFunctions.h"

namespace memorylib
{
	int WindowerInterface::registerInterface(lua_State* L)
	{
		struct luaL_reg api[] = {

    	 	{ "findPattern"        , WindowerInterface::lua_findPattern },
			{ "write_uint8"        , WindowerInterface::lua_write_uint8 },
			{ NULL, NULL }
		};

		luaL_register(L, "memorylib", api);
		return 1;
	}

	int WindowerInterface::lua_findPattern(lua_State* L)
	{
		if (lua_gettop(L) != 2 || !lua_isstring(L, 1) || !lua_isstring(L, 2)) {
			lua_pushstring(L, "invalid memory search parameters");
			lua_error(L);
		}

		const char* moduleName = lua_tostring(L, 1);
		std::string pattern_str = lua_tostring(L, 2);

		// Convert pattern from HEX string to bytes
		ByteArray pattern_bytes = MemFunctions::decodeHex(pattern_str);
		ByteMask mask = MemFunctions::decodeMask(pattern_str);

		DWORD result = MemFunctions::findPattern(moduleName, pattern_bytes, mask);

		lua_pushnumber(L, result);

		return 1;
	}

	int WindowerInterface::lua_write_uint8(lua_State* L)
	{
		if (lua_gettop(L) != 2 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2)) {
			lua_pushstring(L, "invalid memory write parameters");
			lua_error(L);
		}

		DWORD addr = lua_tonumber(L, 1);
		uint8_t value = lua_tonumber(L, 2);

		MemFunctions::write_uint8(addr, value);

		return 1;
	}
}

