#pragma once
#include "Events/Event.h"
#include "Timestep.h"

#include <vector>
#include <unordered_map>
#include <functional>
#include <initializer_list>

enum class LayerID : int
{
   None = -1,
   Snake,

   //insert new layers above the count enum
   LayerCount
};

enum class MessageID : int
{
   None = 0
};

using LayerMessageCallbackFn = std::function<void(LayerID, const std::initializer_list<LayerID>&, MessageID, const void* const)>;
using LayerEventCallbackFn = std::function<bool(Event&)>;
class Layer
{
   friend class LayerStack;
public:
   virtual ~Layer() {}

   virtual void OnUpdate(Timestep deltaTime) {}
   virtual void OnMessage(LayerID sender, MessageID message, const void* const data) {}
   virtual LayerID GetLayer() { return LayerID::None; };
   virtual void OnEvent(Event& event);

protected:
   //void (LayerID sender, const std::initializer_list<LayerID>& receivers, MessageID message, const void* const data);
   LayerMessageCallbackFn OnSendMessage;

   std::unordered_map<EventType, LayerEventCallbackFn> m_EventCallbackMap;
};

class LayerStack
{
public:
   LayerStack();
   ~LayerStack();
   void PushLayer(Layer* layer);
   void PopLayer(Layer* layer);

   void OnEvent(Event& event);
   void OnUpdate(Timestep deltaTime);

private:
   //send data between different layers on the LayerStack
   void OnMessage(LayerID sender, const std::initializer_list<LayerID>& receivers, MessageID message, const void* const data);

private:
   std::vector<Layer*> m_Layers;
   std::unordered_map<LayerID, Layer*> m_LayersMap;  //More efficient way than this ?
};