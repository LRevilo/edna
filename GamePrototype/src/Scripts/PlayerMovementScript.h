#pragma once
#include "EDNA.h"

class PlayerMovementScript : public EDNA::ScriptableEntity
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

		auto& input = GetComponent<EDNA::PlayerInputComponent>();
		float speed = 5.f;

		int x = input.Right - input.Left;
		int y = input.Up - input.Down;



		transform[3][0] += speed * x * ts;
		transform[3][1] += speed * y * ts;


	}
};