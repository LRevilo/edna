#include "ednapch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include "glm/gtc/matrix_transform.hpp"


namespace EDNA {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
	Ref<ShaderLibrary> Renderer::s_ShaderLibrary = CreateRef<ShaderLibrary>();

	struct RendererData
	{
		static const uint32_t MaxQuads = 8192;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; 

		int ShaderCount;
		int MaterialCount;
		std::vector<Ref<VertexArray>> VAs;
		std::vector<Ref<VertexBuffer>> VBs; //straya
		std::vector<Ref<IndexBuffer>> IBs;
	};
	static RendererData s_RendererData;

	struct RendererShadowData
	{



		static const uint32_t MaxQuads = 8192;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> VA;
		Ref<VertexBuffer> VB;
		Ref<IndexBuffer> IB;

	};
	static RendererShadowData s_ShadowData;



	void Renderer::Init()
	{
		EDNA_PROFILE_FUNCTION();


		RenderCommand::Init();
		Renderer2D::Init();

		EDNA_CORE_INFO("Load shader");
		Shaders()->Load("3DShader", "../EDNA/src/EDNA/Shaders/3DShader.glsl");
		// Shaders()->Load("Texture", "../EDNA/src/EDNA/Shaders/Texture.glsl");






		// Init shadow data
		s_ShadowData.VA = VertexArray::Create();
		s_ShadowData.VB = VertexBuffer::Create(s_ShadowData.MaxVertices * sizeof(glm::vec3));
		s_ShadowData.VB->SetLayout({ {DataType::Float3, "a_Position"} });
		s_ShadowData.IB = IndexBuffer::Create(s_ShadowData.MaxIndices);

		s_ShadowData.VA->AddVertexBuffer(s_ShadowData.VB);

		Shaders()->Load("Shadow", "../EDNA/src/EDNA/Shaders/Shadow.glsl");
		s_SceneData->DepthTexture = Texture2D::Create(1024, 1024);



		s_RendererData.ShaderCount = Shaders()->Count();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0,0, width, height);
	}

	void Renderer::ClearDepth()
	{
		RenderCommand::ClearDepth();
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& cameraTransform, const glm::mat4& lightProjection, const glm::mat4& lightTransform)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(cameraTransform);
		s_SceneData->CameraPosition = glm::vec3(cameraTransform[3][0], cameraTransform[3][1], cameraTransform[3][2]);

		auto LightView = glm::lookAt(
			glm::vec3(s_SceneData->CameraPosition.x + 3.f, s_SceneData->CameraPosition.y + 10.f, s_SceneData->CameraPosition.z + 10.f),
			glm::vec3(s_SceneData->CameraPosition.x, s_SceneData->CameraPosition.y, 0.f),
			glm::vec3(0.f, 0.f, 1.f));
		auto LightTransform = glm::inverse(LightView);

		s_SceneData->LightSpaceMatrix = lightProjection * LightView;
		s_SceneData->LightPosition = glm::vec3(LightTransform[3][0], LightTransform[3][1], LightTransform[3][2]);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::BeginShadowPass()
	{
		auto shader = Shaders()->Get("Shadow");
		shader->Bind();
		shader->SetMat4("u_LightSpace", s_SceneData->LightSpaceMatrix);

	}

	void Renderer::EndShadowPass()
	{
	}

	void Renderer::DrawMesh(const Mesh& mesh, const glm::mat4& transform)
	{
		//EDNA_CORE_INFO("DRAW THE MESH");
		Ref<VertexArray> VA = VertexArray::Create();
		Ref<VertexBuffer> VB = VertexBuffer::Create(mesh);


		VB->SetLayout(mesh.m_Layout);

		Ref<IndexBuffer> IB = IndexBuffer::Create(mesh);



		VA->AddVertexBuffer(VB);
		VA->SetIndexBuffer(IB);



		//EDNA_CORE_INFO("Submit");
		Submit(mesh.m_Material, VA, transform);
	}

	void Renderer::DrawMeshShadow(const Mesh& mesh, const glm::mat4& transform)
	{
		//EDNA_CORE_INFO("DRAW THE MESH");
		Ref<VertexArray> VA = VertexArray::Create();
		Ref<VertexBuffer> VB = VertexBuffer::Create(mesh);
		VB->SetLayout(mesh.m_Layout);
		Ref<IndexBuffer> IB = IndexBuffer::Create(mesh);
		VA->AddVertexBuffer(VB);
		VA->SetIndexBuffer(IB);

		//EDNA_CORE_INFO("Submit");
		//Submit(mesh.m_Material, VA, transform);
		auto shader = Shaders()->Get("Shadow");
		//shader->Bind();
		//shader->SetMat4("u_LightSpace", s_SceneData->LightSpaceMatrix);
		shader->SetMat4("u_Transform", transform);

		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);


		RenderCommand::DrawIndexed(VA);
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		//EDNA_CORE_INFO("get shader");
		auto shader = Renderer::s_ShaderLibrary->Get(material->m_ShaderName);

		//EDNA_CORE_INFO("got shader {0}" , material->m_ShaderName);


		//EDNA_CORE_INFO("bind shader");

		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetFloat3("u_CameraPosition", s_SceneData->CameraPosition);

		shader->SetMat4("u_LightSpace", s_SceneData->LightSpaceMatrix);
		shader->SetFloat3("u_LightPosition", s_SceneData->LightPosition);

		shader->SetMat4("u_Transform", transform);

		material->UploadBuffers();
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		//EDNA_CORE_INFO("draw indexed");

		RenderCommand::BindDepthMap(s_SceneData->depthTextureID);

		RenderCommand::DrawIndexed(vertexArray);


	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		

		RenderCommand::DrawIndexed(vertexArray);
	}

}
