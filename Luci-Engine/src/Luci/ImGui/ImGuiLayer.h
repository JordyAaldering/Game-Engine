#pragma once

#include "Luci/Core/Layer.h"
#include "Luci/Events/ApplicationEvent.h"
#include "Luci/Events/KeyEvent.h"
#include "Luci/Events/MouseEvent.h"

namespace Luci {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	};

}
