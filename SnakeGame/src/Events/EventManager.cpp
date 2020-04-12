
#include "EventManager.h"

std::unordered_map<EventType, std::vector<EventCallbackFn>> EventManager::s_MessageMap;

void EventManager::SendEvent(Event& message)
{
   SendEvent(message, message.GetEventType());
}
void EventManager::SendEvent(Event& message, EventType messageID)
{
   auto pair = s_MessageMap.find(messageID);
   if (pair != s_MessageMap.end())
   {
      std::vector<EventCallbackFn>& listeners = pair->second;
      for (std::size_t i = 0; i < listeners.size() && !message.GetHandled(); i++)
      {
         message.SetHandled(listeners[i](message));
      }
   }
}
void EventManager::OnEvent(EventType messageID, EventCallbackFn fn)
{
   std::vector<EventCallbackFn>& listeners = s_MessageMap[messageID];
   listeners.push_back(fn);
}
