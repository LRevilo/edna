#pragma once


#include "EDNA/Core/Core.h"

namespace EDNA {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);

		static std::pair<float, float> GetMousePosition();
		static std::pair<float, float> GetMouseClientPosition();

		static float GetMouseX();
		static float GetMouseY();
	};
}
