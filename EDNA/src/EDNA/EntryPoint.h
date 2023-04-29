#pragma once

#ifdef EDNA_PLATFORM_WINDOWS

extern EDNA::Application* EDNA::CreateApplication();

int main(int argc, char** argv)
{
	//printf("EDNA\n");

	EDNA::Log::Init();
	EDNA_CORE_WARN("Initialised Logger...");
	int a = 5;
	EDNA_INFO("Info message Var={0}",a);

	auto app = EDNA::CreateApplication();
	app->Run();
	delete app;
}
#endif