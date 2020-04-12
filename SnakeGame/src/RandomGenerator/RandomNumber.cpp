#include "RandomNumber.h"
#include "Platform/Platform.h" //for seed
#include <random>

namespace RandomNumber
{
   static std::default_random_engine generator;
   static std::uniform_real_distribution<double> distReal (0.0f, 1.0f);

   //static std::uniform_int_distribution<int> distInt (0, 9); //0 to 9 inclusive

   void Init()
   {
      long long timeNow = Platform::GetSystemTime();
      generator.seed(timeNow);
   }

   float GetFloat()
   {
      return (float)distReal(generator);
   }
   float GetFloat(float min, float max)
   {
      return GetFloat() * (max - min) + min;
   }

   //Get a random int from 0 to 9
   int GetInt()
   {
      //return distInt(generator);
      int val = (int)(GetFloat() * 10);
      return (val >= 10) ? 9 : val;
   }
   int GetInt(int min, int max)
   {
      int val = (int)GetFloat(min, max + 1);
      return (val >= max+1) ? max : val;
   }
}