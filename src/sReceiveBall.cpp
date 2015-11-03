#include "skillSet.h"

namespace Strategy
{
   void SkillSet::receiveBall(const SParam &param)
   {
     /*
     static int framecount = 1;
     static Vector2D<float> ballInitialpos;
     Vector2D<float> ballFinalpos;
     Vector2D<float> currentBotpos = state->homePos, botDestination;
     if(framecount == 1)
     {
       ballInitialpos = state->ballPos;
       return;
     }
     else if((framecount % 10) == 0)
     {
       framecount = 1;
       ballFinalpos = state->ballPos;
     }
     else
     {
       framecount++;
       return;
     }

     float a,b,c1,c2;
     a = ballFinalpos.x - ballInitialpos.x;
     b = ballFinalpos.y - ballFinalpos.y;
     c1 = currentBotpos.y*(-b) + currentBotpos.x*(-a);
     c2 = ballFinalpos.x*ballInitialpos.y - ballFinalpos.y*ballInitialpos.x;

     botDestination.x = -(a*c1 + b*c2)/(a*a + b*b);
     botDestination.y = (a*c2 - b*c1)/(a*a + b*b);
     param.BotConfig.x = botDestination.x;
     param.BotConfig.y = botDestination.y;
     param.BotConfig.finalslope = 0;
     goToPoint(param);
    */
   }
}
