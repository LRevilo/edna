#pragma once
#include <unordered_set>
namespace EDNA {

	enum class DataType
	{
		None = 0, Bool, Float, Float2, Float3, Float4, Mat2, Mat3, Mat4, Int, Int2, Int3, Int4
	};

	enum class PackingType
	{
		None = 0, std140, std430, shared, packed
	};

	static uint32_t ShaderDataTypeSize(DataType type) 
	{
		switch (type)
		{
			case DataType::Float:			return 4;
			case DataType::Float2:		return 4*2;
			case DataType::Float3:		return 4*3;
			case DataType::Float4:		return 4*4;

			case DataType::Mat2:			return 4*2*2;
			case DataType::Mat3:			return 4*3*3;
			case DataType::Mat4:			return 4*4*4;

			case DataType::Int:			return 4;
			case DataType::Int2:			return 4*2;
			case DataType::Int3:			return 4*3;
			case DataType::Int4:			return 4*3;

			case DataType::Bool:			return 1;

		}
		EDNA_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	static PackingType PackingTypeFromString(const std::string& type)
	{
		if (type == "std140")		 return PackingType::std140;
		else if (type == "std430")   return PackingType::std430;
		else if (type == "packed")   return PackingType::packed;
		else if (type == "shared")   return PackingType::shared;

		EDNA_CORE_ASSERT(false, "Unknown packing type!");
		return PackingType::None;
	}

	struct BufferElement
	{
		std::string Name;
		DataType Type;
		uint32_t Size;
		uint32_t Offset;

		bool Normalised;

		BufferElement() = default;
		BufferElement(DataType type, const std::string& name, bool normalised = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalised(normalised)
		{	
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case DataType::Float:			return 1;
			case DataType::Float2:		return 2;
			case DataType::Float3:		return 3;
			case DataType::Float4:		return 4;

			case DataType::Mat2:			return 2 * 2;
			case DataType::Mat3:			return 3 * 3;
			case DataType::Mat4:			return 4 * 4;

			case DataType::Int:			return 1;
			case DataType::Int2:			return 2;
			case DataType::Int3:			return 3;
			case DataType::Int4:			return 3;

			case DataType::Bool:			return 1;
			}
			EDNA_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

	};



	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements) 
		{
			CalculateOffestAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return  m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return  m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }


	private:
		void CalculateOffestAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;

			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

	};

	class IndexBuffer 
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	};


	struct UniformLayoutElement
	{
		std::unordered_set<std::string> Name;
		DataType Type;
	};


	struct UniformLayout {
		UniformLayout() = default;

	    int BindingIndex;
		PackingType Packing;
		std::unordered_set<std::string> Name;
	    std::vector<UniformLayoutElement> Elements;

		bool Merge(const UniformLayout& other)
		{
			if (BindingIndex != other.BindingIndex) return false;
			if (Packing != other.Packing) return false;
			if (Elements.size() != other.Elements.size()) return false;

			// iterate through each layout element and check that the data types match, then merge the element names
			for (size_t i = 0; i < Elements.size(); i++) 
			{
				if (Elements[i].Type != other.Elements[i].Type) return false;
				Elements[i].Name.insert(other.Elements[i].Name.begin(), other.Elements[i].Name.end());
			}
			// merge layout name
			Name.insert(other.Name.begin(), other.Name.end());
			return true;
		}
	};
	
	class UniformBuffer 
	{
	public:
		~UniformBuffer() = default;
		UniformBuffer(const UniformLayout& layout);

	private:
		void SetupLayout(const UniformLayout& layout);
		UniformLayout m_Layout;
		std::vector<float> m_Data;
	};
}

