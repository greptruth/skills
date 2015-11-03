#include "skillSet.h"
#include "beliefState.h"
#include "fieldConfig.h"

namespace Strategy
{
  void SkillSet::turnToAngle(const SParam& param)
  {
    float finalSlope = param.TurnToAngleP.finalslope;
    float turnAngleLeft = normalizeAngle(finalSlope - state->homeAngle[botID]); // Angle left to turn
    float omega = turnAngleLeft * MAX_BOT_OMEGA/(2*PI); // Speedup turn 
    if(omega < MIN_BOT_OMEGA/2 && omega > -MIN_BOT_OMEGA/2) { // This is a rare used skill so believe in Accuracy more than speed. Hence reducing minimum Omega
      if(omega < 0) omega = -MIN_BOT_OMEGA/2;
      else omega = MIN_BOT_OMEGA/2;
    }
    float dist = Vector2D<int>::dist(state->ballPos, state->homePos[botID]);
    if(dist < DRIBBLER_BALL_THRESH)
      comm.sendCommand(botID, 0, 0, omega, 0, true);
    else
      comm.sendCommand(botID, 0, 0, omega, 0, false);
  }
}
