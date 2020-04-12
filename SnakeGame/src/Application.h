#pragma once
#include "Window/Window.h"
#include "Layer/LayerStack.h"

class Application
{
public:
   Application();
   ~Application();

   void Run();
   void OnEvent(Event& e);

private:
   //event callbacks
   bool OnWindowClose(WindowCloseEvent& event);
   bool OnWindowResize(WindowResizeEvent& event);

protected:
   Window* m_pWindow;
   LayerStack m_LayerStack;

   double m_LastTime;

   bool m_bRunning;
};