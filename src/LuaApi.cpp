#include <Windows.h>
#include "include/Utils.h"
#include "include\LuaApi.h"
#include <sstream>

namespace VermHook
{
	int LuaApi::Log::Write(LuaState* state)
	{
		Logger::Write(HandleConsoleOut(state, "Lua"), false, false);
		return 0;
	}
	int LuaApi::Log::Warn(LuaState* state)
	{
		Logger::Warn(HandleConsoleOut(state, "Lua WARNING"), false, false);
		return 0;
	}
	int LuaApi::Log::Debug(LuaState* state)
	{
		Logger::Debug(HandleConsoleOut(state, "Lua debug"), false, false);
		return 0;
	}

	string LuaApi::Log::HandleConsoleOut(LuaState* state, string prefix)
	{
		// Handle vaargs
		int args = lua_gettop(state);
		if (args <= 0)
			return "null";

		std::stringstream str;

		str << ">> " << prefix << ":";

		for (int i = 0; i < args; i++)
		{
			// for handling a sigsegv on lua_tolstring with index 1 being nil
			int type = lua_type(state, 1);

			str << " " <<
				((type != LUA_TNIL)
					? lua_tolstring(state, 1, NULL)
					: "nil");

			lua_remove(state, 1);
		}

		return str.str();
	}

	int LuaApi::Log::Create(LuaState* state)
	{
		bool result = AllocConsole() == TRUE;
		lua_pushboolean(state, result);

		if (result)
		{
#pragma warning(disable:4996)
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);
			freopen("CONIN$", "r", stdin);
			SetConsoleTitle("Vermintide LUA");
		}

		return 1;
	}


#define PARAM_CHECK(errmsg) \
int argc = lua_gettop(state); \
	if (argc == 0 || argc > 2) \
		return luaL_error(state, errmsg); \
	auto path = lua_tolstring(state, 1, NULL); \
	bool dirOnly = false; \
	luaC_pop(state); \
	if (argc == 2) { \
		dirOnly =luaC_toboolean(state, 1); \
		luaC_pop(state); } 

	int LuaApi::Path::GetElements(LuaState* state)
	{
		PARAM_CHECK("Invalid GetElements params");

		if (!Utils::ElementExists(path, dirOnly))
		{
			lua_pushnil(state);
			return 1;
		}

		std::vector<string> out;
		Utils::GetElements((string(path) + "/*").c_str(), out, dirOnly);
		Utils::StrVectorToIndexedTable(state, out);
		return 1;
	}

	int LuaApi::Path::ElementExists(LuaState* state)
	{
		PARAM_CHECK("Invalid Elements exists params");
		lua_pushboolean(state, Utils::ElementExists(path, dirOnly));
		return 1;
	}
}