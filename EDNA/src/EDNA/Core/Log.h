#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"



namespace EDNA {

	class EDNA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}
// Core Log macros
#define EDNA_CORE_TRACE(...)    ::EDNA::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EDNA_CORE_INFO(...)     ::EDNA::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EDNA_CORE_WARN(...)     ::EDNA::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EDNA_CORE_ERROR(...)    ::EDNA::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EDNA_CORE_FATAL(...)    ::EDNA::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log macros
#define EDNA_TRACE(...)         ::EDNA::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EDNA_INFO(...)          ::EDNA::Log::GetClientLogger()->info(__VA_ARGS__)
#define EDNA_WARN(...)          ::EDNA::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EDNA_ERROR(...)         ::EDNA::Log::GetClientLogger()->error(__VA_ARGS__)
#define EDNA_FATAL(...)         ::EDNA::Log::GetClientLogger()->fatal(__VA_ARGS__)