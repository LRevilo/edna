#include <EDNA.h>


class GamePrototype : public EDNA::Application
{
public:
	GamePrototype() 
	{

	}
	~GamePrototype() 
	{

	}
};

EDNA::Application* EDNA::CreateApplication()
{
	return new GamePrototype();
}     