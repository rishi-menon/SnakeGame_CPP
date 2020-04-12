#pragma once

//Events
#include "Events\Event.h"
#include "Events\ApplicationEvent.h"
#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"

#include <string>
//#include "Events/EventManager.h"

#include <functional>

#include "Log.h"  //for assert - debugging

using WindowCallbackFn = std::function<void (Event&)>;

struct WindowProps
{
   std::string Title;
   unsigned int Width;
   unsigned int Height;

   WindowProps(const std::string& title = "Snake Game", unsigned int width = 1280, unsigned int height = 720)
      : Title(title), Width(width), Height(height) {}
};

class Window
{
public:
   virtual void OnUpdate() = 0;

   virtual unsigned int GetWidth() const = 0;
   virtual unsigned int GetHeight() const = 0;
   
   virtual bool IsVSync() const = 0;
   virtual void SetVSync(bool enabled) = 0;

   //Window Attributes
   virtual void SetEventCallback(WindowCallbackFn) = 0;

   //Will be defined in each platform window file
   static Window* Create(WindowCallbackFn, const WindowProps& props = WindowProps());
};
