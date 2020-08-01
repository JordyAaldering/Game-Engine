#pragma once

#include "Luci.h"

class Sandbox2D : public Luci::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Luci::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Luci::Event& event) override;

private:
	Luci::OrthographicCameraController m_CameraController;

	Luci::Ref<Luci::VertexArray> m_VertexArray;
	Luci::Ref<Luci::Shader> m_Shader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
