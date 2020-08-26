#pragma once

#include "Luci.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Luci {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		OrthographicCameraController m_CameraController;

		Ref<Framebuffer> m_Framebuffer;
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;

		Ref<Scene> m_ActiveScene;
		Entity m_CameraEntity, m_SecondaryCameraEntity;
		Entity m_QuadEntity;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}
