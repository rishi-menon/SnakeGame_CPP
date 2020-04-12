#pragma once

#include <sstream>
#include <functional>

#define BIT(x) 1<<(x)
#define BIND_EVENT_FN(x) std::bind(&##x, this, std::placeholders::_1)

enum class EventType
{
   None = 0,
   WindowClose, WindowResize, WindowInit,
   AppRender,  //not implemented 
   KeyPressed, KeyReleased,
   MousePressed, MouseReleased, MouseMoved
};

enum EventCategory
{
   EventCategoryNone          = BIT(0),
   EventCategoryApplication   = BIT(1),  
   EventCategoryInput         = BIT(2),
   EventCategoryKeyboard      = BIT(3),
   EventCategoryMouse         = BIT(4)
};

//static type is for dispatcher. Could be removed if dispatcher uses dynamic cast
#define EVENT_TYPE(type) virtual EventType GetEventType()      const override { return EventType::##type; } \
                         virtual const char* GetName()         const override { return #type; } \
                         static EventType GetStaticEventType()                { return EventType::##type; }

#define EVENT_CATEGORY(category) virtual int GetCategoryFlags ()   const override { return category; }


class Event
{
public:

   virtual ~Event() {}

   virtual EventType GetEventType()  const = 0;
   virtual int GetCategoryFlags ()   const = 0;

   virtual const char* GetName()     const = 0;
   virtual std::string ToString()    const { return GetName(); }

   inline bool IsInCategory(EventCategory category)
   {
      return GetCategoryFlags () & category;
   }

   bool GetHandled() const { return m_Handled; }
   //void SetHandled(bool bHandled) { m_Handled = bHandled; }

   friend class EventDispatcher;
   friend class Layer;
protected:
   bool m_Handled = false;

};

inline std::ostream& operator<< (std::ostream& os, const Event& e)
{
   return os << e.ToString();
}

class EventDispatcher
{

   template <typename T>
   using EventFn = std::function<bool (T&)>;
public:
   EventDispatcher(Event& e)
      : m_Event(e) {}

   template <typename T>
   bool Dispatch(EventFn<T> func)
   {
      if (m_Event.GetEventType() == T::GetStaticEventType())
      {
         //m_Event.SetHandled(func(static_cast<T>(m_Event)));
         m_Event.m_Handled = func(static_cast<T&>(m_Event));
         return true;
      }
      return false;
   }
private:
   Event& m_Event;
};