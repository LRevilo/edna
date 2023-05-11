#include "ednapch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace EDNA {




	void Camera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
	{
		m_Projection = glm::perspective(fovy, aspect, zNear, zFar);
	}

	void Camera::SetOrtho(float	left, float right, float bottom, float top)
	{
		m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}
	

}