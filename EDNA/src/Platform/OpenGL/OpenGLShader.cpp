#include "ednapch.h"
#include "OpenGLShader.h"
#include "EDNA/Renderer/Buffer.h"
#include <fstream>

#include <iostream>

#include <sstream>
#include <regex>
#include <glad/glad.h>
 
#include <glm/gtc/type_ptr.hpp>
#include <map>



namespace EDNA {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") 
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;

		EDNA_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	static DataType DataTypeFromString(const std::string& type)
	{
		if (type == "float")	   return DataType::Float;
		else if (type == "vec2")   return DataType::Float2;
		else if (type == "vec3")   return DataType::Float3;
		else if (type == "vec4")   return DataType::Float4;
		else if (type == "mat2")   return DataType::Mat2;
		else if (type == "mat3")   return DataType::Mat3;
		else if (type == "mat4")   return DataType::Mat4;
		else if (type == "int")	   return DataType::Int;
		else if (type == "int2")   return DataType::Int2;
		else if (type == "int3")   return DataType::Int3;
		else if (type == "int4")   return DataType::Int4;
		else if (type == "bool")   return DataType::Bool;

		EDNA_CORE_ASSERT(false, "Unknown data type!");
		return DataType::None;
	}





	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			EDNA_CORE_ERROR("OpenGL Error: {0}", error);
		}

		// extract name
		// assets/shaders/   texture   .glsl
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');

		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);

	}



	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
		ExtractUniformBuffers(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		//EDNA_CORE_INFO("BINDING SHADER {0}", m_RendererID);
		glUseProgram(m_RendererID);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			EDNA_CORE_INFO("Error binding shader!");
		}
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetUInt(const std::string& name, uint32_t value)
	{
		UploadUniformUInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloatArray(const std::string& name, float* values, uint32_t count)
	{
		UploadUniformFloatArray(name, values, count);
	}

	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& value)
	{
			UploadUniformMat2(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		UploadUniformMat3(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformUInt(const std::string& name, uint32_t value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1ui(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location,  count, values);
	}

	void OpenGLShader::UploadUniformFloatArray(const std::string& name, float* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1fv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}


	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadBuffers()
	{
		for (auto& uniformBuffer : m_UniformBuffers)
		{
			int bindingIndex = uniformBuffer.GetLayout()->BindingIndex;
			//uniformBuffer->Data();
		}
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			EDNA_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& in_source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const std::string source = ReplaceIncludes(in_source);



		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			//find end of current line
			size_t eol = source.find_first_of("\r\n", pos);
			EDNA_CORE_ASSERT(eol != std::string::npos, "Syntax error!");

			// beginning of actual shader source
			size_t begin = pos + typeTokenLength + 1;

			//extract the shader type (#type ShaderType)
			std::string type = source.substr(begin, eol - begin);
			EDNA_CORE_ASSERT(ShaderTypeFromString(type), "Invalid type specified!");

			// Searches the string for the first character that does not match any of the characters specified in its arguments.
			// When pos is specified, the search only includes characters at or after position pos, ignoring any possible occurrences before that character
			// Returns The position of the first character that does not match.
			// If no such characters are found, the function returns string::npos.
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			EDNA_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");

			// find the next type token 
			pos = source.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);

		}
	

		// add includes
		for (auto& [shaderKey, shaderSource] : shaderSources)
		{
			//EDNA_CORE_TRACE(shaderKey);
			//EDNA_CORE_TRACE(shaderSource);

			//shaderSource = ReplaceIncludes(shaderSource);
			//EDNA_CORE_INFO(shaderSource);

		}
	
		
		return shaderSources;
	}

	std::string OpenGLShader::ReplaceIncludes(const std::string& source)
	{	
		// TODO: need to refactor chat gpt code?
		// Replaces #include "somefile.glsl" with the actual code
		
		// Use a regex to match #include "somefilename.txt" lines
		std::regex includeRegex(R"(\s*#include\s*"\s*(.*?)\s*")");

		// Create a match object to store the result
		std::smatch match;

		// Find all matches in the input string
		std::string result = source;
		while (std::regex_search(result, match, includeRegex)) {
			try {
				// Read the content of the file
				std::ifstream file(match[1]);
				if (file.is_open()) {
					std::ostringstream fileContent;
					fileContent << file.rdbuf();

					// Replace the entire #include line with the file content
					result = std::regex_replace(result, includeRegex, "\r\n" + fileContent.str());
					file.close();
				}
				else {
					// Handle file not found error
					EDNA_CORE_ERROR("File not found: {}" , match[1].str());
					// Move to the next match
					result = match.suffix();
				}
			}
			catch (const std::exception& e) {
				// Handle file read error
				EDNA_CORE_ERROR("Error reading file: {}", match[1].str());
				// Move to the next match
				result = match.suffix();
			}
		}

		return result;

	}



	void OpenGLShader::ExtractUniformBuffers(const std::unordered_map<GLenum, std::string> shaderSources)
	{
		std::map<int, UniformLayout> uniformLayoutMap;

		std::regex uniformRegex(R"(layout\(\s*(\w+)\s*,\s*binding\s*=\s*(\d+)\s*\)\s*uniform\s*(\w+)\s*\{([^{}]+)\})");
		std::regex dataRegex(R"(\s*([\w<>]+)\s+(\w+)\s*;)");

		for (auto& [shaderType, shaderSource] : shaderSources)
		{
			
			std::smatch matches;
			std::smatch dataMatches;

			std::string::const_iterator searchStart(shaderSource.cbegin());
			std::string::const_iterator searchEnd(shaderSource.cend());
			while (std::regex_search(searchStart, searchEnd, matches, uniformRegex))
			{
				UniformLayout layout;
				layout.Packing = PackingTypeFromString(matches[1].str());
				layout.BindingIndex = std::stoi(matches[2].str());
				layout.Name.insert(matches[3].str());

				std::string uniformContents = matches[4].str(); // Capture group of the entire block of text inside uniform struct

				std::string::const_iterator dataSearchStart(uniformContents.cbegin());
				std::string::const_iterator dataSearchEnd(uniformContents.cend());
				while (std::regex_search(dataSearchStart, dataSearchEnd, dataMatches, dataRegex)) 
				{
					UniformLayoutElement layoutElement;
					layoutElement.Type = DataTypeFromString(dataMatches[1].str());
					layoutElement.Name.insert(dataMatches[2].str());
					layout.Elements.push_back(layoutElement);
					dataSearchStart = dataMatches.suffix().first;
				}

				if (!uniformLayoutMap.contains(layout.BindingIndex))
				{
					uniformLayoutMap[layout.BindingIndex] = layout;
				}
				else
				{
					bool mergeStatus = uniformLayoutMap[layout.BindingIndex].Merge(layout);
					EDNA_CORE_ASSERT(mergeStatus, "Error merging uniform layout at index {0}", layout.BindingIndex);
				}
				searchStart = matches.suffix().first;
			}

	
			
		
		}
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> shaderSources)
	{

		GLuint program = glCreateProgram();
		// leave as vector because I might want to use geometry shaders
		std::vector<GLenum> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());


		for (auto& [shaderType, shaderSource] : shaderSources)
		{

			GLuint shader = glCreateShader(shaderType);
			const GLchar* source_c_str = shaderSource.c_str();
			glShaderSource(shader, 1, &source_c_str, 0);


			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);


				EDNA_CORE_ERROR("{0}", infoLog.data());
				EDNA_CORE_ASSERT(false, "Shader compilation error!");
				return;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			EDNA_CORE_ERROR("{0}", infoLog.data());
			EDNA_CORE_ASSERT(false, "Shader linking error!");

			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id); 
		}

		m_RendererID = program;
	}




}