#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/FactoryLoader.hpp>
#include <GarrysMod/InterfacePointers.hpp>
#include "util2.h"
//#include <util.h> // Causes Errors?!? Time to use the source code.

SourceSDK::FactoryLoader engine_loader("engine");
int size = 0;
byte* current_pvs;

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

// engine.GetPVSForCluster(Number (The id of the cluster to get the PVS from) or Vector (The position to get the PVS from))
LUA_FUNCTION_STATIC(GetPVSForCluster)
{
	if (LUA->GetType(1) == Type::Vector) {
		Vector* vec1 = LUA->GetUserType<Vector>(1, Type::Vector);
		//delete[] current_pvs;
		//current_pvs = new byte[size];
		engine->ResetPVS(current_pvs, size);
		engine->GetPVSForCluster(engine->GetClusterForOrigin(*vec1), size, current_pvs);
	}
	else if (LUA->GetType(1) == Type::Number) {
		engine->ResetPVS(current_pvs, size);
		engine->GetPVSForCluster(LUA->GetNumber(1), size, current_pvs);
	}
	else {
		LUA->ThrowError("Expected a number or a Vector!");
	}
	
	return 1;
}

// engine.CheckOriginInPVS(Vector (to check if it is in the PVS), Vector (to get the PVS from))
LUA_FUNCTION_STATIC(CheckOriginInPVS)
{
	if (LUA->GetType(2) == Type::Vector) {
		Vector* vec1 = LUA->GetUserType<Vector>(2, Type::Vector);
		engine->ResetPVS(current_pvs, size);
		engine->GetPVSForCluster(engine->GetClusterForOrigin(*vec1), size, current_pvs);
	}

	Vector* vec2 = LUA->GetUserType<Vector>(1, Type::Vector);
	LUA->PushBool(engine->CheckOriginInPVS(*vec2, current_pvs, size));

	return 1;
}

// engine.CheckBoxInPVS(Vector mins, Vector maxs, Vector pvs(The Position to get the PVS from, if not passed it will use the PVS got by GetPVSForCluster))
LUA_FUNCTION_STATIC(CheckBoxInPVS)
{
	if (LUA->GetType(3) == Type::Vector) {
		Vector* vec1 = LUA->GetUserType<Vector>(3, Type::Vector);
		engine->ResetPVS(current_pvs, size);
		engine->GetPVSForCluster(engine->GetClusterForOrigin(*vec1), size, current_pvs);
	}

	Vector* vec2 = LUA->GetUserType<Vector>(1, Type::Vector);
	Vector* vec3 = LUA->GetUserType<Vector>(2, Type::Vector);
	LUA->PushBool(engine->CheckBoxInPVS(*vec2, *vec3, current_pvs, size));

	return 1;
}

class CBaseEntity;

/* Player:SetClientVisibilityPVS(Entity player to set the PVS, Vector pvs(The Position to get the PVS from, if not passed it will use the PVS got by GetPVSForCluster))
LUA_FUNCTION_STATIC(SetClientVisibilityPVS)
{
	if (LUA->GetType(2) == Type::Vector) {
		Vector* vec1 = LUA->GetUserType<Vector>(2, Type::Vector);
		engine->ResetPVS(current_pvs, size);
		engine->GetPVSForCluster(engine->GetClusterForOrigin(*vec1), size, current_pvs);
	}

	edict_t* ply = LUA->GetUserType<edict_t>(1, Type::Entity);
	Msg(std::to_string(engine->IndexOfEdict(ply)).c_str());
	Msg("\n");
	//UTIL_SetClientVisibilityPVS(ply, current_pvs, size);

	return 0;
}*/

