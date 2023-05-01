#include <EDNA.h>

class ExampleLayer : public EDNA::Layer {

public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
	}

	void OnUpdate() override
	{
		//EDNA_INFO("Layer Update");
	}
	void OnEvent(EDNA::Event& event) override
	{
		EDNA_TRACE("{0}", event);
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