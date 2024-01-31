#pragma once
#include "imgui.h"
#include "imgui_internal.h"

namespace UI {

	inline bool TexturedButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		ImGui::ImageButton(str_id, user_texture_id, size, uv0, uv1, bg_col, tint_col);
	}
}