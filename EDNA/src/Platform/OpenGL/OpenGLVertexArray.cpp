#include "ednapch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace EDNA {

	static GLenum ShaderDataTypeToOpenGLBase(DataType type)
	{
		switch (type)
		{
		case DataType::Float:			return GL_FLOAT;
		case DataType::Float2:		return GL_FLOAT;
		case DataType::Float3:		return GL_FLOAT;
		case DataType::Float4:		return GL_FLOAT;

		case DataType::Mat2:			return GL_FLOAT;
		case DataType::Mat3:			return GL_FLOAT;
		case DataType::Mat4:			return GL_FLOAT;

		case DataType::Int:			return GL_INT;
		case DataType::Int2:			return GL_INT;
		case DataType::Int3:			return GL_INT;
		case DataType::Int4:			return GL_INT;

		case DataType::Bool:			return GL_BOOL;
		}
		EDNA_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID); 
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		EDNA_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();


		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBase(element.Type),
				element.Normalised ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}