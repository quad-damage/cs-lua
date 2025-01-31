#ifndef CSLUAVECTOR_H
#define CSLUAVECTOR_H
#include <core.h>
#include <vector.h>
#include "luascript.h"

typedef enum _ELuaVectorType {
	LUAVECTOR_TFLOAT = 0,
	LUAVECTOR_TSHORT
} ELuaVectorType;

typedef struct _LuaVector {
	ELuaVectorType type;
	union {
		Vec f;
		SVec s;
	} value;
} LuaVector;

cs_bool lua_isvector(lua_State *L, int idx);
LuaVector *lua_newvector(lua_State *L);
LuaVector *lua_checkvector(lua_State *L, int idx);
LuaVector *lua_tovector(lua_State *L, int idx);
Vec *lua_checkfloatvector(lua_State *L, int idx);
Vec *lua_tofloatvector(lua_State *L, int idx);
SVec *lua_checkshortvector(lua_State *L, int idx);
SVec *lua_toshortvector(lua_State *L, int idx);
int luaopen_vector(lua_State *L);
#endif
