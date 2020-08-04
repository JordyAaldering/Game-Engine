#include "lucipch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Luci {

	struct Renderer2DData {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DData* s_Data;

	void Renderer2D::Init() {
		LUCI_PROFILE_FUNCTION();

		s_Data = new Renderer2DData();
		s_Data->QuadVertexArray = VertexArray::Create();

		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
		});
		s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[2 * 3] = {
			0, 1, 2,
			2, 3, 0,
		};

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&textureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() {
		LUCI_PROFILE_FUNCTION();
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		LUCI_PROFILE_FUNCTION();
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene() {
		LUCI_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, rotation, scale, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const glm::vec4& color) {
		LUCI_PROFILE_FUNCTION();

		s_Data->WhiteTexture->Bind();
		s_Data->TextureShader->SetFloat2("u_Tiling", glm::vec2(1.0f));
		s_Data->TextureShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			// Only calculate rotation if there is any.
			* (rotation == 0.0f ? glm::mat4(1.0f) : glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const Ref<Texture2D> texture, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, rotation, scale, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const Ref<Texture2D> texture, const glm::vec4& color) {
		LUCI_PROFILE_FUNCTION();

		texture->Bind();
		s_Data->TextureShader->SetFloat2("u_Tiling", glm::vec2(1.0f));
		s_Data->TextureShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			// Only calculate rotation if there is any.
			* (rotation == 0.0f ? glm::mat4(1.0f) : glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}
