#pragma once
#include "EDNA.h"

class CameraController : public EDNA::ScriptableEntity
{
public:
	void OnCreate()
	{

	}
	void OnDestroy()
	{

	}




	void OnUpdate(EDNA::Timestep ts)
	{
		auto& transform = GetComponent<EDNA::TransformComponent>().Transform;
		float speed = 5.f;

		if (EDNA::Input::IsKeyPressed(EDNA_KEY_A))
			transform[3][0] -= speed * ts;

		if (EDNA::Input::IsKeyPressed(EDNA_KEY_D))
			transform[3][0] += speed * ts;

		if (EDNA::Input::IsKeyPressed(EDNA_KEY_W))
			transform[3][1] += speed * ts;

		if (EDNA::Input::IsKeyPressed(EDNA_KEY_S))
			transform[3][1] -= speed * ts;

	}
};