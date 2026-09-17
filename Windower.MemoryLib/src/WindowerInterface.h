#pragma once

extern "C"
{
#define LUA_BUILD_AS_DLL

#include "lauxlib.h"
#include "lua.h"
}

namespace memorylib
{
	/* a simple, mostly static interface addon-on to
	 * provide interopperability with the LUA-C API 
	 */
	class WindowerInterface
	{
		private: 

		public:
			~WindowerInterface(void) {};

			static int registerInterface(lua_State *L);

			static int lua_findPattern(lua_State* L);
			static int lua_write_uint8(lua_State* L);
			static int lua_readBytes(lua_State* L);
			static int lua_writeBytes(lua_State* L);


		protected:
			WindowerInterface(void) {};
	};
}

