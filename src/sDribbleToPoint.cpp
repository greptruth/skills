#include "skillSet.h"
#include "pathPlanners.h"
#include "beliefState.h"
#include "logger.h"
#include "fieldConfig.h"

using namespace Util;

namespace Strategy
{
  void SkillSet::dribbleToPoint(const SParam& param)
  {
    float v_x, v_y;
    Vector2D<int> point(param.DribbleToPointP.x, param.DribbleToPointP.y);
    
    float ballSlope = Vector2D<int>::angle(state->ballPos, state->homePos[botID]);
    float ballDist  = Vector2D<int>::dist(state->ballPos, state->homePos[botID]);
    float ballTheta = fabs(normalizeAngle(state->homeAngle[botID] - ballSlope));
//    if(ballDist > BOT_BALL_THRESH || ballTheta > DRIBBLER_BALL_ANGLE_RANGE)
//    {
//      goToBall(param);
//      return;
//    }

    obstacle obs[HomeTeam::SIZE + AwayTeam::SIZE];
    for (int i = 0; i < HomeTeam::SIZE; ++i)
    {
      obs[i].x = state->homePos[i].x;
      obs[i].y = state->homePos[i].y;
      obs[i].radius = 3 * BOT_RADIUS;
    }

    for (int i = HomeTeam::SIZE; i < HomeTeam::SIZE + AwayTeam::SIZE; ++i)
    {
      obs[i].x = state->awayPos[i - HomeTeam::SIZE].x;
      obs[i].y = state->awayPos[i - HomeTeam::SIZE].y;
      obs[i].radius = 3 * BOT_RADIUS;
    }
    Vector2D<int> nextWP, nextNWP;
    pathPlanner->plan(state->homePos[botID],
                      point,
                      &nextWP,
                      &nextNWP,
                      obs,
                      HomeTeam::SIZE + AwayTeam::SIZE,
                      botID,
                      true);

    if (nextWP.valid())
    {
      comm.addCircle(nextWP.x, nextWP.y, 50);
      comm.addLine(state->homePos[botID].x, state->homePos[botID].y, nextWP.x, nextWP.y);
    }
    if (nextNWP.valid())
    {
      comm.addCircle(nextNWP.x, nextNWP.y, 50);
      comm.addLine(nextWP.x, nextWP.y, nextNWP.x, nextNWP.y);
    }
  
//    Logger::toStdOut("Dribbling\n");
    float dist = Vector2D<int>::dist(nextWP, state->homePos[botID]);
//    float angle = Vector2D<int>::angle(nextWP, state->homePos[botID]);
    float angle = param.DribbleToPointP.finalslope;
    float theta = normalizeAngle(state->homeAngle[botID] - angle);
    float profileFactor = 2 * dist * MAX_BOT_SPEED / MAX_FIELD_DIST;
    if(fabs(profileFactor) < MIN_BOT_SPEED )
    {
      if(profileFactor < 0 )
        profileFactor = - MIN_BOT_SPEED;
      else
        profileFactor = MIN_BOT_SPEED;
    }
    v_y = profileFactor * cos(theta);
    v_x = profileFactor * sin(theta);
    float romega = theta / ( 2 * PI ) * MAX_BOT_OMEGA;
    if(v_y < - MAX_BACK_DRIBBLE_V_Y)
    {
      v_x /= (v_y / -MAX_BACK_DRIBBLE_V_Y);
      v_y = -MAX_BACK_DRIBBLE_V_Y;
    }
  else if(v_y > MAX_FRONT_DRIBBLE_V_Y)
  {
    v_x /= (v_y / MAX_FRONT_DRIBBLE_V_Y);
    v_y = MAX_FRONT_DRIBBLE_V_Y;
  }
  
    if(v_x > MAX_DRIBBLE_V_X)
    {
      // Clamp right velocity
      v_y /= (v_x / MAX_DRIBBLE_V_X);
      v_x = MAX_DRIBBLE_V_X;
    }
  else if(v_x < -MAX_DRIBBLE_V_X)
  {
    v_y /= (v_x / -MAX_DRIBBLE_V_X);
    v_x = -MAX_DRIBBLE_V_X;
  }
  
  if(fabs(theta)> SATISFIABLE_THETA)
  { 
    if(fabs(romega < MIN_BOT_OMEGA))
    {
      if(romega > 0)
        romega = MIN_BOT_OMEGA;
      else
        romega = -MIN_BOT_OMEGA;
    }
    else if(fabs(romega) > MAX_DRIBBLE_R)
    {
      if(romega > 0)
        romega = MAX_DRIBBLE_R;
      else
        romega = -MAX_DRIBBLE_R;
    }
  }else
  {
    romega = 0;
  }
    if(dist < BOT_BALL_THRESH )
    {
      comm.sendCommand(botID, 0, 0, 0, 0, true);
    }
    else
    {
      comm.sendCommand(botID, v_x, v_y, -romega, 0, true);
    }
  } // dribbleToPoint
}
