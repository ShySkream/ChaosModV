#pragma once

#include "Util/OptionsFile.h"

enum EffectType : int;
struct EffectData;

namespace EffectConfig
{
	inline void ReadConfig(const char* szConfigPath, auto& out)
	{
		OptionsFile effectsFile(szConfigPath);

		for (int i = 0; i < _EFFECT_ENUM_MAX; i++)
		{
			EEffectType effectType = static_cast<EEffectType>(i);
			const EffectInfo& effectInfo = g_EffectsMap.at(effectType);

			// Default EffectData values
			// Enabled, TimedType, CustomTime (-1 = Disabled), Weight, Permanent, ExcludedFromVoting
			std::vector<int> rgValues{ true, static_cast<int>(EEffectTimedType::Unk), -1, 5, false, false };
			// HACK: Store EffectCustomName seperately
			std::string szValueEffectName;

			std::string szValue = effectsFile.ReadValueString(effectInfo.Id);

			if (!szValue.empty())
			{
				size_t ullSplitIndex = szValue.find(",");
				for (int j = 0; ; j++)
				{
					if (j == 6)
					{
						// HACK for EffectCustomName :(
						if (szValue != "0")
						{
							szValueEffectName = szValue;
						}

						break;
					}
					else
					{
						const std::string& szSplit = szValue.substr(0, ullSplitIndex);

						Util::TryParse<int>(szSplit, rgValues[j]);
					}

					if (ullSplitIndex == szValue.npos)
					{
						break;
					}

					szValue = szValue.substr(ullSplitIndex + 1);

					ullSplitIndex = szValue.find(",");
				}
			}

			if (!rgValues[0]) // enabled == false
			{
				if (effectInfo.EffectGroupType != EffectGroupType::None)
				{
					g_CurrentEffectGroupMemberCount[effectInfo.EffectGroupType]--;
				}

				continue;
			}

			EffectData effectData;
			if (!effectInfo.IsTimed)
			{
				effectData.TimedType = EEffectTimedType::NotTimed;
			}
			else if (rgValues[4])
			{
				effectData.TimedType = EEffectTimedType::Permanent;
			}
			else if (rgValues[2] > -1)
			{
				effectData.TimedType = EEffectTimedType::Custom;
				effectData.CustomTime = rgValues[2];
			}
			else
			{
				effectData.TimedType = static_cast<EEffectTimedType>(static_cast<EEffectTimedType>(rgValues[1]) == EEffectTimedType::Unk ? effectInfo.IsShortDuration : rgValues[1]);
			}

			effectData.WeightMult = rgValues[3];
			effectData.Weight = effectData.WeightMult; // Set initial effect weight to WeightMult
			effectData.ExcludedFromVoting = rgValues[5];
			effectData.IsMeta = effectInfo.ExecutionType == EEffectExecutionType::Meta;
			effectData.Name = effectInfo.Name;
			if (!szValueEffectName.empty())
			{
				effectData.HasCustomName = true;
				effectData.CustomName = szValueEffectName;
			}
			effectData.Id = effectInfo.Id;

			for (EEffectType effectType : effectInfo.IncompatibleWith)
			{
				effectData.IncompatibleIds.push_back(g_EffectsMap.at(effectType).Id);
			}

			effectData.EffectGroupType = effectInfo.EffectGroupType;

			out.emplace(effectType, effectData);
		}
	}
}