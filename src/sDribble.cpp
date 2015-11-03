#include "skillSet.h"

namespace Strategy
{
  void SkillSet::dribble(const SParam& param)
  {
    comm.sendCommand(botID, 0, 0, 0, 0, true);
  } // dribble
}
