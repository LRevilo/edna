#include "ednapch.h"
#include "Entity.h"

namespace EDNA {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}

	Entity::~Entity()
	{

	}


}