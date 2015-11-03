#include "skillSet.h"
#include "pathPlanners.h"
#include "beliefState.h"
#include "fieldConfig.h"

namespace Strategy
{
  void SkillSet::goalKeeping(SParam &param)
  {
    static int framecount = 1;
    static Vector2D<int> ballInitialpos;
    Vector2D<int> ballFinalpos, botDestination, point, nextWP, nextNWP;;
//    if(framecount == 1)
//    {
//      ballInitialpos = state->ballPos;
//      framecount++;
//      return;
//    }
//    else if((framecount % 5) == 0)
//    {
//      framecount = 1;
//      ballFinalpos = state->ballPos;
//    }
//    else
//    {
//      framecount++;
//      return;
//    }

    //if bot moves parallel to x axis (y is constant)
    botDestination.y = state->homePos[botID].y;
    botDestination.x = ((botDestination.y - ballFinalpos.y) * (ballFinalpos.x - ballInitialpos.x) / (ballFinalpos.y - ballInitialpos.y)) + ballFinalpos.x;

    //if bot moves parallel to y axis (x is constant)
    /*botDestination.x = state->homePos[botID].x;
    botDestination.y = (ballFinalpos.y - ballInitialpos.y)/(ballFinalpos.x - ballInitialpos.x)*(botDestination.x - ballFinalpos.x) + ballFinalpos.y;*/

    param.GoalKeepingP.x = botDestination.x;
    param.GoalKeepingP.y = botDestination.y;
    param.GoalKeepingP.finalslope = 0;
//    goToPointFast(param);

    float v_x, v_y, v_t;

    point.x = param.GoalKeepingP.x;
    point.y = param.GoalKeepingP.y;

    obstacle obs[HomeTeam::SIZE + AwayTeam::SIZE];

    for (int i = 0; i < HomeTeam::SIZE; i++)
    {
      obs[i].x = state->homePos[i].x;
      obs[i].y = state->homePos[i].y;
      obs[i].radius = 2 * BOT_RADIUS;
    }

    for (int i = HomeTeam::SIZE; i < HomeTeam::SIZE + AwayTeam::SIZE; i++)
    {
      obs[i].x = state->awayPos[i - HomeTeam::SIZE].x;
      obs[i].y = state->awayPos[i - HomeTeam::SIZE].y;
      obs[i].radius = 2 * BOT_RADIUS;
    }

    pathPlanner->plan(state->homePos[botID],
                      point,
                      &nextWP,
                      &nextNWP,
                      obs,
                      HomeTeam::SIZE + AwayTeam::SIZE,
                      botID,
                      true);

    float angle = Vector2D<int>::angle(nextWP, state->homePos[botID]);
    float dist = Vector2D<int>::dist(point, state->homePos[botID]);
    float theta = (state->homeAngle[botID] - angle);
    float rot_theta = (state->homeAngle[botID] - param.GoalKeepingP.finalslope) * (180 / PI);
    if(rot_theta > 0)
    {
      if(rot_theta < DRIBBLER_BALL_ANGLE_RANGE)
      {
        v_t = 0;
      }
      if(rot_theta < 45)
      {
        v_t = -rot_theta / 10;
      }
      else
      {
        v_t = -4.5f;
      }
    }
    float profileFactor = MAX_BOT_SPEED;
    if(profileFactor < MIN_BOT_SPEED)
      profileFactor = MIN_BOT_SPEED;
    v_x = profileFactor * sin(theta);
    v_y = profileFactor * cos(theta);

    if(dist < BOT_BALL_THRESH)
    {
      comm.sendCommand(botID, 0, 0, 0, 0, false);
    }
    else
    {
      comm.sendCommand(botID, v_x, v_y, v_t, 0, false);
    }
  }
}
