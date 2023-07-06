#include "util2.h"

GarrysMod::Lua::ILuaBase* GlobalLUA;
// CGlobalVarsBase* gpGlobals;
IVEngineServer* engine;

// should never be used outside of main thread!!!
void LuaPrint(const char* Text) {
	GlobalLUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		GlobalLUA->GetField(-1, "print");
		GlobalLUA->PushString(Text);
		GlobalLUA->Call(1, 0);
	GlobalLUA->Pop();
}

// should never be used outside of main thread!!!
void LuaPrint(std::string Text) {
	GlobalLUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		GlobalLUA->GetField(-1, "print");
		GlobalLUA->PushString(Text.c_str());
		GlobalLUA->Call(1, 0);
	GlobalLUA->Pop();
}

// should never be used outside of main thread!!!
void LuaPrint(const char* Text, GarrysMod::Lua::ILuaBase* LUA) {
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "print");
		LUA->PushString(Text);
		LUA->Call(1, 0);
	LUA->Pop();
}

// should never be used outside of main thread!!!
void LuaPrint(std::string Text, GarrysMod::Lua::ILuaBase* LUA) {
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "print");
		LUA->PushString(Text.c_str());
		LUA->Call(1, 0);
	LUA->Pop();
}

void Start_Table(GarrysMod::Lua::ILuaBase* LUA) {
	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->CreateTable();
}

void Add_Func(GarrysMod::Lua::ILuaBase* LUA, CFunc Func, const char* Name) {
	LUA->PushCFunction(Func);
	LUA->SetField(-2, Name);
}

void Add_Func(GarrysMod::Lua::ILuaBase* LUA, CFunc Func, std::string Name) {
	LUA->PushCFunction(Func);
	LUA->SetField(-2, Name.c_str());
}

void Finish_Table(GarrysMod::Lua::ILuaBase* LUA, const char* Name) {
	LUA->SetField(-2, Name);
	LUA->Pop();
}

void Finish_Table(GarrysMod::Lua::ILuaBase* LUA, std::string Name) {
	LUA->SetField(-2, Name.c_str());
	LUA->Pop();
}

void ThrowError(const char* error) {
	GlobalLUA->ThrowError(error);
}

