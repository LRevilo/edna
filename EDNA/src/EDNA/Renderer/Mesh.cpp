#include "ednapch.h"
#include "Mesh.h"
#include "EDNA/Generators/Generator.h"

namespace EDNA {

	Mesh::Mesh()
	{
	}


	Mesh::Mesh(const Ref<Material>& material) : m_Material(material)
	{
	}



    void Mesh::CubeMesh(glm::vec4 color)
    {
        

        for (int n = 0; n < cube_vertices.size(); n++)
        {
            Vertex vertex;
            vertex.Position = cube_vertices[n] - glm::vec3(0.5f);// +offset;
            vertex.Position *= 0.5;
            vertex.Normal = cube_normals[n];
            vertex.UV = cube_uvs[n];
            vertex.Color = color;
            m_Vertices.push_back(vertex);
        }
        for (int i = 0; i < cube_indicies.size(); i++)
        {
            m_Indices.push_back(cube_indicies[i]);
        }
    }


    void Mesh::PlaneMesh(float scale)
    {

        for (int n = 0; n < 4; n++)
        {
            Vertex vertex;
            vertex.Position = (2.f*cube_vertices[n]-1.f);// +offset;
            vertex.Position.x *= scale;
            vertex.Position.y *= scale;
            vertex.Normal = -cube_normals[n];
            vertex.UV = cube_uvs[n];
            vertex.Color = glm::vec4(0.15f, 0.5f, 0.08f, 1.0f);
            m_Vertices.push_back(vertex);
        }
        m_Indices.push_back(0);
        m_Indices.push_back(2);
        m_Indices.push_back(1);
        m_Indices.push_back(1);
        m_Indices.push_back(2);
        m_Indices.push_back(3);
    }

    void Mesh::PlaneMesh(float scale, int sidePoints)
    {
        float ds = 1.f / (sidePoints-1.f);
        for (int y = 0; y < sidePoints; y++)
        {
            for (int x = 0; x < sidePoints; x++)
            {
                float dx = 2.f * x * ds - 1.f;
                float dy = 2.f * y * ds - 1.f;
                Vertex vertex;
                vertex.Position.z = 0.f;
                vertex.Position.x = 0.5*scale * dx;
                vertex.Position.y = 0.5*scale * dy;
                vertex.Normal = glm::vec3(0.f, 0.f, 1.f);
                vertex.UV = glm::vec2(x * ds, y * ds);
                vertex.Color = glm::vec4(Noise::ValueNoise(scale * dx, scale * dy, 1),
                                         Noise::ValueNoise(0.5 * scale * dx, scale * dy, -1),
                                         Noise::ValueNoise(0.166 * scale * dx, scale * dy, 5),
                                         Noise::ValueNoise(0.0125 * scale * dx, scale * dy, -61));
                  
                
                
                
                
                //floor(8.f * glm::vec4(x * ds, y * ds, 0.f, 1.0f)) / 8.f;
                m_Vertices.push_back(vertex);
            }
        }
        for (int y = 0; y < sidePoints-1; y++)
        {
            for (int x = 0; x < sidePoints-1; x++)
            {

                int pos = x + y * (sidePoints);
                m_Indices.push_back(x + y * (sidePoints));
                m_Indices.push_back((x+1) + y * (sidePoints));
                m_Indices.push_back((x + 1) + (y+1) * (sidePoints));
                m_Indices.push_back(x + y * (sidePoints));
                m_Indices.push_back((x + 1) + (y + 1) * (sidePoints));
                m_Indices.push_back((x) + (y + 1) * (sidePoints));

                //m_Indices.push_back(0);
                //m_Indices.push_back(2);
                //m_Indices.push_back(1);
                //m_Indices.push_back(1);
                //m_Indices.push_back(2);
                //m_Indices.push_back(3);
            }
        }
    }

    void Mesh::FloorPlaneMesh(glm::vec2 offset, float scale = 8.0, int sidePoints = 8)
    {

        float ds = 1.f / (sidePoints - 1.f);
        for (int y = 0; y < sidePoints; y++)
        {
            for (int x = 0; x < sidePoints; x++)
            {
                float dx = 2.f * x * ds - 1.f;
                float dy = 2.f * y * ds - 1.f;
                Vertex vertex;
                vertex.Position.z = 0.f;
                vertex.Position.x = 0.5 * scale * dx;
                vertex.Position.y = 0.5 * scale * dy;
                vertex.Normal = glm::vec3(0.f, 0.f, 1.f);
                vertex.UV = glm::vec2(x * ds, y * ds);  

                float cx = (dx / 2.f + offset.x);
                float cy = (dy / 2.f + offset.y);

                float n0 = Noise::ValueNoise(32.0 * cx, 32.0 * cy, 1);
                float n1 = Noise::ValueNoise(4.0 * cx, 4.0 * cy, -1);
                float n2 = Noise::ValueNoise(1.0 * cx, 1.0 * cy, 5);
                float n3 = Noise::ValueNoise(0.125 * cx, 0.2 * cy, -61);
                vertex.Color = glm::vec4(n0, n0, n0, 1.0);
                                         
                                         
                                         
                    




                //floor(8.f * glm::vec4(x * ds, y * ds, 0.f, 1.0f)) / 8.f;
                m_Vertices.push_back(vertex);
            }
        }
        for (int y = 0; y < sidePoints - 1; y++)
        {
            for (int x = 0; x < sidePoints - 1; x++)
            {

                int pos = x + y * (sidePoints);
                m_Indices.push_back(x + y * (sidePoints));
                m_Indices.push_back((x + 1) + (y + 1) * (sidePoints));
                m_Indices.push_back((x)+(y + 1) * (sidePoints));

                m_Indices.push_back((x + 1) + (y + 1) * (sidePoints));
                m_Indices.push_back((x + 1) + y * (sidePoints));
                m_Indices.push_back(x + y * (sidePoints));



                //m_Indices.push_back(0);
                //m_Indices.push_back(2);
                //m_Indices.push_back(1);
                //m_Indices.push_back(1);
                //m_Indices.push_back(2);
                //m_Indices.push_back(3);
            }
        }
    }

}