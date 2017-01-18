#include "include/Hook.h"
#include "include/Globals.h"
#include "include/IATHook.h"
#include "include/Routine/ModLoaderRoutine.h"

namespace VermHook
{
	void(*lua_pushcclosure) (LuaState*, LuaCFunction, int);
	void(*lua_setfield) (LuaState*, int, const char*);
	void(*luaL_openlibs) (LuaState*);
	int(*luaL_loadfile) (LuaState*, const char*);
	int(*lua_type)(LuaState*, int);
	const char* (*lua_tolstring) (LuaState*, int, size_t*);
	void(*lua_call)(LuaState*, int/*nargs*/, int /*nresults*/);
	void(*lua_getfield)(LuaState*, int /*index*/, const char */*k*/);
	void(*lua_pushstring)(LuaState*, const char */*s*/);
	int(*lua_pcall)(LuaState*, int /*nargs*/, int /*nresults*/, int /*errfunc*/);
	void(*luaL_openlib) (LuaState*, const char* /*libname*/, const LuaReg*, int /*nup*/);
	void(*luaL_register) (LuaState*, const char* /*libname*/, const LuaReg*);
	void(*lua_remove) (LuaState*, int /*index*/);
	int(*luaL_loadbuffer)(LuaState*, const char* /*buff*/, size_t, const char* /*name*/);
	int(*luaL_loadstring)(LuaState*, const char*);
	void (*lua_pushboolean)(LuaState*, bool);
    int (*lua_gettop)(LuaState*);
    void (*lua_pop)(LuaState*, int);
    void (*lua_settable)(LuaState*, int /*index*/);

	unique_ptr<ModLoaderRoutine> routine = nullptr;

	void InitHook()
	{
		LOG("Initialize hook.");
		routine = std::make_unique<ModLoaderRoutine>();

		LOG("mapcalling lua functions");
		auto luaModule = GetModuleHandle(LuaModule.c_str());

#define mapcall(name) *(void**)(&name) = GetProcAddress(luaModule, #name);\
	if(name == nullptr) LOG(#name << " mapcall IS nullptr!")

		mapcall(luaL_openlibs);
		mapcall(lua_pushcclosure);
		mapcall(lua_setfield);
		mapcall(lua_call);
		mapcall(lua_type);
		mapcall(lua_tolstring);
		mapcall(lua_getfield);
		mapcall(lua_pushstring);
		mapcall(luaL_loadfile);
		mapcall(lua_pcall);
		mapcall(luaL_openlib);
		mapcall(luaL_register);
		mapcall(lua_remove);
		mapcall(luaL_loadbuffer);
		mapcall(luaL_loadstring);
		mapcall(luaL_register);
		mapcall(lua_pushboolean);
        mapcall(lua_gettop);
        mapcall(lua_pop);
        mapcall(lua_settable);
#undef mapcall

		routine->PostInit();
	}

	void DestroyHook()
	{
		routine.reset();
	}
}
