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

		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetOrtho(float	left, float right, float bottom, float top);

	private:

		glm::mat4 m_Projection = glm::mat4(1.0f);


	};

}

