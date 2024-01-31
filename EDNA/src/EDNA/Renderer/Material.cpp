#include "ednapch.h"
#include "Material.h"
#include <EDNA/Renderer/Renderer.h>
#include <EDNA/Renderer/Shader.h>
#include <EDNA/Renderer/Texture.h>
#include <glm/gtc/type_ptr.hpp> 
#include "glad/glad.h"

namespace EDNA {

	Material::Material()
	{
	}

	void Material::UploadBuffers()
	{
	}

	GlyphMaterial::GlyphMaterial()
	{
		//float twoPiDiv8 = 2.f * 3.141592653f / 8.f;
		//for (int n = 0; n < 8; n++)
		//{
		//	m_BezierPoints[2*n] = std::cos(n * twoPiDiv8)*0.5f;
		//	m_BezierPoints[2 * n + 1] = std::sin(n * twoPiDiv8) * 0.5f;
		//}
	}


	void GlyphMaterial::UploadBuffers() 
	{
		auto shader = Renderer::Shaders()->Get(m_ShaderName);

		shader->SetFloatArray("u_BezierPoints", &m_BezierPoints[0], 16);
		shader->SetIntArray("u_PointIndices", &m_PointIndices[0], 8);
	}

	DebugMaterial::DebugMaterial()
	{
		//float twoPiDiv8 = 2.f * 3.141592653f / 8.f;
		//for (int n = 0; n < 8; n++)
		//{
		//	m_BezierPoints[2*n] = std::cos(n * twoPiDiv8)*0.5f;
		//	m_BezierPoints[2 * n + 1] = std::sin(n * twoPiDiv8) * 0.5f;
		//}
	}


	void DebugMaterial::UploadBuffers()
	{
		auto shader = Renderer::Shaders()->Get(m_ShaderName);

		uint32_t lower32bits = static_cast<uint32_t>(Bits & 0xFFFFFFFF);
		uint32_t upper32bits = static_cast<uint32_t>((Bits >> 32) & 0xFFFFFFFF);

		shader->SetUInt("u_LowerBits", lower32bits);
		shader->SetUInt("u_UpperBits", upper32bits);

		shader->SetFloat("u_Time", m_Time);
		shader->SetFloat("u_Angle", m_Angle);


	}

}