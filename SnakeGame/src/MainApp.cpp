#include "Application.h"
#include "GL/glew.h"

int main()
{  
   Application* pApplication = new Application ();

   pApplication->Run();

   delete pApplication;
   return 0;
}