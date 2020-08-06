#pragma once

#include <Luci.h>

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

		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_ErrorTexture;
		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;

	};

}
