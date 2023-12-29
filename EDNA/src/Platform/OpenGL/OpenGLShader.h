#pragma once

#include "EDNA/Renderer/Shader.h"

#include <glm/glm.hpp>

// todo remove
typedef unsigned int GLenum;

namespace EDNA {

	class OpenGLShader : public Shader
	{

	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloatArray(const std::string& name, float* values, uint32_t count) override;
		virtual void SetMat2(const std::string& name, const glm::mat2& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4 & value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3 & value) override;
		virtual void SetFloat(const std::string& name, float value) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloatArray(const std::string& name, float* values, uint32_t count);
		


		void UploadUniformFloat(const std::string& name,  float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformMat2(const std::string& name, const glm::mat2& matrix);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void UploadBuffers();

	private:
		std::string ReadFile(const std::string& filePath);
			
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void ExtractUniformBuffers(const std::unordered_map<GLenum, std::string> shaderSources);

		void Compile(const std::unordered_map<GLenum, std::string> shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;


	};
}