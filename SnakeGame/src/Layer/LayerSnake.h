#pragma once
#include "LayerStack.h"
#include "Events/KeyEvent.h"
#include "Renderer/RendererQuad.h"

#include "SnakeGameCore/Snake.h"
#include "SnakeGameCore/Food.h"

class LayerSnake : public Layer
{
private:
   LayerID ThisLayer = LayerID::Snake;

public:
   LayerSnake();

   //Overrides
   virtual void OnEvent(Event& event) override;
   virtual void OnUpdate(Timestep deltaTime) override;
   virtual LayerID GetLayer() override { return ThisLayer; }
   
private:
   void OnDraw();
   // Mainly to move the snake.. gets called wayy less frequently (m_snakeMoveFrequency times) than the OnUpdate. This is so that snake doesnt move crazy fast
   void OnConstantUpdate(); 
private:
   //event callbacks
   bool OnKeyPress(Event& event);
   bool OnWindowResize(Event& event);
   bool OnWindowInit(Event& event);

private:
   bool m_bGamePaused;
   bool m_bGameOver;
   int m_nDirX;
   int m_nDirY;

   double m_timeElapsed;
   //minimum wait time in seconds before the snake moves every time
   float m_snakeMoveThresholdTime = 1.0f / 15;  
   
   const int m_cSnakeBlockSize;
   int m_nTilesX;
   int m_nTilesY;

   Snake m_player;
   Food m_food;

   //renderer
   RendererQuad m_Renderer;
};