#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>

class WindowsWindow : public Window
{
public:
   WindowsWindow(WindowCallbackFn callbackFn, const WindowProps& props);
   ~WindowsWindow();

   void OnInit(WindowCallbackFn callbackFn, const WindowProps& props);
   void OnTerminate();

   virtual void OnUpdate() override;

   virtual unsigned int GetWidth() const override { return m_Data.Width; }
   virtual unsigned int GetHeight() const override { return m_Data.Height; }

   virtual bool IsVSync() const override { return m_Data.VSync; }
   virtual void SetVSync(bool enabled) override;

   //Window Attributes
   virtual inline void SetEventCallback(WindowCallbackFn fn) override { m_Data.eventCallback = fn; }

private:
   GLFWwindow* m_pWindow;

   struct WindowData
   {
      std::string Title;
      unsigned int Width;
      unsigned int Height;
      bool VSync;
      WindowCallbackFn eventCallback;
   };
    WindowData m_Data;
};