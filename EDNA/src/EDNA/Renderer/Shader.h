#pragma once

#include <string>
#include <glm/glm.hpp>

#include <unordered_map>
#include "Buffer.h"

namespace EDNA {

	class Shader
	{

	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetUInt(const std::string& name, uint32_t value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloatArray(const std::string& name, float* values, uint32_t count) = 0;
		virtual void SetMat2(const std::string& name, const glm::mat2& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void UploadBuffers() = 0;


		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		std::vector<UniformBuffer> m_UniformBuffers;

	private:
		
		

	};


	class ShaderLibrary
	{
	public:

		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filePath); // Assets/Texture.glsl
		Ref<Shader> Load(const std::string& name, const std::string& filePath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
		int Count() { return m_Shaders.size(); }


	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}