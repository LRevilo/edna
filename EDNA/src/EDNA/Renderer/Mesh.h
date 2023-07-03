#pragma once
#include "Buffer.h"
#include "Material.h"
#include <glm/glm.hpp>
namespace EDNA {
	
	class Mesh 
	{

		public:
		Ref<VertexBuffer> Vertices;
		Ref<IndexBuffer> Indices;
		glm::mat4 Transform = glm::mat4(1.0f);

		private:
	
	
	};

}