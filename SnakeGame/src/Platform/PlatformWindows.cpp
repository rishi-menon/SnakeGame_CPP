#include "Platform.h"
#include "GLFW/glfw3.h"

#include <chrono> //for now

namespace Platform
{
   double GetTimeSinceStart()
   {
      return glfwGetTime();
   }

   //mainly for a seed for random number generator
   long long GetSystemTime()
   {
      //return time since epoch... might want to rename this function and call it something better
      std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
      long long value = std::chrono::time_point_cast <std::chrono::milliseconds> (timeNow).time_since_epoch().count();
      return value;
   }
}