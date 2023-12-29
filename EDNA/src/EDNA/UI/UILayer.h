#pragma once

#include "EDNA/Core/Layer.h"

#include "EDNA/Events/ApplicationEvent.h"
#include "EDNA/Events/MouseEvent.h"
#include "EDNA/Events/KeyEvent.h"

namespace EDNA {

	class EDNA_API UILayer : public Layer
	{
	public:
		UILayer();
		~UILayer();



		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(Timestep ts);
		virtual void OnImGuiRender();

		virtual void OnEvent(Event& event);
		

		void Begin();
		void End();

	private:



	private:
		float m_Time = 0.0f;
	};
 }