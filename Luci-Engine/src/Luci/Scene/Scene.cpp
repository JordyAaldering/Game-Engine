#include "lucipch.h"

#include "Luci/Scene/Scene.h"
#include "Luci/Scene/Entity.h"
#include "Luci/Scene/Components.h"
#include "Luci/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Luci {

	Scene::Scene() {}

	Scene::~Scene() {}

	Entity Scene::CreateEntity(const std::string& tag) {
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::OnUpdate(Timestep timestep) {
		{ LUCI_PROFILE_SCOPE("Scene::OnUpdate Update Scripts");
			m_Registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc) {
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity(entity, this);
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(timestep);
			});
		}

		{ LUCI_PROFILE_SCOPE("Scene::OnUpdate Render 2D");
			Camera* mainCamera = nullptr;
			glm::mat4 cameraTransform;

			auto cameraGroup = m_Registry.group<CameraComponent, TransformComponent>();
			for (auto entity : cameraGroup) {
				auto [camera, transform] = cameraGroup.get<CameraComponent, TransformComponent>(entity);
				if (camera.Primary) {
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}

			if (mainCamera) {
				Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

				auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				}

				Renderer2D::EndScene();
			}
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto cameraView = m_Registry.view<CameraComponent>();
		for (auto entity : cameraView) {
			auto& cameraComponent = cameraView.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

}
