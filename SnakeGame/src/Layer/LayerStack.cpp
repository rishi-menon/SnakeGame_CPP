#include "LayerStack.h"
#include "Log.h"


void Layer::OnEvent(Event& event)
{
   if (event.GetHandled())
   {
      return;
   }
   std::unordered_map<EventType, LayerEventCallbackFn>::iterator it = m_EventCallbackMap.find(event.GetEventType());
   if (it != m_EventCallbackMap.end())
   {
      event.m_Handled = it->second(event);
   }
}


LayerStack::LayerStack()
{
   m_Layers.reserve    ((int)LayerID::LayerCount);
   m_LayersMap.reserve ((int)LayerID::LayerCount);
}
LayerStack::~LayerStack()
{
   for (std::vector<Layer*>::iterator it = m_Layers.begin(); it != m_Layers.end(); it++)
   {
      delete (*it);
   }
   m_Layers.clear();
   m_LayersMap.clear();
   ASSERT(m_Layers.empty(), "LayerStack is not empty");
   ASSERT(m_LayersMap.empty(), "LayersMap is not empty");
}

void LayerStack::OnMessage(LayerID sender, const std::initializer_list<LayerID>& receivers, MessageID message, const void* const data)
{
   std::initializer_list<LayerID>::const_iterator it = receivers.begin();
   for (; it != receivers.end(); it++)
   {
      ASSERT(m_LayersMap.count(*it), "Layer Map is empty");   //Bad if it fails
      m_LayersMap.at(*it)->OnMessage(sender, message, data);
   }
}

void LayerStack::PushLayer(Layer* pLayer)
{
   using namespace std::placeholders;
   pLayer->OnSendMessage = std::bind(&LayerStack::OnMessage, this, _1, _2, _3, _4);
   ASSERT(pLayer->OnSendMessage, "Failed to bind layer callback function");

   m_LayersMap.insert({ pLayer->GetLayer(), pLayer });
   m_Layers.push_back(pLayer);

}
void LayerStack::PopLayer(Layer* layer)
{
   //To Do - Change to link list maybe ?
   std::vector<Layer*>::iterator itLayer = std::find(m_Layers.begin(), m_Layers.end(), layer);
   if (itLayer != m_Layers.end())
   {
      m_Layers.erase(itLayer);
   }
   m_LayersMap.erase(layer->GetLayer());
}

void LayerStack::OnEvent(Event& event)
{
   //reverse order as last element is the topmost layer
   for (std::vector<Layer*>::reverse_iterator it = m_Layers.rbegin(); it != m_Layers.rend() && !event.GetHandled(); it++)
   {
      (*it)->OnEvent(event);
   }
}
void LayerStack::OnUpdate(Timestep deltaTime)
{
   //render... same order as the vector. first element is the bottommost layer
   for (std::vector<Layer*>::iterator it = m_Layers.begin(); it != m_Layers.end(); it++)
   {
      Layer* layer = *it;
      (*it)->OnUpdate(deltaTime);
   }
}