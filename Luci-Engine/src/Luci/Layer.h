#pragma once

#include "Luci/Core.h"
#include "Luci/Events/Event.h"

namespace Luci {

	class LUCI_API Layer {
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};

}
