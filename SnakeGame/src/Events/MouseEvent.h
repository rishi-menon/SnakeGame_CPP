#pragma once
#include "Event.h"

class MouseEvent : public Event
{
public:
   EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

   MouseEvent(int key)
   : m_KeyCode(key) {}

   inline int GetKeyCode() const { return m_KeyCode; }
protected:
   int m_KeyCode;
};

class MousePressedEvent : public MouseEvent
{
public:
   EVENT_TYPE(MousePressed)

   MousePressedEvent(int key)
      : MouseEvent(key) {}

   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "MousePressed: " << m_KeyCode;
      return ss.str();
   }
};

class MouseReleasedEvent : public MouseEvent
{
public:
   EVENT_TYPE(MouseReleased)

   MouseReleasedEvent(int key)
   : MouseEvent(key) {}

   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "MouseReleased: " << m_KeyCode;
      return ss.str();
   }
};

class MouseMovedEvent : public Event
{
public:
   EVENT_TYPE (MouseMoved)
   EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)
public:
   MouseMovedEvent(float x, float y)
      : m_MouseX(x), m_MouseY(y) {}

   inline float GetX() const { return m_MouseX; }
   inline float GetY() const { return m_MouseY; }

   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "MouseMovedEvent: (" << m_MouseX << ", " <<m_MouseY<<")";
      return ss.str();
   }

private:
   float m_MouseX, m_MouseY;
};