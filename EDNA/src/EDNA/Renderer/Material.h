#pragma once
#include "glm/glm.hpp"
#include <unordered_map>
#include "Shader.h"

namespace EDNA {

	class Material
	{
	public:
		Material();
		virtual void UploadBuffers();
		std::string m_MaterialName = "Material";
		std::string m_ShaderName = "Shader";
		Ref<Shader> m_Shader;
	};

	class GlyphMaterial : public Material
	{
	public:
		GlyphMaterial();
		void UploadBuffers() override;

		float m_BezierPoints[16] = {
		-0.5f, 0.f,
		0.3f, 0.3f,
		0.7f, -0.1f,
		-0.5f, -0.3f,
		0.4f, 0.8f,
		-0.2f, 0.9f,
		0.f, -0.3f,
		0.f, 0.f};

		int m_PointIndices[8] =
		{
			0,1,2,3,4,5,6,7
		};

	};

	class DebugMaterial : public Material
	{
	public:
		DebugMaterial();
		void UploadBuffers() override;

		uint64_t Bits = 0;
		float m_Time = 0;
		float m_Angle = 0;

	};

}
