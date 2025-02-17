/*
	Effect by Lucas7yoshi, modified
*/

#include <stdafx.h>

static std::vector<Ped> m_peds;

//based on PedsSpawnAngryJesus.cpp
static void OnStart()
{
	static const Hash alienHash = GET_HASH_KEY("s_m_m_movalien_01");

	Ped playerPed = PLAYER_PED_ID();
	Vector3 playerPos = GET_ENTITY_COORDS(playerPed, false);

	static const Hash playerGroup = GET_HASH_KEY("PLAYER");
	static const Hash civGroup = GET_HASH_KEY("CIVMALE");
	static const Hash femCivGroup = GET_HASH_KEY("CIVFEMALE");

	Hash relationshipGroup;
	ADD_RELATIONSHIP_GROUP("_HOSTILE_ALIEN", &relationshipGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

	Ped ped = CreatePoolPed(4, alienHash, playerPos.x, playerPos.y, playerPos.z, 0.f);
	SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	SET_PED_HEARING_RANGE(ped, 9999.f);
	SET_PED_CONFIG_FLAG(ped, 281, true);

	SET_PED_COMPONENT_VARIATION(ped, 0, 0, 0, 0);
	SET_PED_COMPONENT_VARIATION(ped, 3, 0, 0, 0);
	SET_PED_COMPONENT_VARIATION(ped, 4, 0, 0, 0);
	SET_PED_COMPONENT_VARIATION(ped, 5, 0, 0, 0);
	SET_PED_COMPONENT_VARIATION(ped, 6, 0, 0, 0);

	SET_ENTITY_HEALTH(ped, 500, 0);
	SET_PED_ARMOUR(ped, 500);
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		SET_PED_INTO_VEHICLE(ped, GET_VEHICLE_PED_IS_IN(playerPed, false), -2);
	}

	SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);
	SET_PED_COMBAT_ATTRIBUTES(ped, 0, true);

	SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
	SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
	SET_PED_SUFFERS_CRITICAL_HITS(ped, false);
	
	GIVE_WEAPON_TO_PED(ped, GET_HASH_KEY("WEAPON_RAYPISTOL"), 9999, true, true); // give the alien an up n atomizer
	TASK_COMBAT_PED(ped, playerPed, 0, 16);

	SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);
}

static RegisterEffect registerEffect(EFFECT_ANGRY_ALIEN, OnStart, EffectInfo
	{
		.Name = "Spawn Angry Alien",
		.Id = "peds_angryalien",
		.EffectGroupType = EffectGroupType::SpawnEnemySpecial
	}
);