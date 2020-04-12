#include "Renderer.h"

#include "GL/glew.h"
#include "Log.h"

void ClearError()
{
   while (glGetError() != GL_NO_ERROR);
}

bool CheckError(int line)
{
   bool bSafe = true;
   while (unsigned int error = glGetError())
   {
      LOG_ERROR("OpenGl Error Line {0}: Code {1}", line, error);
      bSafe = false;
   }
   return bSafe;
}


unsigned int CompileShader(const char* const src, unsigned int type)
{
   unsigned int id = glCreateShader(type);
   glShaderSource(id, 1, &src, nullptr);
   glCompileShader(id);

   int result;
   glGetShaderiv(id, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE)
   {
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char buff[100];
      glGetShaderInfoLog(id, 100, &length, buff);
      const char* pShaderName = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
      LOG_WARN("Error while compiling {0} shader: {1}", pShaderName, buff);
      ASSERT(false, "");
      return 0;
   }
   return id;
}

Color::Color(float R, float G, float B, float A)
   : r(R), g(G), b(B), a(A) {}

Color& Color::operator = (const Color& other)
{
   r = other.r;
   g = other.g;
   b = other.b;
   a = other.a;
   return (*this);
}