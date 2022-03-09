#include <core.h>
#include <str.h>
#include <vector.h>
#include <platform.h>
#include "luascript.h"
#include "luaangle.h"

Ang *lua_newangle(lua_State *L) {
	Ang *ang = lua_newuserdata(L, sizeof(Ang));
	Memory_Zero(ang, sizeof(Ang));
	luaL_setmetatable(L, CSLUA_MANGLE);
	return ang;
}

Ang *lua_checkangle(lua_State *L, int idx) {
	return (Ang *)luaL_checkudata(L, idx, CSLUA_MANGLE);
}

static int ang_setvalue(lua_State *L) {
	Ang *ang = lua_checkangle(L, 1);
	ang->yaw = (cs_float)luaL_checknumber(L, 2);
	ang->pitch = (cs_float)luaL_checknumber(L, 3);
	return 0;
}

static int ang_getvalue(lua_State *L) {
	Ang *ang = lua_checkangle(L, 1);
	lua_pushnumber(L, (lua_Number)ang->yaw);
	lua_pushnumber(L, (lua_Number)ang->pitch);
	return 2;
}

static cs_bool getaxis(cs_str str, cs_char *ax) {
	if(String_CaselessCompare(str, "yaw")) {
		*ax = 'y';
		return true;
	} else if(String_CaselessCompare(str, "pitch")) {
		*ax = 'p';
		return true;
	}

	return false;
}

static int meta_index(lua_State *L) {
	Ang *ang = lua_checkangle(L, 1);

	cs_char axis = 0;
	if(getaxis(luaL_checkstring(L, 2), &axis)) {
		switch(axis) {
			case 'y': lua_pushnumber(L, (lua_Number)ang->yaw); break;
			case 'p': lua_pushnumber(L, (lua_Number)ang->pitch); break;
		}

		return 1;
	}

	luaL_argerror(L, 2, CSLUA_MANGLE " axis expected");
	return 0;
}

static int meta_newindex(lua_State *L) {
	Ang *ang = lua_checkangle(L, 1);

	cs_char axis = 0;
	if(getaxis(luaL_checkstring(L, 2), &axis)) {
		switch(axis) {
			case 'y': ang->yaw = (cs_float)luaL_checknumber(L, 3); break;
			case 'p': ang->pitch = (cs_float)luaL_checknumber(L, 3); break;
		}

		return 0;
	}

	luaL_argerror(L, 2, CSLUA_MANGLE " axis expected");
	return 0;
}

static int meta_tostring(lua_State *L) {
	Ang *ang = lua_checkangle(L, 1);
	lua_pushfstring(L, "Angle(%.2f, %.2f)", ang->yaw, ang->pitch);
	return 1;
}

static const luaL_Reg anglemeta[] = {
	{"set", ang_setvalue},
	{"get", ang_getvalue},

	{"__index", meta_index},
	{"__newindex", meta_newindex},
	{"__tostring", meta_tostring},

	{NULL, NULL}
};

static int ang_new(lua_State *L) {
	lua_newangle(L);

	if(lua_gettop(L) > 1) {
		luaL_getmetafield(L, -1, "set");
		lua_pushvalue(L, -2);
		lua_pushvalue(L, 1);
		lua_pushvalue(L, 2);
		lua_call(L, 3, 0);
	}

	return 1;
}

int luaopen_angle(lua_State *L) {
	luaL_newmetatable(L, CSLUA_MANGLE);
	luaL_setfuncs(L, anglemeta, 0);
	lua_pop(L, 1);

	lua_pushcfunction(L, ang_new);
	lua_setglobal(L, luaL_checkstring(L, 1));
	return 0;
}
