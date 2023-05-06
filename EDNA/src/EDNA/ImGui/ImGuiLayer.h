#pragma once

#include "EDNA/Core/Layer.h"

#include "EDNA/Events/ApplicationEvent.h"
#include "EDNA/Events/MouseEvent.h"
#include "EDNA/Events/KeyEvent.h"

namespace EDNA {

	class EDNA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:



	private:
		float m_Time = 0.0f;
	};
 }