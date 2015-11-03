#include "skillSet.h"
#include "beliefState.h"
#include "fieldConfig.h"

namespace Strategy
{
  void SkillSet::turnToPoint(const SParam& param)
  {
    Vector2D<int> point(param.TurnToPointP.x, param.TurnToPointP.y);
    float finalSlope = Vector2D<int>::angle(point, state->homePos[botID]);
    float turnAngleLeft = normalizeAngle(finalSlope - state->homeAngle[botID]); // Angle left to turn
    float omega = turnAngleLeft * param.TurnToPointP.max_omega / (2 * PI); // Speedup turn
    if(omega < MIN_BOT_OMEGA && omega > -MIN_BOT_OMEGA)
    {
      if(omega < 0) omega = -MIN_BOT_OMEGA;
      else omega = MIN_BOT_OMEGA;
    }
    float v_x = omega*BOT_BALL_THRESH*1.5;
    comm.addCircle(state->homePos[botID].x,  state->homePos[botID].y, 50);
    float dist = Vector2D<int>::dist(state->ballPos, state->homePos[botID]);
    if(dist < DRIBBLER_BALL_THRESH*1.2)
      comm.sendCommand(botID, v_x, 0, omega, 0, true);
    else
      comm.sendCommand(botID, 0, 0, omega, 0, false);
  }
}
