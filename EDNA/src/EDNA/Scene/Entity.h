#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace EDNA {
	class Entity
	{
	public:

		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;
		~Entity();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}


		template<typename T>
		void Destroy()
		{
			m_Scene->m_Registry.destroy(m_EntityHandle);
		}



		operator bool() const { return m_EntityHandle != entt::null; }
		entt::entity GetHandle() { return m_EntityHandle; }

	private:

		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;

	};
}

