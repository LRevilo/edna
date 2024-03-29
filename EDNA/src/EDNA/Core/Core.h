#pragma once

#include <memory>

#ifdef EDNA_PLATFORM_WINDOWS
#if EDNA_DYNAMIC_LINK
	#ifdef EDNA_BUILD_DLL
		#define EDNA_API __declspec(dllexport)
	#else
		#define EDNA_API __declspec(dllimport)
	#endif
#else
	#define EDNA_API
#endif
#else
	#error Unsupported OS
#endif

#ifdef EDNA_DEBUG
	#define EDNA_ENABLE_ASSERTS
#endif

#ifdef EDNA_ENABLE_ASSERTS
	#define EDNA_ASSERT(x, ...) { if(!(x)) { EDNA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EDNA_CORE_ASSERT(x, ...) { if(!(x)) { EDNA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EDNA_ASSERT(x, ...)
	#define EDNA_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define EDNA_BIND_EVENT_FN(fn) [this](auto&&... args)->decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

//#define GLM_FORCE_CTOR_INIT

namespace EDNA {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}