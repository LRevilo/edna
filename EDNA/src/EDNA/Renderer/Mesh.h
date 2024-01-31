#pragma once
#include "EDNA/Renderer/Buffer.h"
#include "EDNA/Renderer/Material.h"
#include <glm/glm.hpp>

namespace EDNA {
	
#define MAX_BONE_INFLUENCE 4

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
		//glm::vec3 Tangent;
		//glm::vec3 Bitangent;
		glm::vec4 Color;

		//int m_BoneIDs[MAX_BONE_INFLUENCE];
		//float m_Weights[MAX_BONE_INFLUENCE];
	};

	class Mesh 
	{

	public:

        Mesh();
        Mesh(const Ref<Material>& material);


		BufferLayout m_Layout = BufferLayout({
							{ DataType::Float3, "a_Position"},
							{ DataType::Float3, "a_Normal"},
							{ DataType::Float2, "a_TexCoord"},
							{ DataType::Float4, "a_Color"}
							});
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		glm::mat4 m_Transform = glm::mat4(1.0f);
        Ref<Material> m_Material;



        inline static const std::vector<glm::vec3> cube_vertices = {
            //bottom face
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(1.f, 1.f, 0.f),

            //top face
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(1.f, 0.f, 1.f),
            glm::vec3(0.f, 1.f, 1.f),
            glm::vec3(1.f, 1.f, 1.f),

            //facing -y
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(1.f, 0.f, 1.f),

            //facing +y
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(1.f, 1.f, 0.f),
            glm::vec3(0.f, 1.f, 1.f),
            glm::vec3(1.f, 1.f, 1.f),

            //facing -x
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.f, 1.f, 1.f),

            //facing +x
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(1.f, 1.f, 0.f),
            glm::vec3(1.f, 0.f, 1.f),
            glm::vec3(1.f, 1.f, 1.f),


        };
        inline static const std::vector<glm::vec3> cube_normals = {
            //bottom face
            glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(0.f, 0.f, -1.f),
            //top face
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.f, 0.f, 1.f),

            //facing -y
            glm::vec3(0.f, -1.f, 0.f),
            glm::vec3(0.f, -1.f, 0.f),
            glm::vec3(0.f, -1.f, 0.f),
            glm::vec3(0.f, -1.f, 0.f),

            //facing +y
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f),

            //facing -x
            glm::vec3(-1.f, 0.f, 0.f),
            glm::vec3(-1.f, 0.f, 0.f),
            glm::vec3(-1.f, 0.f, 0.f),
            glm::vec3(-1.f, 0.f, 0.f),

            //facing +x
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(1.f, 0.f, 0.f)
        };
        inline static const std::vector<glm::vec2> cube_uvs = {
            //bottom face
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            //top face
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),

            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),

            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),

            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),

            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
        };
        inline static const std::vector<uint32_t> cube_indicies = {
            0,1,2,2,1,3,
            4,5,6,6,5,7,
            8,9,10,10,9,11,
            12,13,14,14,13,15,
            16,17,18,18,17,19,
            20,21,22,22,21,23
        };



        void CubeMesh(glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f));
        void PlaneMesh(float scale);
        void PlaneMesh(float scale, int sidePoints);
        void FloorPlaneMesh(glm::vec2 offset, float scale, int sidePoints);





	};




}