#include "skillSet.h"

namespace Strategy
{
  void SkillSet::velocity(const SParam& param)
  {
    comm.sendCommand(botID, param.VelocityP.v_x, param.VelocityP.v_y, param.VelocityP.v_t, 0, false);
  } // velocity
}
