#include "Food.h"
#include "RandomGenerator/RandomNumber.h"
#include "Log.h"

Food::Food(const Color& Col) :
   col(Col) 
{

}

void Food::ResetFood(const int maxBoundX, const int maxBoundY)
{
   // -1 because its starts from 0 (similar to array)
   // if there are 5 columns, then u want x coordinate to go from 0 to 4 and GetInt() is inclusive.

   {
      int nAttempts = 0;
      int newx;
      do
      {
         newx = RandomNumber::GetInt(0, maxBoundX - 1);
         nAttempts++;

      } while (x == newx && nAttempts < 10);
      ASSERT(x != newx, "");
      x = newx;
   }

   {
      int nAttempts = 0;
      int newy;
      do
      {
         newy = RandomNumber::GetInt(0, maxBoundY - 1);
         nAttempts++;

      } while (y == newy && nAttempts < 10);
      ASSERT(y != newy, "");
      y = newy;
   }
}