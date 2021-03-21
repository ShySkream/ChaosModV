/*
	Effect by Last0xygen
*/

#include <stdafx.h>

static void OnTick()
{

	LOG(GET_ENTITY_SPEED(PLAYER_PED_ID()));
	for (Ped pd : GetAllPeds())
	{
		if (GET_ENTITY_SPEED(pd) > 5.2)
		{
			SET_PED_TO_RAGDOLL(pd, 3000, 3000, 0, true, true, false);
		}
	}
}

static RegisterEffect registerEffect(EFFECT_PEDS_SLIPPERY_PEDS, nullptr, nullptr, OnTick);
