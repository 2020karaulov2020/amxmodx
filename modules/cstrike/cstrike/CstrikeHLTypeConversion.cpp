//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Counter-Strike Module
//

#include "CstrikeHLTypeConversion.h"

OffsetHandler*     G_OffsetHandler;
HL_TypeConversion  G_HL_TypeConversion;

void OffsetHandler::search_pev()
{
	edict_t *pEdict = INDEXENT(0);
	entvars_t *pev = VARS(pEdict);

	byte *privateData = reinterpret_cast<byte*>(pEdict->pvPrivateData);

	for (int i = 0; i < 0xFFF; i++)
	{
		entvars_t *val = *(reinterpret_cast<entvars_t**>(privateData + i));

		if (val == pev)
		{
			this->pev = i;
			return;
		}
	}

	// This should not happen.
	this->pev = 0;
}

inline edict_t* HL_TypeConversion::INDEXENT2(int iEdictNum)
{
	if (iEdictNum >= 1 && iEdictNum <= gpGlobals->maxClients)
	{
		return MF_GetPlayerEdict(iEdictNum);
	}
	else
	{
		return (*g_engfuncs.pfnPEntityOfEntIndex)(iEdictNum);
	}
}

edict_t* HL_TypeConversion::entvar_to_edict(entvars_t *pev)
{
	if (!pev)
	{
		return nullptr;
	}

	return pev->pContainingEntity;
}

int HL_TypeConversion::entvar_to_id(entvars_t *pev)
{
	if (!pev)
	{
		return -1;
	}

	if (!pev->pContainingEntity)
	{
		return -1;
	}

	return ENTINDEX(pev->pContainingEntity);
}

void* HL_TypeConversion::id_to_cbase(int index)
{
	edict_t* edict = INDEXENT2(index);
	return edict ? edict->pvPrivateData : nullptr;
}

entvars_t* HL_TypeConversion::id_to_entvar(int index)
{
	edict_t *pEdict = INDEXENT2(index);
	return pEdict ? VARS(pEdict) : nullptr;
}

entvars_t* HL_TypeConversion::cbase_to_entvar(void* cbase)
{
	if (!cbase)
	{
		return nullptr;
	}

	return *reinterpret_cast<entvars_t**>(reinterpret_cast<int8*>(cbase) + G_OffsetHandler->pev);
}

int HL_TypeConversion::cbase_to_id(void *cbase)
{
	if (!cbase)
	{
		return -1;
	}

	entvars_t *pev = this->cbase_to_entvar(cbase);

	if (!pev)
	{
		return -1;
	}

	if (!pev->pContainingEntity || FNullEnt(pev->pContainingEntity))
	{
		return -1;
	}

	return ENTINDEX(pev->pContainingEntity);
}
