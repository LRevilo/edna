#pragma once

#ifdef EDNA_PLATFORM_WINDOWS

extern EDNA::Application* EDNA::CreateApplication();

int main(int argc, char** argv)
{
	//printf("EDNA\n");

	EDNA::Log::Init();
	EDNA_CORE_INFO("Initialised Logger...");

	EDNA_PROFILE_BEGIN_SESSION("Startup", "EDNAProfile-Startup.json");
	auto app = EDNA::CreateApplication();
	EDNA_PROFILE_END_SESSION();

	EDNA_PROFILE_BEGIN_SESSION("Runtime", "EDNAProfile-Runtime.json");
	app->Run();
	EDNA_PROFILE_END_SESSION();

	EDNA_PROFILE_BEGIN_SESSION("Shutdown", "EDNAProfile-Shutdown.json");
	delete app;
	EDNA_PROFILE_END_SESSION();
}
#endif