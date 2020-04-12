#pragma once

#define glCall(x) ClearError();\
                  x;\
                  ASSERT(CheckError(__LINE__), "")

void ClearError();
bool CheckError(int line);
unsigned int CompileShader(const char* const src, unsigned int type);

struct Color
{
   float r, g, b, a;

   Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
   Color(float R, float G, float B, float A = 1.0f);

   Color& operator = (const Color& other);
};