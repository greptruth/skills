#include "skillSet.h"
#include "beliefState.h"
#include "logger.h"

using namespace Util;

namespace Strategy
{
  void SkillSet::spin(const SParam& param)
  {
//    Logger::toStdOut("%f\n", state->homeAngle[botID]);
    comm.sendCommand(botID, 0, 0, param.SpinP.radPerSec, 0, false);
  } // spin
}
