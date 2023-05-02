#include <EDNA.h>
#include "imgui/imgui.h"


class ExampleLayer : public EDNA::Layer {

public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
	}

	void OnUpdate() override
	{
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_TAB))
			EDNA_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(EDNA::Event& event) override
	{
		if (event.GetEventType() == EDNA::EventType::KeyPressed)
		{
			EDNA::KeyPressedEvent& e = (EDNA::KeyPressedEvent&)event;
			if (e.GetKeyCode() == EDNA_KEY_TAB)
				EDNA_TRACE("Tab key is pressed (event)!");
			EDNA_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};


class GamePrototype : public EDNA::Application
{
public:
	GamePrototype() 
	{
		PushLayer(new ExampleLayer());

	}
	~GamePrototype() 
	{

	}
};

EDNA::Application* EDNA::CreateApplication()
{
	return new GamePrototype();
}     