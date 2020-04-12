#pragma once
#include "Renderer/RendererQuad.h"  //Color and for drawing trail

class Snake
{
public:
   Snake(const int blockSize);
   ~Snake();

   void SetStartingPos(int x, int y);  //in pixels

   void DrawTrail(RendererQuad& renderer);

   void Move(int dx, int dy);
   void UpdateTrail();

   bool CheckFoodCollision(int foodX, int foodY);
   bool CheckBodyCollision();

   void Reset();

   void Teleport(int nTileX, int nTileY);

private:
   void Grow();

private:
   struct Node
   {
      int x;
      int y;
      Color col;
      Node* next;

      Node(int X, int Y, const Color& Col = Color(0.5f, 0.5f, 0.5f), Node* Next = nullptr);
   };

   //this is head and tail of the snake. the head will be the last element in the linked list
   Node* m_pHead;
   Node* m_pTail;

private:
   int m_nCount;
   
   const int m_cBlockSize;

   //x and y coordinates of where to add new node while growing
   int m_nLastPosX;
   int m_nLastPosY;

   //starting pos for reset
   int m_nStartingPosX;
   int m_nStartingPosY;

   float m_trailColVal;
};