LUA_FUNCTION_STATIC(SetStateFlag)
{
	int index = LUA->CheckNumber(1);
	int flag = LUA->CheckNumber(2);
	edict_t* ent = engine->PEntityOfEntIndex(index);
	int nFlags = ent->m_fStateFlags;
	if (nFlags & FL_EDICT_DONTSEND) {
		ent->m_fStateFlags &= ~FL_EDICT_DONTSEND;
	} else if (nFlags & FL_EDICT_ALWAYS) {
		ent->m_fStateFlags &= ~FL_EDICT_ALWAYS;
	} else if (nFlags & FL_EDICT_PVSCHECK) {
		ent->m_fStateFlags &= ~FL_EDICT_PVSCHECK;
	} else if (nFlags & FL_EDICT_FULLCHECK) {
		ent->m_fStateFlags &= ~FL_EDICT_FULLCHECK;
	}

	if (flag == 1) {
		ent->m_fStateFlags |= FL_EDICT_DONTSEND;
	} else if (flag == 2) {
		ent->m_fStateFlags |= FL_EDICT_ALWAYS;
	} else if (flag == 3) {
		ent->m_fStateFlags |= FL_EDICT_PVSCHECK;
	} else if (flag == 4) {
		ent->m_fStateFlags |= FL_EDICT_FULLCHECK;
	}

	return 0;
}

LUA_FUNCTION_STATIC(GetStateFlag)
{
	int index = LUA->CheckNumber(1);
	edict_t* ent = engine->PEntityOfEntIndex(index);
	int nFlags = ent->m_fStateFlags;
	if (nFlags & FL_EDICT_DONTSEND) {
		LUA->PushNumber(1);
		return 1;
	} else if (nFlags & FL_EDICT_ALWAYS) {
		LUA->PushNumber(2);
		return 1;
	} else if (nFlags & FL_EDICT_PVSCHECK) {
		LUA->PushNumber(3);
		return 1;
	} else if (nFlags & FL_EDICT_FULLCHECK) {
		LUA->PushNumber(4);
		return 1;
	}

	return 0;
}

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	engine = (IVEngineServer*)engine_loader.GetFactory()(INTERFACEVERSION_VENGINESERVER, nullptr);
	if (engine == nullptr)
		ThrowError("Failed to initialize IVEngineServer");

	gpGlobals = InterfacePointers::GlobalVars();

	size = ceil(engine->GetClusterCount() / 8.0f);

	LUA->PushSpecial(SPECIAL_REG);
		LUA->GetField(-1, "Entity");
			//Add_Func(LUA, SetClientVisibilityPVS, "SetClientVisibilityPVS");
			Add_Func(LUA, SetStateFlag, "SetStateFlag");
			Add_Func(LUA, GetStateFlag, "GetStateFlag");
	LUA->Pop(2);

	// This is a shitty workaround because I couldn't get it to work with GetUserType.
	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "RunString");
		LUA->PushString("\
local meta = FindMetaTable('Entity') \
local SetStateFlag = meta.SetStateFlag \
meta.SetStateFlag = function(self, ...)  \
return self:IsValid() and SetStateFlag(self:EntIndex(), ...) \
end");
		LUA->Call(1, 0);

		LUA->GetField(-1, "RunString");
		LUA->PushString("\
local meta = FindMetaTable('Entity') \
local GetStateFlag = meta.GetStateFlag \
meta.GetStateFlag = function(self, ...)  \
return self:IsValid() and GetStateFlag(self:EntIndex(), ...) \
end");
		LUA->Call(1, 0);

		LUA->PushNumber(1);
		LUA->SetField(-2, "FL_EDICT_DONTSEND");
		LUA->PushNumber(2);
		LUA->SetField(-2, "FL_EDICT_ALWAYS");
		LUA->PushNumber(3);
		LUA->SetField(-2, "FL_EDICT_PVSCHECK");
		LUA->PushNumber(4);
		LUA->SetField(-2, "FL_EDICT_FULLCHECK");

		LUA->GetField(-1, "engine");
			Add_Func(LUA, GetClusterCount, "GetClusterCount");
			Add_Func(LUA, GetClusterForOrigin, "GetClusterForOrigin");
			Add_Func(LUA, CheckAreasConnected, "CheckAreasConnected");
			Add_Func(LUA, GetArea, "GetArea");
			Add_Func(LUA, CheckOriginInPVS, "CheckOriginInPVS");
			Add_Func(LUA, CheckBoxInPVS, "CheckBoxInPVS");

			Add_Func(LUA, GetPVSForCluster, "GetPVSForCluster");
			//Add_Func(LUA, SetClientVisibilityPVS, "SetClientVisibilityPVS");
	LUA->Pop(2);


	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}