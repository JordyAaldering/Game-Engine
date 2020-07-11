#pragma once

#include "Core.h"
#include "Window.h"
#include "Luci/LayerStack.h"
#include "Luci/Events/Event.h"
#include "Luci/Events/ApplicationEvent.h"

namespace Luci {

	class LUCI_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);
	};

	// to be defined in client
	Application* CreateApplication();

}
