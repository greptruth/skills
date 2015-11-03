#include "skillSet.h"
#include "pathPlanners.h"
#include "beliefState.h"
#include "fieldConfig.h"
#include <math.h>
#include <stdio.h>
#define DEFEND_RADIUS 50.0

namespace Strategy
{
	void SkillSet::defendPoint(const SParam& param)
	{
		float point_ball_angle = atan2(state->ballPos.y-param.DefendPointP.y,state->ballPos.x-param.DefendPointP.x);
		Vector2D<int> dpoint;
		comm.addCircle(param.DefendPointP.x,param.DefendPointP.y,50);
		dpoint.y=param.DefendPointP.y + (param.DefendPointP.radius)*sin(point_ball_angle);

		dpoint.x=param.DefendPointP.x + (param.DefendPointP.radius)*cos(point_ball_angle);


		obstacle obs[HomeTeam::SIZE + AwayTeam::SIZE];
		for(int i = 0; i < HomeTeam::SIZE; ++i)
		{
			obs[i].x = state->homePos[i].x;
			obs[i].y = state->homePos[i].y;
			obs[i].radius = 3 * BOT_RADIUS;
		}

		for(int i = HomeTeam::SIZE; i < HomeTeam::SIZE + AwayTeam::SIZE; ++i)
		{
			obs[i].x = state->awayPos[i - HomeTeam::SIZE].x;
			obs[i].y = state->awayPos[i - HomeTeam::SIZE].y;
			obs[i].radius = 3 * BOT_RADIUS;
		}
		Vector2D<int> point, nextWP, nextNWP;
		pathPlanner->plan(state->homePos[botID],
		                  dpoint,
		                  &nextWP,
		                  &nextNWP,
		                  obs,
		                  HomeTeam::SIZE + AwayTeam::SIZE,
		                  botID,
		                  true);

		if(nextWP.valid())
		{
			comm.addCircle(nextWP.x, nextWP.y, 50);
			comm.addLine(state->homePos[botID].x, state->homePos[botID].y, nextWP.x, nextWP.y);
		}
		if(nextNWP.valid())
		{
			comm.addCircle(nextNWP.x, nextNWP.y, 50);
			comm.addLine(nextWP.x, nextWP.y, nextNWP.x, nextNWP.y);
		}

		float motionAngle = Vector2D<int>::angle(nextWP, state->homePos[botID]);

		float finalSlope;   // final slope the current bot motion should aim for!
		if(nextNWP.valid())
			finalSlope = Vector2D<int>::angle(nextNWP, nextWP);
		else
			finalSlope = Vector2D<int>::angle(nextWP, state->homePos[botID]);

		finalSlope=point_ball_angle;
		float turnAngleLeft = normalizeAngle(finalSlope - state->homeAngle[botID]); // Angle left to turn

		float omega = turnAngleLeft * MAX_BOT_OMEGA / (2 * PI); // Speedup turn
		if(omega < MIN_BOT_OMEGA && omega > -MIN_BOT_OMEGA)
		{
			if(omega < 0) omega = -MIN_BOT_OMEGA;
			else omega = MIN_BOT_OMEGA;
		}

		float dist = Vector2D<int>::dist(nextWP, state->homePos[botID]);  // Distance of next waypoint from the bot
		float theta =  motionAngle - state->homeAngle[botID];               // Angle of dest with respect to bot's frame of reference

		float profileFactor = (dist * 2 / MAX_FIELD_DIST) * MAX_BOT_SPEED;
		if(profileFactor < MIN_BOT_SPEED)
			profileFactor = MIN_BOT_SPEED;
		else if(profileFactor > MAX_BOT_SPEED)
			profileFactor = MAX_BOT_SPEED;

		if(dist < BOT_POINT_THRESH)
		{
			if((turnAngleLeft) > -DRIBBLER_BALL_ANGLE_RANGE  && (turnAngleLeft) < DRIBBLER_BALL_ANGLE_RANGE)
			{
				comm.sendCommand(botID, 0, 0, 0, 0, true); //kick
			}
			else
				comm.sendCommand(botID, 0, 0, omega, 0, true);
		}
		else
		{
			comm.sendCommand(botID, profileFactor * sin(-theta), profileFactor * cos(-theta), omega, 0, false);
		}
	}
}
