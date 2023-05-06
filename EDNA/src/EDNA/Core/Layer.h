#pragma once

#include "EDNA/Core/Core.h"
#include "EDNA/Core/Timestep.h"
#include "EDNA/Events/Event.h"

namespace EDNA {

	class EDNA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
