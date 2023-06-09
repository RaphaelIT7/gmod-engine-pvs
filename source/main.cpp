#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/FactoryLoader.hpp>
#include "eiface.h"
#include "util.h"

SourceSDK::FactoryLoader engine_loader("engine");

static IVEngineServer* server;

LUA_FUNCTION_STATIC(GetClusterCount)
{
	LUA->PushNumber(server->GetClusterCount());

	return 1;
}

LUA_FUNCTION_STATIC(GetClusterForOrigin)
{
	Vector* vec = LUA->GetUserType<Vector>(1, Type::Vector);
	LUA->PushNumber(server->GetClusterForOrigin(*vec));

	return 1;
}

LUA_FUNCTION_STATIC(CheckAreasConnected)
{
	LUA->PushBool(server->CheckAreasConnected(LUA->CheckNumber(1), LUA->CheckNumber(2)));

	return 1;
}

LUA_FUNCTION_STATIC(GetArea)
{
	Vector* vec = LUA->GetUserType<Vector>(1, Type::Vector);
	LUA->PushNumber(server->GetArea(*vec));

	return 1;
}

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	server = (IVEngineServer*)engine_loader.GetFactory()(INTERFACEVERSION_VENGINESERVER, nullptr);
	if (server == nullptr)
		ThrowError("Failed to initialize IVEngineServer");

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "engine");
			Add_Func(LUA, GetClusterCount, "GetClusterCount");
			Add_Func(LUA, GetClusterForOrigin, "GetClusterForOrigin");
			Add_Func(LUA, CheckAreasConnected, "CheckAreasConnected");
			Add_Func(LUA, GetArea, "GetArea");
	LUA->Pop(2);


	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}