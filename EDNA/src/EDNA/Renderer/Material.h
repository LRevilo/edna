#pragma once

#include <EDNA/Renderer/Shader.h>
#include <EDNA/Renderer/Texture.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <EDNA/Renderer/Buffer.h>


namespace EDNA {


	class Material
	{
	public:






	private:
		Ref<Shader> m_Shader;
		std::unordered_map<std::string, Ref<Texture>> m_Textures;

		std::vector<Ref<UniformBuffer>> m_UniformBuffers;


	};

}
