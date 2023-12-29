#pragma once
#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "EDNA/Renderer/Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Texture.h"

namespace EDNA {


	class Renderer
	{
	public:

		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void ClearDepth();

		static void BeginScene(const Camera& camera, const glm::mat4& cameraTransform, const glm::mat4& lightProjection, const glm::mat4& lightTransform);
		static void EndScene();

		static void BeginShadowPass();
		static void EndShadowPass();

		static void DrawMesh(const Mesh& Mesh, const glm::mat4& transform);
		static void DrawMeshShadow(const Mesh& Mesh, const glm::mat4& transform);

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));


		inline static Ref<ShaderLibrary> Shaders() { return s_ShaderLibrary; }
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static Ref<ShaderLibrary> s_ShaderLibrary;

	public:

		// Data which is modified by the scene and used for renderering
		struct SceneData
		{
			glm::vec3 CameraPosition;
			glm::mat4 ViewProjectionMatrix;

			glm::vec3 LightPosition;
			glm::mat4 LightSpaceMatrix;

			uint64_t depthTextureID;
			Ref<Texture2D> DepthTexture;

		};

		static Scope<SceneData> s_SceneData;


		
		//friend class Renderer2D;

	private: 		

	};

}
