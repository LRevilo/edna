#pragma once

#include "EDNA/Input.h"
#include <GLFW/glfw3.h>


namespace EDNA {


	class WindowsInput : public Input
	{

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> GetMousePositionImpl() override;

		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}