#include "skillSet.h"

namespace Strategy
{
  void SkillSet::kick(const SParam& param)
  {
    comm.sendCommand(botID, 0, 0, 0, param.KickP.power, false);
  } // kick
}
