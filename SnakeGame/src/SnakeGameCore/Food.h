#pragma once
#include "Renderer/Renderer.h"   //for color
struct Food
{
   int x;
   int y;
   Color col;

   Food(const Color& Col = Color(1.0f, 0.2f, 0.2f));

   void ResetFood(const int maxBoundX, const int maxBoundY);
};