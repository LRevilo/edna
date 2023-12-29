#pragma once
#include <glm/glm.hpp>

namespace EDNA {

	class Camera
	{

	public:
		Camera(const glm::mat4& projection)
			: m_Projection(projection)  {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
		const float& GetFovY() const { return m_FovY; }
		const float& GetAspect() const { return m_Aspect; }
		const float& GetzNear() const { return m_zNear; }
		const float& GetzFar() const { return m_zFar; }

		void SetAspectRatio(float aspect);
		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetOrtho(float	left, float right, float bottom, float top);

	private:

		glm::mat4 m_Projection = glm::mat4(1.0f);

		float m_FovY = 120.f;
		float m_Aspect = 16.f/9.f;
		float m_zNear = 1.0f;
		float m_zFar = 40.f;

		float m_Left = 0.f;
		float m_Right = 0.f;
		float m_Bottom = 0.f;
		float m_Top = 0.f;


	};

}

