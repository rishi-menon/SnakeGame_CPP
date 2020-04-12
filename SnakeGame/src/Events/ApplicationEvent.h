#pragma once
#include "Event.h"

class WindowCloseEvent : public Event
{
public:
   EVENT_TYPE (WindowClose)
   EVENT_CATEGORY (EventCategoryApplication)
public:
   WindowCloseEvent() {}

   std::string ToString() const override
   {
      return "WindowClosedEvent";
   }
};

class WindowResizeEvent : public Event
{
public:
   EVENT_TYPE(WindowResize)
   EVENT_CATEGORY(EventCategoryApplication)
public:
   WindowResizeEvent(unsigned int width, unsigned int height)
      : m_Width(width), m_Height(height) {}

   inline unsigned int  GetWidth() const { return m_Width; }
   inline unsigned int  GetHeight() const { return m_Height; }

   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "WindowResizeEvent: (" << m_Width << ", " << m_Height << ")";
      return ss.str();
   }

private:
   unsigned int m_Width, m_Height;
};

//class AppRenderEvent : public Event
//{
//public:
//   EVENT_TYPE(AppRender)
//   EVENT_CATEGORY(EventCategoryApplication)
//public:
//   AppRenderEvent() {}
//
//   std::string ToString() const override
//   {
//      return "AppRenderEvent";
//   }
//};

class Window;

class WindowInitEvent : public Event
{
public:
   EVENT_TYPE (WindowInit)
   EVENT_CATEGORY (EventCategoryApplication)

public:
   WindowInitEvent(Window* window)
      : m_pWindow(window) {}

   inline Window* GetWindow() { return m_pWindow; }

   std::string ToString() const override
   {
      return "WindowInitEvent";
   }

private:
   Window* m_pWindow;
};