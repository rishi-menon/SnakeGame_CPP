#pragma once
#include "LayerSnake.h"
#include "Log.h"

#include "Window/Window.h"
#include "Events/ApplicationEvent.h"

LayerSnake::LayerSnake() :

   m_player (m_cSnakeBlockSize),
   m_timeElapsed (0),
   m_cSnakeBlockSize (30),

   m_bGameOver (false),
   m_bGamePaused (false)

{
   //Set up any event callbacks.
   m_EventCallbackMap.reserve(3);
   m_EventCallbackMap.insert({ EventType::KeyPressed, BIND_EVENT_FN(LayerSnake::OnKeyPress )});
   m_EventCallbackMap.insert({ EventType::WindowInit, BIND_EVENT_FN(LayerSnake::OnWindowInit )});
   m_EventCallbackMap.insert({ EventType::WindowResize, BIND_EVENT_FN(LayerSnake::OnWindowResize) });
}
void LayerSnake::OnEvent(Event& event)
{
   //call this for the events to get dispatched to the right functions
   Layer::OnEvent(event);
   //additional stuff if required
}
bool LayerSnake::OnKeyPress(Event& event)
{
   ASSERT(event.GetEventType() == EventType::KeyPressed, "It should be a key pressed event");
   KeyPressedEvent& keyEvent = static_cast<KeyPressedEvent&> (event);
   bool bHandled = false;

   //To do own key codes
   switch (keyEvent.GetKeyCode())
   {
      case 'A':
      case 263:
      {
         m_nDirX = -1;
         m_nDirY = 0;
         bHandled = true;
         m_bGamePaused = false;
         break;
      }

      case 'D':
      case 262:
      {
         m_nDirX = 1;
         m_nDirY = 0;
         bHandled = true;
         m_bGamePaused = false;
         break;
      }
      case 'W':
      case 265:
      {
         m_nDirX = 0;
         m_nDirY = 1;
         bHandled = true;
         m_bGamePaused = false;
         break;
      }
      case 'S':
      case 264:
      {
         m_nDirX = 0;
         m_nDirY = -1;
         bHandled = true;
         m_bGamePaused = false;
         break;
      }

      case 'R':
      {
         //restart the game
         m_food.ResetFood(m_nTilesX, m_nTilesY);
         m_player.Reset();
         m_nDirX = 1;
         m_nDirY = 0;
         m_bGamePaused = false;
         m_bGameOver = false;
         break;
      }

      case 32:
      {
         //space key
         //pause game
         m_bGamePaused = true;
         m_nDirX = 0;
         m_nDirY = 0;
      }
   }
   
   //return true for now because event was handled
   return true;
}
bool LayerSnake::OnWindowResize(Event& event)
{
   ASSERT(event.GetEventType() == EventType::WindowResize, "It should be a key pressed event");

   int width =  (static_cast<WindowResizeEvent&> (event)).GetWidth();
   int height = (static_cast<WindowResizeEvent&> (event)).GetHeight();

   m_Renderer.SetProjectionMatrix(width, height);

   m_nTilesX = width / m_cSnakeBlockSize;
   m_nTilesY = height / m_cSnakeBlockSize;

   if (m_food.x >= m_nTilesX)
   {
      m_food.x = m_nTilesX - 1;
   }
   if (m_food.y >= m_nTilesY)
   {
      m_food.y = m_nTilesY - 1;
   }
   return false;
}

bool LayerSnake::OnWindowInit(Event& event)
{
   ASSERT(event.GetEventType() == EventType::WindowInit, "");

   int width =  (static_cast<WindowInitEvent&> (event)).GetWindow()->GetWidth();
   int height = (static_cast<WindowInitEvent&> (event)).GetWindow()->GetHeight();

   m_Renderer.SetProjectionMatrix(width, height);
   m_player.SetStartingPos(width / (2*m_cSnakeBlockSize), height / (2* m_cSnakeBlockSize));
   
   m_nTilesX = width / m_cSnakeBlockSize;
   m_nTilesY = height / m_cSnakeBlockSize;
   
   m_food.ResetFood(m_nTilesX, m_nTilesY);
   return false;  //return false so the event goes throgh all the layers
}
void LayerSnake::OnUpdate(Timestep deltaTime)
{
   //Do everything here
   m_timeElapsed += deltaTime;

   if (m_timeElapsed >= m_snakeMoveThresholdTime)
   {
      m_timeElapsed = 0;
      
      OnConstantUpdate();
      
   }

   OnDraw();
}
void LayerSnake::OnConstantUpdate()
{
   if (!m_bGameOver && !m_bGamePaused)
   {
      m_player.Move(m_nDirX, m_nDirY);
      m_player.Teleport(m_nTilesX, m_nTilesY);

      //check collision
      if (m_player.CheckFoodCollision(m_food.x, m_food.y))
      {
         m_food.ResetFood(m_nTilesX, m_nTilesY);
      }

      if (m_player.CheckBodyCollision())
      {
         //game over
         m_bGameOver = true;
      }
   }
   else
   {
      m_player.UpdateTrail();
   }
}
void LayerSnake::OnDraw()
{
   m_Renderer.BeginBatch();
   
   m_player.DrawTrail(m_Renderer);

   //draw food
   constexpr float drawPercent = 0.9f;
   m_Renderer.DrawQuad(m_cSnakeBlockSize * m_food.x, m_cSnakeBlockSize * m_food.y, (int)(m_cSnakeBlockSize * drawPercent), (int)(m_cSnakeBlockSize * drawPercent), m_food.col);

   m_Renderer.EndBatch();
}