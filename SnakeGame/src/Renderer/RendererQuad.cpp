
#include "RendererQuad.h"

#include "GL/glew.h"
#include "Log.h"

RendererQuad::RendererQuad()
   :m_nOffset (0), 
    m_nIndices (0)
{

   LOG_TRACE("Initialising Renderer Quad");

   //vertex array
   {
      glCall(glGenVertexArrays(1, &m_vertexArrayID));
      glCall(glBindVertexArray(m_vertexArrayID));
   }

   //vertex buffer
   {
      glCall(glGenBuffers(1, &m_vertexBufferID));
      glCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID));
      glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexQuad) * m_cMaxVertices, nullptr, GL_DYNAMIC_DRAW));
   }
   //layout
   {
      //position
      glCall(glEnableVertexAttribArray(0));
      glCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexQuad), (const void*)offsetof(VertexQuad, position)));
      //color
      glCall(glEnableVertexAttribArray(1));
      glCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexQuad), (const void*)offsetof(VertexQuad, color)));
   }

   //index buffer
   {
      //allocate on heap as the size may be very large
      unsigned int* indices = new unsigned int[m_cMaxIndices];
      unsigned int quadPattern[] = { 0, 1, 2, 2, 3, 0 };
      for (int i = 0; i < m_cMaxIndices; i += 6)
      {
         for (int j = 0; j < 6; j++)
         {
            indices[i + j] = quadPattern[j];
            quadPattern[j] += 4;
         }
      }

      glCall(glGenBuffers(1, &m_indexBufferID));
      glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID));
      glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_cMaxIndices, indices, GL_STATIC_DRAW));
      delete[] indices;
   }

   //Shader
   {
      //define source code for shader
      const char* vertexSrc = R"(

      #version 330 core

      layout(location=0) in vec4 position;
      layout(location=1) in vec4 color;

      out vec4 v_Color;
      uniform mat4 u_mvp;

      void main ()
      {
         v_Color = color;
         gl_Position = u_mvp * position;
      }
)";

      const char* fragmentSrc = R"(

      #version 330 core

      layout(location=0) out vec4 col;
      in vec4 v_Color;

      void main ()
      {
         col = v_Color;
      }
)";

      //create program here
      m_uShaderID = glCreateProgram();

      unsigned int nVertexID = CompileShader(vertexSrc, GL_VERTEX_SHADER);
      unsigned int nFragID = CompileShader(fragmentSrc, GL_FRAGMENT_SHADER);

      glCall(glAttachShader(m_uShaderID, nVertexID));
      glCall(glAttachShader(m_uShaderID, nFragID));
      glCall(glLinkProgram(m_uShaderID));
      glCall(glValidateProgram(m_uShaderID));
      
      glCall(glDeleteShader(nVertexID));
      glCall(glDeleteShader(nFragID));
      
      glCall(glUseProgram(m_uShaderID));

      m_nUniformMVP = glGetUniformLocation(m_uShaderID, "u_mvp");
      if (m_nUniformMVP == -1)
      {
         LOG_WARN("Uniform {0} is not being used in RendererQuad (Line {1})", "u_mvp", __LINE__);
      }
   }

   //projection matrix will get initialized by the WindowInitEvent
}

RendererQuad::~RendererQuad()
{
   glDeleteVertexArrays(1, &m_vertexArrayID);
   glDeleteBuffers(1, &m_vertexBufferID);

   //To Do cleanup other GL stuff
}

void RendererQuad::SetProjectionMatrix(int width, int height)
{
   m_matProjection = glm::ortho<float>(0, (float)width, 0, (float)height);
}

void RendererQuad::BeginBatch()
{
   m_nIndices = 0;
   m_nOffset = 0;
}
void RendererQuad::EndBatch()
{
   Rend();
}

void RendererQuad::Rend()
{
   glCall(glUniformMatrix4fv(m_nUniformMVP, 1, GL_FALSE, &m_matProjection[0][0]));
   glCall(glDrawElements(GL_TRIANGLES, m_nIndices, GL_UNSIGNED_INT, nullptr));
}

static void CreateVertices(VertexQuad v[4], int x, int y, int w, int h, const Color& col)
{
   for (int i = 0; i < 4; i++)
   {
      v[i].color = col;
   }

   //vertices go in the clockwise direction starting from the top left corner
   v[0].position.x = (float)(x);
   v[0].position.y = (float)(y);
                     
   v[1].position.x = (float)(x+w);
   v[1].position.y = (float)(y);
                   
   v[2].position.x = (float)(x+w);
   v[2].position.y = (float)(y+w);
                     
   v[3].position.x = (float)(x);
   v[3].position.y = (float)(y+w);

}

void RendererQuad::DrawQuad(int x, int y, int w, int h, const Color& col)
{
   VertexQuad vertices[4];
   CreateVertices(vertices, x, y, w, h, col);
   glCall(glBufferSubData(GL_ARRAY_BUFFER, m_nOffset, sizeof(VertexQuad) * 4, vertices));

   m_nOffset += sizeof(VertexQuad) * 4;
   m_nIndices += 6;

   if (m_nIndices + 6 > m_cMaxIndices)
   {
      Rend();
      m_nIndices = 0;
      m_nOffset = 0;
   }
}