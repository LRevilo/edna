#include "ednapch.h"
#include "Scene.h"
 
#include "Components.h"
#include "EDNA/Renderer/Renderer2D.h"

#include <glm/glm.hpp>
#include "Entity.h"


namespace EDNA {




	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{

		//update scripts

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});




		// update camera
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : cameraView)
		{
			auto [transformComponent, cameraComponent] = cameraView.get<TransformComponent, CameraComponent>(entity);
			if (cameraComponent.Primary)
			{
				mainCamera = &cameraComponent.Camera;
				cameraTransform = &transformComponent.Transform;
				break;
			}
		}
		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto spriteGroup = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : spriteGroup)
			{
				auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}




	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		float aspect = (float)width / (float)height;

		// Resize non fixed aspect ration cameras
		auto cameraView = m_Registry.view<CameraComponent>();
		for (auto entity : cameraView)
		{
			auto& cameraComponent = cameraView.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetPerspective(90.f, aspect, 1.0f, 10.f);

			}
		}

	}

}