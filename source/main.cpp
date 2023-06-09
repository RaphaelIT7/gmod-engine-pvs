#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/FactoryLoader.hpp>
#include "eiface.h"
#include "util.h"

SourceSDK::FactoryLoader engine_loader("engine");

static IVEngineServer* engine;
int size = 0;
//byte* current_pvs;

// engine.GetClusterCount() returns the cluster count
LUA_FUNCTION_STATIC(GetClusterCount)
{
	LUA->PushNumber(engine->GetClusterCount());

	return 1;
}

// engine.GetClusterForOrigin(Vector (The Vector to gte the cluster id from))
LUA_FUNCTION_STATIC(GetClusterForOrigin)
{
	Vector* vec = LUA->GetUserType<Vector>(1, Type::Vector);
	LUA->PushNumber(engine->GetClusterForOrigin(*vec));

	return 1;
}

// engine.CheckAreasConnected(Int (First Area ID), Int (Second Area ID))
LUA_FUNCTION_STATIC(CheckAreasConnected)
{
	LUA->PushBool(engine->CheckAreasConnected(LUA->CheckNumber(1), LUA->CheckNumber(2)));

	return 1;
}

// engine.GetArea(Vector (to get the Area id from))
LUA_FUNCTION_STATIC(GetArea)
{
	Vector* vec = LUA->GetUserType<Vector>(1, Type::Vector);
	LUA->PushNumber(engine->GetArea(*vec));

	return 1;
}

/*
// engine.GetPVSForCluster(Vector (The position to get the PVS from))
LUA_FUNCTION_STATIC(GetPVSForCluster)
{
	Vector* vec1 = LUA->GetUserType<Vector>(1, Type::Vector);
	delete[] current_pvs;
	current_pvs = new byte[size];
	engine->GetPVSForCluster(engine->GetClusterForOrigin(*vec1), size, current_pvs);

	return 0;
}*/

// engine.CheckOriginInPVS(Vector (to get the PVS from), Vector (to check if it is in the PVS))
LUA_FUNCTION_STATIC(CheckOriginInPVS)
{
	Vector* vec1 = LUA->GetUserType<Vector>(1, Type::Vector);
	byte* pvs = new byte[size];
	engine->GetPVSForCluster(engine->GetClusterForOrigin(*vec1), size, pvs);

	Vector* vec2 = LUA->GetUserType<Vector>(2, Type::Vector);
	LUA->PushBool(engine->CheckOriginInPVS(*vec2, pvs, size));

	delete[] pvs; // We don't want a memory leak :D

	return 1;
}

// engine.CheckBoxInPVS(Vector pvs(The Position to get the PVS from), Vector mins, Vector maxs)
LUA_FUNCTION_STATIC(CheckBoxInPVS)
{
	Vector* vec1 = LUA->GetUserType<Vector>(1, Type::Vector);
	byte* pvs = new byte[size];
	engine->GetPVSForCluster(engine->GetClusterForOrigin(*vec1), size, pvs);

	Vector* vec2 = LUA->GetUserType<Vector>(2, Type::Vector);
	Vector* vec3 = LUA->GetUserType<Vector>(3, Type::Vector);
	LUA->PushBool(engine->CheckBoxInPVS(*vec2, *vec3, pvs, size));

	delete[] pvs; // We don't want a memory leak :D

	return 1;
}

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	engine = (IVEngineServer*)engine_loader.GetFactory()(INTERFACEVERSION_VENGINESERVER, nullptr);
	if (engine == nullptr)
		ThrowError("Failed to initialize IVEngineServer");

	size = ceil(engine->GetClusterCount() / 8.0f);

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "engine");
			Add_Func(LUA, GetClusterCount, "GetClusterCount");
			Add_Func(LUA, GetClusterForOrigin, "GetClusterForOrigin");
			Add_Func(LUA, CheckAreasConnected, "CheckAreasConnected");
			Add_Func(LUA, GetArea, "GetArea");
			Add_Func(LUA, CheckOriginInPVS, "CheckOriginInPVS");
			Add_Func(LUA, CheckBoxInPVS, "CheckBoxInPVS");
	LUA->Pop(2);


	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}