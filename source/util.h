#pragma once

#include "GarrysMod/Lua/Interface.h"
#include "GameEventListener.h"
#include <string>

using namespace GarrysMod::Lua;

extern GarrysMod::Lua::ILuaBase* GlobalLUA;
extern GarrysMod::Lua::ILuaBase* ClientLUA;

extern void LuaPrint(const char*);
extern void LuaPrint(std::string);
extern void LuaPrint(const char*, GarrysMod::Lua::ILuaBase*);
extern void LuaPrint(std::string, GarrysMod::Lua::ILuaBase*);

extern void Start_Table(GarrysMod::Lua::ILuaBase*);

extern void Add_Func(GarrysMod::Lua::ILuaBase*, CFunc, const char*);
extern void Add_Func(GarrysMod::Lua::ILuaBase*, CFunc, std::string);

extern void Finish_Table(GarrysMod::Lua::ILuaBase*, const char*);
extern void Finish_Table(GarrysMod::Lua::ILuaBase*, std::string);

extern void ThrowError(const char*);