#include "Application.h"
#include "Log.h"
#include "Layer/LayerSnake.h"

#include "GL/glew.h"


#include "Platform/Platform.h"
#include "Timestep.h"

#include "Renderer/Renderer.h"

Application::Application() : 
   m_LastTime(0),
   m_bRunning (true)
{
   Log::Init();

   
   m_pWindow = Window::Create(BIND_EVENT_FN(Application::OnEvent));
  

   //Init glew here for now... To Do move it to a better place
   {
      if (glewInit() != GLEW_OK)
      {
         ASSERT(false, "Failed while initialising glew");
      }
      LOG_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));

      glCall(glEnable(GL_BLEND));
      glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

   }

   m_LayerStack.PushLayer(new LayerSnake);
   //manually send the window created event... This has to be send AFTER calling PushLayer (so that the event can go through all the layers

   WindowInitEvent windowInitEvent (m_pWindow);
   OnEvent(windowInitEvent);

   const float f = 0.1f;
   glClearColor(f,f,f, 1.0f);  //to do move somewhere else
}
Application::~Application()
{
   delete m_pWindow;
}

void Application::OnEvent(Event& e)
{
   EventDispatcher dispatcher(e);
   bool bSuccess = dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
   dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
   m_LayerStack.OnEvent(e);
   
   //LOG_TRACE(e);
}
void Application::Run()
{
   while (m_bRunning)
   {
      glClear(GL_COLOR_BUFFER_BIT);

      double time = Platform::GetTimeSinceStart();
      Timestep deltaTime = time - m_LastTime;
      m_LastTime = time;

      m_LayerStack.OnUpdate(deltaTime);

      m_pWindow->OnUpdate();
   }
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
   m_bRunning = false;
   return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
   //To do make this independent of OpenGL in the future ?
   glViewport(0, 0, event.GetWidth(), event.GetHeight());
   return false;
}