#pragma once

#include <vector>
#include <unordered_map>

#include <functional>

#include "Event.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

using EventCallbackFn = std::function <bool(Event&)>;

//struct Message
//{
//   EventType EventID;
//   std::vector<EventCallbackFn> Listeners;
//
//   Message()
//      : EventID(EventType::None)
//   {
//      Listeners.reserve(10);
//   }
//};

class EventManager
{
public:
   static void SendEvent(Event& message);
   static void SendEvent(Event& message, EventType messageID);
   static void OnEvent(EventType messageID, EventCallbackFn fn);

private:
   static std::unordered_map<EventType, std::vector<EventCallbackFn>> s_MessageMap;
};

//#define OnEventMethod(message, method, classname) ::EventManager::OnEvent (message, std::bind(&classname::##method, this, std::placeholders::_1))

#define METHOD(classname, method, pObject)  std::bind(&classname::##method, pObject, std::placeholders::_1)