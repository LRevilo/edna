#pragma once
#include "entt.hpp"
#include "EDNA/Core/Timestep.h"
#include "EDNA/Core/UUID.h"
#include "EDNA/Renderer/FrameBuffer.h"
#include "EDNA/Renderer/Camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace EDNA {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);


		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);



		void UpdateInputs();
		void UpdateScripts(Timestep ts);
		void UpdateCamera();
	
		void RaycastMouse();
		void RenderScene();


		const glm::vec3 GetRayDirection() { return m_RayDirection; }
		Ref<Framebuffer> GetShadowBuffer() { return m_ShadowFramebuffer; }

	private:

		Camera* m_SceneCamera = nullptr;
		glm::mat4* m_SceneCameraTransform = nullptr;

		glm::vec3 m_RayDirection = glm::vec3(0.f, 0.f, -1.f);

		entt::registry m_Registry;


		Ref<Framebuffer> m_ShadowFramebuffer;
		int m_ShadowWidth = 2048;
		int m_ShadowHeight = 2048;
		glm::mat4 m_LightTransform;
		glm::mat4 m_LightProjection;


		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
	};
}



