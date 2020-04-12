#pragma once

//could make it a staic class ?.... namespace is sufficenet for now
namespace RandomNumber
{
   extern void Init();
   extern float GetFloat();
   extern float GetFloat(float min, float max);

   extern int GetInt();
   extern int GetInt(int min, int max);
}