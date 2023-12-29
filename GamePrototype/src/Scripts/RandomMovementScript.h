#pragma once
#include "EDNA.h"
#include "glm/glm.hpp"

class RandomMovementScript : public EDNA::ScriptableEntity
{
public:

	int seed;
	int state;

	float uptime = 0.5f;
	float downtime = 0.0f;

	float clock = 0.f;
	float cycle = 0.f;

	bool active = true;

	glm::vec3 dir = glm::vec3{ 0.f, 0.f, 0.f };
	glm::vec3 prev_dir = glm::vec3{ 0.f, 0.f, 0.f };

	void OnCreate()
	{
		seed = std::rand();
		state = seed;

	}
	void OnDestroy()
	{
	}

	void OnUpdate(EDNA::Timestep ts)
	{
		auto& transform = GetComponent<EDNA::TransformComponent>().Transform;
		float speed = 3.f;

		clock += ts;


		if (clock > uptime + downtime) {
			clock = 0.f; 
			active = true; 
			state = rand() % 4;
			prev_dir = dir;
		}
		if (clock > uptime) {active = false;}


		if (active)
		{
			cycle = clock / uptime;

			//float value = 1.f - abs(2 * clock - 1.f);
			//float value = std::sin(3.141592653589f * cycle);
			//value *= value;
			//float value = 1.f;

			float value = cycle;

			switch (state) {
			case 0: { dir = { 1.f, 0.f, 0.f }; break; }
			case 1: { dir = { -1.f, 0.f, 0.f }; break; }
			case 2: { dir = { 0.f, 1.f, 0.f }; break; }
			case 3: { dir = { 0.f, -1.f, 0.f }; break; }
			}


			glm::vec3 active_dir = (1.f - value) * prev_dir + value * dir;

			transform[3][0] += speed  * ts * active_dir.x;
			transform[3][1] += speed  * ts * active_dir.y;

			//prev_dir = dir;
		}

	}
};