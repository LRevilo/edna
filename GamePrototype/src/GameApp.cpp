#include <EDNA.h>
// EntryPoint -------------
#include "EDNA/Core/EntryPoint.h"
// ------------------------

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MainLayer.h"


class GamePrototype : public EDNA::Application
{
public:
	GamePrototype() 
	{
		EDNA_INFO("GamePrototype constructor");

		PushLayer(new EDNA::MainLayer());

	}
	~GamePrototype() 
	{

	}
};

EDNA::Application* EDNA::CreateApplication()
{
	EDNA_INFO("CreateApplication");
	return new GamePrototype();
}     