
#include "WindowsWindow.h"


static bool s_GlfwInitialised = false;

static void GLFWErrorCallback(int error, const char* description)
{
   LOG_ERROR("GLFW Error {0}: {1}", error, description);
}

Window* Window::Create(WindowCallbackFn callbackFn, const WindowProps& props)
{
   return new WindowsWindow(callbackFn, props);
}
WindowsWindow::WindowsWindow(WindowCallbackFn callbackFn, const WindowProps& props)
{
   OnInit(callbackFn, props);
}
WindowsWindow::~WindowsWindow()
{
   OnTerminate();
}

void WindowsWindow::OnInit(WindowCallbackFn callbackFn, const WindowProps& props)
{
   LOG_INFO("Creating window \"{0}\" {1}x{2}", props.Title, props.Width, props.Height);
   m_Data.Title = props.Title;
   m_Data.Width = props.Width;
   m_Data.Height = props.Height;
   WindowsWindow::SetEventCallback(callbackFn);

   if (!s_GlfwInitialised)
   {
      int success = glfwInit();
      glfwSetErrorCallback(GLFWErrorCallback);
      s_GlfwInitialised = true;
      ASSERT(success, "Failed to initialise GLFW");
   }

   m_pWindow = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

   ASSERT(m_pWindow, "Failed to make window");
   glfwMakeContextCurrent(m_pWindow);
   glfwSetWindowUserPointer(m_pWindow, &m_Data);
   SetVSync(true);

   //Window Resize
   glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height)
   {
         WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
         data.Width = width;
         data.Height = height;
         WindowResizeEvent event(width, height);
         data.eventCallback(event);
   });

   //window close
   glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* window)
   {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      WindowCloseEvent event;
      data.eventCallback(event);
   });

   //window keys
   glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
   {
         WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
         switch (action)
         {
            case GLFW_PRESS:
            {
               KeyPressedEvent event (key, 0);
               data.eventCallback(event);
               break;
            }
            case GLFW_RELEASE:
            {
               KeyReleasedEvent event(key);
               data.eventCallback(event);
               break;
            }
            case GLFW_REPEAT:
            {
               KeyPressedEvent event(key, 1);
               data.eventCallback(event);
               break;
            }
            default:
               ASSERT(false, "Key button callback was unhandled");
         }
   });

   //mouse button
   glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods)
   {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      switch (action)
      {
      case GLFW_PRESS:
      {
         MousePressedEvent event(button);
         data.eventCallback(event);
         break;
      }
      case GLFW_RELEASE:
      {
         MouseReleasedEvent event(button);
         data.eventCallback(event);
         break;
      }
      default:
         ASSERT(false, "Mouse button callback was unhandled");
      }
   });

   glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* window, double xpos, double ypos)
   {
         WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
         MouseMovedEvent event ((float)xpos, (float)ypos);
         data.eventCallback(event);
   });
}


void WindowsWindow::OnTerminate()
{
   glfwDestroyWindow(m_pWindow);
}

void WindowsWindow::OnUpdate()
{
   glfwPollEvents();
   glfwSwapBuffers(m_pWindow);
}

void WindowsWindow::SetVSync(bool enable)
{
   m_Data.VSync = enable;
   glfwSwapInterval((int)enable);
}