#include "ednapch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace EDNA {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		EDNA_CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	void OpenGLContext::Init()
	{
		EDNA_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EDNA_CORE_ASSERT(status, "Could not initialise Glad!");

		auto render_info = (const char*)glGetString(GL_RENDERER);
		EDNA_CORE_INFO("Renderer: {0}", render_info);

		auto version_info = (const char*)glGetString(GL_VERSION);
		EDNA_CORE_INFO("Version: {0}", version_info);
	}

	void OpenGLContext::SwapBuffers()
	{
		EDNA_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}

}