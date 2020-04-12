#include "Snake.h"

Snake::Node::Node(int X, int Y, const Color& Col, Node* Next)
   : x (X), y(Y), col(Col), next(Next)
{

}

Snake::Snake(const int blockSize)
   :  m_nCount(1), //1 as there is one block in the trail
      m_nLastPosX (0), m_nLastPosY(0),
      m_cBlockSize (blockSize),
      m_trailColVal (0.4)
{
   m_pHead = new Node(0, 0, Color (m_trailColVal, m_trailColVal, m_trailColVal, 0.8f));   //head of the snake
   m_pTail = m_pHead;
}
Snake::~Snake()
{
   Node* pNode;
   while (m_pTail)
   {
      pNode = m_pTail->next;
      delete m_pTail;
      m_pTail = pNode;
   }
}

void Snake::UpdateTrail()
{
   Node* pNode = m_pTail;
   while (pNode != m_pHead)
   {
      pNode->x = pNode->next->x;
      pNode->y = pNode->next->y;
      pNode = pNode->next;
   }
}

void Snake::Teleport(int nTileX, int nTileY)
{
   int& x = m_pHead->x;
   int& y = m_pHead->y;

   //teleport if necessary
   if (x < 0)
   {
      x = nTileX - 1;
   }
   else if (x >= nTileX)
   {
      x = 0;
   }

   if (y < 0)
   {
      y = nTileY - 1;
   }
   else if (y >= nTileY)
   {
      y = 0;
   }
}
void Snake::Move(int dx, int dy)
{
   //store the position before moving so that u can grow properly
   m_nLastPosX = m_pTail->x;
   m_nLastPosY = m_pTail->y;
      
   //update trail first so that the the second block goes to the heads position which will move
   UpdateTrail();

   //now you can move the head... the second blocks position is at the head now so it will appear as if the entire snake moved
   int& x = m_pHead->x;
   int& y = m_pHead->y;
   x += dx;
   y += dy;

   //teleport or check for out of bounds now.... This is done from the layer class
}

//ONLY call at start
//X and Y are in grid coords
void Snake::SetStartingPos(int x, int y)
{
   m_nStartingPosX = x;
   m_nStartingPosY = y;
   m_pHead->x = x;
   m_pHead->y = y;
}

void Snake::DrawTrail(RendererQuad& renderer)
{
   constexpr float drawPercent = 0.9;
   Node* pNode = m_pTail;
   while (pNode)
   {
      renderer.DrawQuad(pNode->x * m_cBlockSize, pNode->y * m_cBlockSize, m_cBlockSize * drawPercent, m_cBlockSize * drawPercent, pNode->col);
      pNode = pNode->next;
   }
}

bool Snake::CheckBodyCollision()
{
   Node* pNode = m_pTail;
   while (pNode != m_pHead)
   {
      if (pNode->x == m_pHead->x && pNode->y == m_pHead->y)
      {
         return true;
      }
      pNode = pNode->next;
   }
   return false;
}

bool Snake::CheckFoodCollision(int foodX, int foodY)
{
   if (m_pHead->x == foodX && m_pHead->y == foodY)
   {
      //Collision... eat the food and grow
      Grow();
      return true;
   }
   return false;
}
void Snake::Grow()
{
   constexpr float trailMax = 0.68;
   constexpr float trailChange = 0.015;
   m_trailColVal += trailChange;
   if (m_trailColVal >= trailMax)
   {
      m_trailColVal = trailMax;
   }
   //the tail of the snake is the first element of the link list 
   Node* pNode = new Node(m_nLastPosX, m_nLastPosY, Color(m_trailColVal, m_trailColVal, m_trailColVal, 0.8f), m_pTail);
   m_pTail = pNode;
   m_nCount++;
}

void Snake::Reset()
{
   m_trailColVal = 0.4f;
   m_nLastPosX = 0;
   m_nLastPosY = 0;
   while (m_pTail != m_pHead)
   {
      Node* pNode = m_pTail->next;
      delete m_pTail;
      m_pTail = pNode;
   }
   m_pHead->x = m_nStartingPosX;
   m_pHead->y = m_nStartingPosY;
}