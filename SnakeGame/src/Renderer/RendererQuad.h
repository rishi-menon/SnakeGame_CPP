#pragma once

#include "Renderer.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"


struct VertexQuad
{
   glm::vec2 position;
   Color color;
};
class RendererQuad
{
public:
   RendererQuad();
   ~RendererQuad();

   void BeginBatch();
   void EndBatch();
   void SetProjectionMatrix(int width, int height);

   void DrawQuad(int x, int y, int w, int h, const Color& col);
private:
   void Rend();
private:
   glm::mat4 m_matProjection;
   int m_nOffset;
   int m_nIndices;
   int m_nUniformMVP;

   //OpenGl Stuff
   unsigned int m_vertexArrayID;
   unsigned int m_vertexBufferID;
   unsigned int m_indexBufferID;
   unsigned int m_uShaderID;

   //constants
   const int m_cMaxVertices = 200;
   const int m_cMaxIndices = (m_cMaxVertices * 6) / 4;
};