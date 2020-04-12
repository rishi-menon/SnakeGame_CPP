#pragma once

#pragma once
#include "Event.h"

class KeyEvent : public Event
{
public:
   EVENT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

   KeyEvent(int key)
      : m_KeyCode(key) {}

   inline int GetKeyCode() const { return m_KeyCode; }
protected:
   int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
   EVENT_TYPE (KeyPressed)

   KeyPressedEvent(int key, int repeat)
      : KeyEvent(key), m_RepeatCount(repeat) {}
   
   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
      return ss.str();
   }

   inline int GetRepeatCount() const { return m_RepeatCount; }

protected:
   int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
   EVENT_TYPE(KeyReleased)

   KeyReleasedEvent(int key)
      : KeyEvent(key) {}

   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "KeyReleasedEvent: " << m_KeyCode;
      return ss.str();
   }
};