#include "Thorn/pch.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Thorn
{
    Event<> Window::OnWindowClose; // No arguments
    Event<int, int> Window::OnWindowResize; // Width, Height
    Event<int> Window::OnKeyPressed; // Key
    Event<int> Window::OnKeyReleased; // Key
    Event<double, double> Window::OnMouseMoved; // Mouse X, Mouse Y
    Event<float> Window::OnMouseScrolled; // Scrolled
    Event<int> Window::OnMouseButtonPressed; // Button
    Event<int> Window::OnMouseButtonReleased; // Button

    Window::Window(const WindowSpecs& specs)
        : m_NativeWindow(nullptr), m_Specs(specs)
    {
        if (!glfwInit())
        {
            Log::Error("Failed to init GLFW");
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_NativeWindow = glfwCreateWindow(specs.Width, specs.Height, specs.Title.c_str(), nullptr, nullptr);
        if (!m_NativeWindow)
        {
            Log::Error("Failed to create window");
            glfwTerminate();
            return;
        }

        GLFWwindow* m_Window = (GLFWwindow*)m_NativeWindow;
        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Log::Error("Failed to init GLAD");
            return;
        }

        glfwSetErrorCallback([](int error, const char* desc)
        {
            Log::Error(desc);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            Window::OnWindowClose();
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            Window* wWindow = ((Window*)glfwGetWindowUserPointer(window));
            wWindow->m_Specs.Width = width;
            wWindow->m_Specs.Height = height;

            Window::OnWindowResize(width, height);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
                Window::OnKeyPressed(key);
            else if (action == GLFW_RELEASE)
                Window::OnKeyReleased(key);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            Window::OnMouseMoved(xpos, ypos);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            Window::OnMouseScrolled((float)yoffset);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                Window::OnMouseButtonPressed(button);
            }
            else if (action == GLFW_RELEASE)
                Window::OnMouseButtonReleased(button);
        });

        Log::Info("Window Created {} {}:{}", specs.Title, specs.Width, specs.Height);
    }

    Window::~Window()
    {
        glfwDestroyWindow((GLFWwindow*)m_NativeWindow);
        glfwTerminate();
    }

    void Window::OnUpdate()
    {
        glfwSwapBuffers((GLFWwindow*)m_NativeWindow);
        glfwPollEvents();
    }
}