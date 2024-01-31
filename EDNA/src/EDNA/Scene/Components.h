#pragma once
#include <glm/glm.hpp>
#include "EDNA/Renderer/Camera.h" 
#include "EDNA/Core/UUID.h"
#include "EDNA/Renderer/Mesh.h"
#include "EDNA/Renderer/Texture.h"
//#include "ScriptableEntity.h"
namespace EDNA {

	struct IDComponent 
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID& uuid)
			: ID(uuid) {}

		const char* Name() { return "ID"; }
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		const char* Name() { return "Tag"; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }

		const char* Name() { return "Transform"; }
	};

	class ScriptableEntity;
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		ScriptableEntity* (*InstantiateScript)();

		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() {return static_cast<ScriptableEntity*>( new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };

		}
		const char* Name() { return "NativeScript"; }

	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color { 1.0f };
		EDNA::Ref<Texture2D> Texture = nullptr;
		bool ScreenSpace = false;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
		SpriteRendererComponent(const Ref<Texture2D> &texture, const glm::vec4& color)
			: Texture(texture), Color(color) {}

		const char* Name() { return "SpriteRenderer"; }
	};

	struct CameraComponent
	{
		EDNA::Camera Camera{glm::mat4(1.0f)};
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}

		const char* Name() { return "Camera"; }
	};

	struct CameraAttachmentComponent
	{

		bool Active = true;
		float SmoothingFactor = 0.0f;
		glm::vec3 Offset = { 0.f, 0.5f, 1.f };
		glm::vec3 Direction = { 0.f, 0.5f, -1.f };

		CameraAttachmentComponent() = default;
		CameraAttachmentComponent(const CameraAttachmentComponent&) = default;
		CameraAttachmentComponent(const float& smoothing, const glm::vec3& offset)
			: SmoothingFactor(smoothing), Offset(offset)  {}

		const char* Name() { return "CameraAttachment"; }
	};


	struct RenderableComponent
	{
		bool Visible = true;
		RenderableComponent() = default;
		RenderableComponent(const RenderableComponent&) = default;
		RenderableComponent(const bool& visible)
			: Visible(visible) {}

		const char* Name() { return "Renderable"; }
	};

	struct ShadowCasterComponent
	{
		bool Visible = true;
		ShadowCasterComponent() = default;
		ShadowCasterComponent(const ShadowCasterComponent&) = default;
		ShadowCasterComponent(const bool& visible)
			: Visible(visible) {}

		const char* Name() { return "ShadowCaster"; }
	};

	struct MeshComponent
	{
		EDNA::Mesh MeshData;
		bool Dirty = false;
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Mesh mesh)
			: MeshData(mesh) {}
	
		const char* Name() { return "Mesh"; }
	};

	struct PlayerInputComponent
	{
		bool Active = false;

		bool Up = false;
		bool Down = false;
		bool Left = false;
		bool Right = false;
		
		void Reset()
		{
			Up = false;
			Down = false;
			Left = false;
			Right = false;
		}


		PlayerInputComponent() = default;
		PlayerInputComponent(const PlayerInputComponent&) = default;

		const char* Name() { return "PlayerInput"; }
	};

}