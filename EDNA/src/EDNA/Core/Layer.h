 #pragma once

#include "EDNA/Core/Core.h"
#include "EDNA/Core/Timestep.h"
#include "EDNA/Events/Event.h"

#include "imgui.h"
#include "imgui_internal.h"


namespace EDNA {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");

		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnUIRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
