#include "skillSet.h"
#include "beliefState.h"
#include "fieldConfig.h"

namespace Strategy
{
  void SkillSet::kickToPoint(const SParam &param)
  {
    Vector2D<int> destPoint(param.KickToPointP.x, param.KickToPointP.y);
    float finalSlope = Vector2D<int>::angle(destPoint, state->homePos[botID]);
    float turnAngleLeft = normalizeAngle(finalSlope - state->homeAngle[botID]); // Angle left to turn
    float dist = Vector2D<int>::dist(state->ballPos, state->homePos[botID]);
    if(dist > BOT_BALL_THRESH)
    {
      //printf("going to ball %d, %f\n", BOT_BALL_THRESH, dist);
      goToBall(param);
      return;
    }
    if(fabs(turnAngleLeft) > SATISFIABLE_THETA/2)
    {
      SkillSet::SParam paramt;
      paramt.TurnToPointP.x = destPoint.x;
      paramt.TurnToPointP.y = destPoint.y;
      paramt.TurnToPointP.max_omega = MAX_BOT_OMEGA*3;
      turnToPoint(paramt);
      return;
    }
    //printf("should kick now %f\n", turnAngleLeft);
    comm.sendCommand(botID, 0, 0, 0, param.KickToPointP.power, false);
  }
}