/*class CBasePlayer;
class CUserCmd
{
public:
	CUserCmd()
	{
		Reset();
	}

	virtual ~CUserCmd() { };

	void Reset()
	{
		command_number = 0;
		tick_count = 0;
		viewangles.Init();
		forwardmove = 0.0f;
		sidemove = 0.0f;
		upmove = 0.0f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;
#ifdef GAME_DLL
		server_random_seed = 0;
#endif
		mousedx = 0;
		mousedy = 0;

		hasbeenpredicted = false;
#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
		entitygroundcontact.RemoveAll();
#endif
	}

	CUserCmd& operator =(const CUserCmd& src)
	{
		if (this == &src)
			return *this;

		command_number = src.command_number;
		tick_count = src.tick_count;
		viewangles = src.viewangles;
		forwardmove = src.forwardmove;
		sidemove = src.sidemove;
		upmove = src.upmove;
		buttons = src.buttons;
		impulse = src.impulse;
		weaponselect = src.weaponselect;
		weaponsubtype = src.weaponsubtype;
		random_seed = src.random_seed;
#ifdef GAME_DLL
		server_random_seed = src.server_random_seed;
#endif
		mousedx = src.mousedx;
		mousedy = src.mousedy;

		hasbeenpredicted = src.hasbeenpredicted;

#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
		entitygroundcontact = src.entitygroundcontact;
#endif

		return *this;
	}

	CUserCmd(const CUserCmd& src)
	{
		*this = src;
	}

	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));
		CRC32_Final(&crc);

		return crc;
	}

	// Allow command, but negate gameplay-affecting values
	void MakeInert(void)
	{
		viewangles = vec3_angle;
		forwardmove = 0.f;
		sidemove = 0.f;
		upmove = 0.f;
		buttons = 0;
		impulse = 0;
	}

	// For matching server and client commands for debugging
	int		command_number;

	// the tick the client created this command
	int		tick_count;

	// Player instantaneous view angles.
	QAngle	viewangles;
	// Intended velocities
	//	forward velocity.
	float	forwardmove;
	//  sideways velocity.
	float	sidemove;
	//  upward velocity.
	float	upmove;
	// Attack button states
	int		buttons;
	// Impulse command issued.
	byte    impulse;
	// Current weapon id
	int		weaponselect;
	int		weaponsubtype;

	int		random_seed;	// For shared random functions
#ifdef GAME_DLL
	int		server_random_seed; // Only the server populates this seed
#endif

	short	mousedx;		// mouse accum in x from create move
	short	mousedy;		// mouse accum in y from create move

	// Client only, tracks whether we've predicted this command at least once
	bool	hasbeenpredicted;

	// Back channel to communicate IK state
#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
	CUtlVector< CEntityGroundContact > entitygroundcontact;
#endif

};

abstract_class IGameSystem
{
public:
	// GameSystems are expected to implement these methods.
	virtual char const* Name() = 0;

	// Init, shutdown
	// return true on success. false to abort DLL init!
	virtual bool Init() = 0;
	virtual void PostInit() = 0;
	virtual void Shutdown() = 0;

	// Level init, shutdown
	virtual void LevelInitPreEntity() = 0;
	// entities are created / spawned / precached here
	virtual void LevelInitPostEntity() = 0;

	virtual void LevelShutdownPreClearSteamAPIContext() {};
	virtual void LevelShutdownPreEntity() = 0;
	// Entities are deleted / released here...
	virtual void LevelShutdownPostEntity() = 0;
	// end of level shutdown

	// Called during game save
	virtual void OnSave() = 0;

	// Called during game restore, after the local player has connected and entities have been fully restored
	virtual void OnRestore() = 0;

	// Called every frame. It's safe to remove an igamesystem from within this callback.
	virtual void SafeRemoveIfDesired() = 0;

	virtual bool	IsPerFrame() = 0;

	// destructor, cleans up automagically....
	virtual ~IGameSystem();

	// Client systems can use this to get at the map name
	static char const* MapName();

	// These methods are used to add and remove server systems from the
	// main server loop. The systems are invoked in the order in which
	// they are added.
	static void Add(IGameSystem* pSys);
	static void Remove(IGameSystem* pSys);
	static void RemoveAll();

	// These methods are used to initialize, shutdown, etc all systems
	static bool InitAllSystems();
	static void PostInitAllSystems();
	static void ShutdownAllSystems();
	static void LevelInitPreEntityAllSystems(char const* pMapName);
	static void LevelInitPostEntityAllSystems();
	static void LevelShutdownPreClearSteamAPIContextAllSystems(); // Called prior to steamgameserverapicontext->Clear()
	static void LevelShutdownPreEntityAllSystems();
	static void LevelShutdownPostEntityAllSystems();

	static void OnSaveAllSystems();
	static void OnRestoreAllSystems();

	static void SafeRemoveIfDesiredAllSystems();

#ifdef CLIENT_DLL
	static void PreRenderAllSystems();
	static void UpdateAllSystems(float frametime);
	static void PostRenderAllSystems();
#else
	static void FrameUpdatePreEntityThinkAllSystems();
	static void FrameUpdatePostEntityThinkAllSystems();
	static void PreClientUpdateAllSystems();

	// Accessors for the above function
	static CBasePlayer* RunCommandPlayer();
	static CUserCmd* RunCommandUserCmd();
#endif
};

// Quick and dirty server system for users who don't care about precise ordering
// and usually only want to implement a few of the callbacks
class CBaseGameSystem : public IGameSystem
{
public:

	virtual char const* Name() { return "unnamed"; }

	// Init, shutdown
	// return true on success. false to abort DLL init!
	virtual bool Init() { return true; }
	virtual void PostInit() {}
	virtual void Shutdown() {}

	// Level init, shutdown
	virtual void LevelInitPreEntity() {}
	virtual void LevelInitPostEntity() {}
	virtual void LevelShutdownPreClearSteamAPIContext() {}
	virtual void LevelShutdownPreEntity() {}
	virtual void LevelShutdownPostEntity() {}

	virtual void OnSave() {}
	virtual void OnRestore() {}
	virtual void SafeRemoveIfDesired() {}

	virtual bool	IsPerFrame() { return false; }
private:

	// Prevent anyone derived from CBaseGameSystem from implementing these, they need
	//  to derive from CBaseGameSystemPerFrame below!!!
#ifdef CLIENT_DLL
	// Called before rendering
	virtual void PreRender() {}

	// Gets called each frame
	virtual void Update(float frametime) {}

	// Called after rendering
	virtual void PostRender() {}
#else
	// Called each frame before entities think
	virtual void FrameUpdatePreEntityThink() {}
	// called after entities think
	virtual void FrameUpdatePostEntityThink() {}
	virtual void PreClientUpdate() {}
#endif
};

// Quick and dirty server system for users who don't care about precise ordering
// and usually only want to implement a few of the callbacks
class CAutoGameSystem : public CBaseGameSystem
{
public:
	CAutoGameSystem(char const* name = NULL);	// hooks in at startup, no need to explicitly add
	CAutoGameSystem* m_pNext;

	virtual char const* Name() { return m_pszName ? m_pszName : "unnamed"; }

private:
	char const* m_pszName;
};

//-----------------------------------------------------------------------------
// Purpose: Helper for UTIL_FindClientInPVS
// Input  : check - last checked client
// Output : static int UTIL_GetNewCheckClient
//-----------------------------------------------------------------------------
// FIXME:  include bspfile.h here?
class CCheckClient : public CAutoGameSystem
{
public:
	CCheckClient(char const* name) : CAutoGameSystem(name)
	{
	}

	void LevelInitPreEntity()
	{
		m_checkCluster = -1;
		m_lastcheck = 1;
		m_lastchecktime = -1;
		m_bClientPVSIsExpanded = false;
	}

	byte	m_checkPVS[MAX_MAP_LEAFS / 8];
	byte	m_checkVisibilityPVS[MAX_MAP_LEAFS / 8];
	int		m_checkCluster;
	int		m_lastcheck;
	float	m_lastchecktime;
	bool	m_bClientPVSIsExpanded;
};

CCheckClient g_CheckClient("CCheckClient");

inline CBaseEntity* GetContainingEntity(edict_t* pent);

static int UTIL_GetNewCheckClient(int check)
{
	int		i;
	edict_t* ent;
	Vector	org;

	// cycle to the next one

	if (check < 1)
		check = 1;
	if (check > gpGlobals->maxClients)
		check = gpGlobals->maxClients;

	if (check == gpGlobals->maxClients)
		i = 1;
	else
		i = check + 1;

	for (; ; i++)
	{
		if (i > gpGlobals->maxClients)
		{
			i = 1;
		}

		ent = engine->PEntityOfEntIndex(i);
		if (!ent)
			continue;

		// Looped but didn't find anything else
		if (i == check)
			break;

		if (!ent->GetUnknown())
			continue;

		CBaseEntity* entity = GetContainingEntity(ent);
		if (!entity)
			continue;

		if (entity->GetFlags() & FL_NOTARGET)
			continue;

		// anything that is a client, or has a client as an enemy
		break;
	}

	if (i != check)
	{
		memset(g_CheckClient.m_checkVisibilityPVS, 0, sizeof(g_CheckClient.m_checkVisibilityPVS));
		g_CheckClient.m_bClientPVSIsExpanded = false;
	}

	if (ent)
	{
		// get the PVS for the entity
		CBaseEntity* pce = GetContainingEntity(ent);
		if (!pce)
			return i;

		org = pce->EyePosition();

		int clusterIndex = engine->GetClusterForOrigin(org);
		if (clusterIndex != g_CheckClient.m_checkCluster)
		{
			g_CheckClient.m_checkCluster = clusterIndex;
			engine->GetPVSForCluster(clusterIndex, sizeof(g_CheckClient.m_checkPVS), g_CheckClient.m_checkPVS);
		}
	}

	return i;
}


static edict_t* UTIL_GetCurrentCheckClient()
{
	edict_t* ent;

	// find a new check if on a new frame
	float delta = gpGlobals->curtime - g_CheckClient.m_lastchecktime;
	if (delta >= 0.1 || delta < 0)
	{
		g_CheckClient.m_lastcheck = UTIL_GetNewCheckClient(g_CheckClient.m_lastcheck);
		g_CheckClient.m_lastchecktime = gpGlobals->curtime;
	}

	// return check if it might be visible	
	ent = engine->PEntityOfEntIndex(g_CheckClient.m_lastcheck);

	// Allow dead clients -- JAY
	// Our monsters know the difference, and this function gates alot of behavior
	// It's annoying to die and see monsters stop thinking because you're no longer
	// "in" their PVS
	if (!ent || ent->IsFree() || !ent->GetUnknown())
	{
		return NULL;
	}

	return ent;
}

void UTIL_SetClientVisibilityPVS(edict_t* pClient, const unsigned char* pvs, int pvssize)
{
	if (pClient == UTIL_GetCurrentCheckClient())
	{
		Assert(pvssize <= sizeof(g_CheckClient.m_checkVisibilityPVS));

		g_CheckClient.m_bClientPVSIsExpanded = false;

		unsigned* pFrom = (unsigned*)pvs;
		unsigned* pMask = (unsigned*)g_CheckClient.m_checkPVS;
		unsigned* pTo = (unsigned*)g_CheckClient.m_checkVisibilityPVS;

		int limit = pvssize / 4;
		int i;

		for (i = 0; i < limit; i++)
		{
			pTo[i] = pFrom[i] & ~pMask[i];

			if (pFrom[i])
			{
				g_CheckClient.m_bClientPVSIsExpanded = true;
			}
		}

		int remainder = pvssize % 4;
		for (i = 0; i < remainder; i++)
		{
			((unsigned char*)&pTo[limit])[i] = ((unsigned char*)&pFrom[limit])[i] & !((unsigned char*)&pMask[limit])[i];

			if (((unsigned char*)&pFrom[limit])[i] != 0)
			{
				g_CheckClient.m_bClientPVSIsExpanded = true;
			}
		}
	}
}*/