#include "EditorLayer.h"

#include "scripts/CameraController.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Luci {

    EditorLayer::EditorLayer()
        : Layer("GameLayer"), m_CameraController(1280.0f / 720.0f) {
    }

    void EditorLayer::OnAttach() {
        LUCI_PROFILE_FUNCTION();

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>();
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        m_SecondaryCameraEntity = m_ActiveScene->CreateEntity("Secondary Camera");
        auto& cc = m_SecondaryCameraEntity.AddComponent<CameraComponent>();
        cc.Primary = false;

        m_QuadEntity = m_ActiveScene->CreateEntity("Quad");
        m_QuadEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach() {
        LUCI_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep timestep) {
        LUCI_PROFILE_FUNCTION();

        // Resize
        FramebufferSpecification spec = m_Framebuffer->GetSpecification();
        if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Update
        if (m_ViewportFocused) {
            m_CameraController.OnUpdate(timestep);
        }

        // Render
        Renderer2D::ResetStatistics();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        // Update scene
        m_ActiveScene->OnUpdate(timestep);

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender() {
        LUCI_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen) {
            ImGui::PopStyleVar(2);
        }

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        m_SceneHierarchyPanel.OnImGuiRender();

        if (ImGui::Begin("Renderer 2D")) {
            auto stats = Renderer2D::GetStatistics();
            ImGui::Text("> Statistics");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetVertexCount());
            ImGui::Text("Indices: %d", stats.GetIndexCount());

            if (m_QuadEntity) {
                ImGui::Separator();
                auto& tag = m_QuadEntity.GetComponent<TagComponent>().Tag;
                ImGui::Text("Tag: %s", tag.c_str());

                auto& color = m_QuadEntity.GetComponent<SpriteRendererComponent>().Color;
                ImGui::ColorEdit4("Quad color", glm::value_ptr(color));
                ImGui::Separator();
            }

            static bool usePrimary = true;
            if (ImGui::Checkbox("Camera A", &usePrimary)) {
                m_CameraEntity.GetComponent<CameraComponent>().Primary = usePrimary;
                m_SecondaryCameraEntity.GetComponent<CameraComponent>().Primary = !usePrimary;
            }

            ImGui::End();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::Begin("Viewport")) {
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::End();
        }
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event) {
        m_CameraController.OnEvent(event);
    }

}
