#include "Thorn/pch.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "Renderer.h"
#include "RenderBuffer.h"
#include "Shader.h"
#include "RenderCommands.h"
#include "Thorn/Core/Application.h"

namespace Thorn
{
	namespace Render
	{
		static const int MaxQuads = 100;
		static const int MaxVertices = MaxQuads * 4;
		static const int MaxIndices = MaxQuads * 6;
		static const int MaxTextureSlots = 32;

		struct Vertex
		{
			Vector3 Position;
			Color Color;
			Vector2 TexCoord;
			float TexIndex;
		};

		struct Data
		{
			Ref<Shader> Shader;
			Ref<RenderBuffer> Buffer;
			Vertex* StagingBuffer{ nullptr };
			Vertex* StagingBufferPtr{ nullptr };
			uint32_t IndexCount{ 0 };
			uint32_t TextureSlotIndex{ 1 };
			Ref<Image> TextureSlots[MaxTextureSlots];

			Vector4 VertexPositions[4]{};
			Vector2 TexturePositions[4]{};

			Ref<Framebuffer> SwapBuffer;

			struct CameraBuffer
			{
				Matrix4 View{ 1.0f };
				Matrix4 Projection{ 1.0f };
			} Camera;

			Ref<RenderBuffer> CameraUniform;
		};
	}

	static Render::Data s_QuadData;

	void Renderer::_Init()
	{
		RenderBufferSpecs vbSpecs{};
		vbSpecs.Size = sizeof(Render::Vertex) * Render::MaxVertices;
		vbSpecs.Usage = BufferUsage::Vertex;
		vbSpecs.Rate = BufferRate::Dynamic;
		s_QuadData.Buffer = RenderBuffer::Create(vbSpecs);

		s_QuadData.StagingBuffer = new Render::Vertex[Render::MaxVertices];

		uint32_t indices[Render::MaxIndices]{};
		uint32_t offset = 0;
		for (size_t i = 0; i < Render::MaxIndices; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		RenderBufferSpecs ibSpecs{};
		ibSpecs.Size = sizeof(indices);
		ibSpecs.Usage = BufferUsage::Index;
		ibSpecs.Rate = BufferRate::Static;
		Ref<RenderBuffer> indexBuffer = RenderBuffer::Create(ibSpecs);

		indexBuffer->SetData(indices, sizeof(indices), 0);

		s_QuadData.VertexPositions[0] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_QuadData.VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_QuadData.VertexPositions[2] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_QuadData.VertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_QuadData.TexturePositions[0] = { 1.0f, 1.0f };
		s_QuadData.TexturePositions[1] = { 1.0f, 0.0f };
		s_QuadData.TexturePositions[2] = { 0.0f, 0.0f };
		s_QuadData.TexturePositions[3] = { 0.0f, 1.0f };

		uint32_t whiteTextureData = 0xFFFFFFFF;
		ImageSpecs imageSpecs{};
		imageSpecs.Format = ImageFormat::RGBA8;
		imageSpecs.Size = { 1, 1, 1 };
		imageSpecs.Type = ImageType::e2D;
		Ref<Image> whiteTex = Image::Create(imageSpecs);

		whiteTex->SetData(&whiteTextureData, sizeof(uint32_t));
		s_QuadData.TextureSlots[0] = whiteTex;

		RenderBufferSpecs cameraSpecs{};
		cameraSpecs.Binding = 0;
		cameraSpecs.Rate = BufferRate::Dynamic;
		cameraSpecs.Size = sizeof(Render::Data::CameraBuffer);
		cameraSpecs.Usage = BufferUsage::Uniform;
		s_QuadData.CameraUniform = RenderBuffer::Create(cameraSpecs);

		ShaderSpecs pipelineSpecs{};
		pipelineSpecs.VertPath = "../Assets/Shaders/QuadShader.vert";
		pipelineSpecs.FragPath = "../Assets/Shaders/QuadShader.frag";
		pipelineSpecs.pVertexBuffer = s_QuadData.Buffer;
		pipelineSpecs.pIndexBuffer = indexBuffer;
		pipelineSpecs.pRenderBuffers = { s_QuadData.CameraUniform };
		pipelineSpecs.Attributes = { VertexAttribute::Float3, VertexAttribute::Float4, VertexAttribute::Float2, VertexAttribute::Float };
		s_QuadData.Shader = Shader::Create(pipelineSpecs);

		FramebufferSpecs sbSpecs{};
		sbSpecs.Size = { Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight() };
		s_QuadData.SwapBuffer = Framebuffer::Create(sbSpecs);

		RenderCommands::EnableDepthTesting(true);
	}

	void Renderer::_Destroy()
	{
		delete[] s_QuadData.StagingBuffer;
	}

	void Renderer::BeginFrame(const Camera& camera, const Transform& transform, bool swapBuffer)
	{
		Transform transformCopy(transform);
		transformCopy.Position.y *= -1;
		transformCopy.Position.z *= -1;

		

		s_QuadData.Camera.View = transformCopy.GetInverseMatrix();
		s_QuadData.Camera.Projection = camera.GetProjection(s_QuadData.SwapBuffer->GetSpecs().Size);
		s_QuadData.CameraUniform->SetData(&s_QuadData.Camera, sizeof(Render::Data::CameraBuffer), 0);

		if (swapBuffer)
		{
			s_QuadData.SwapBuffer->Bind();
		}
		RenderCommands::ClearColor(camera.ClearColor);

		_BeginBatch();
	}

	void Renderer::EndFrame()
	{
		_EndBatch();
		s_QuadData.SwapBuffer->Unbind();
	}

	void Renderer::DrawQuad(const Vector3& position, const Vector3& rotation, const Vector3& scale, const Color& color)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), glm::vec3(position))
			* glm::scale(Matrix4(1.0f), glm::vec3(scale))
			* glm::rotate(Matrix4(1.0f), glm::radians(rotation.x), glm::vec3(Vector3(1.0f, 0.0f, 0.0f)))
			* glm::rotate(Matrix4(1.0f), glm::radians(rotation.y), glm::vec3(Vector3(0.0f, 1.0f, 0.0f)))
			* glm::rotate(Matrix4(1.0f), glm::radians(rotation.z), glm::vec3(Vector3(0.0f, 0.0f, 1.0f)));

		DrawQuad(transform, color, 0.0f);
	}

	void Renderer::DrawQuad(const Vector3& position, const Vector3& rotation, const Vector3& scale, const Color& color, Ref<Image> texture)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), glm::vec3(position))
			* glm::scale(Matrix4(1.0f), glm::vec3(scale))
			* glm::rotate(Matrix4(1.0f), glm::radians(rotation.x), glm::vec3(Vector3(1.0f, 0.0f, 0.0f)))
			* glm::rotate(Matrix4(1.0f), glm::radians(rotation.y), glm::vec3(Vector3(0.0f, 1.0f, 0.0f)))
			* glm::rotate(Matrix4(1.0f), glm::radians(rotation.z), glm::vec3(Vector3(0.0f, 0.0f, 1.0f)));

		float texIndex = _GetTextureIndex(texture);
		DrawQuad(transform, color, texIndex);
	}

	void Renderer::DrawQuad(const Matrix4& transform, const Color& color, float texIndex)
	{
		_CheckBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_QuadData.StagingBufferPtr->Position = Vector3(transform * (glm::vec4)s_QuadData.VertexPositions[i]);
			s_QuadData.StagingBufferPtr->Color = color;
			s_QuadData.StagingBufferPtr->TexCoord = s_QuadData.TexturePositions[i];
			s_QuadData.StagingBufferPtr->TexIndex = texIndex;
			s_QuadData.StagingBufferPtr++;
		}

		s_QuadData.IndexCount += 6;
	}

	void Renderer::DrawQuad(const Matrix4& transform, const Color& color, Vector2 texCoords[4], float texIndex)
	{
		_CheckBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_QuadData.StagingBufferPtr->Position = Vector3(transform * (glm::vec4)s_QuadData.VertexPositions[i]);
			s_QuadData.StagingBufferPtr->Color = color;
			s_QuadData.StagingBufferPtr->TexCoord = texCoords[i];
			s_QuadData.StagingBufferPtr->TexIndex = texIndex;
			s_QuadData.StagingBufferPtr++;
		}

		s_QuadData.IndexCount += 6;
	}

	void Renderer::Resize(const UVector2& size)
	{
		s_QuadData.SwapBuffer->Resize(size);
		RenderCommands::Resize(size);
	}

	Ref<Framebuffer> Renderer::GetFramebuffer()
	{
		return s_QuadData.SwapBuffer;
	}

	void Renderer::_CheckBatch()
	{
		if (s_QuadData.IndexCount >= Render::MaxIndices || s_QuadData.TextureSlotIndex == Render::MaxTextureSlots)
		{
			_EndBatch();
			_BeginBatch();
		}
	}

	void Renderer::_BeginBatch()
	{
		s_QuadData.StagingBufferPtr = s_QuadData.StagingBuffer;
		s_QuadData.IndexCount = 0;
		s_QuadData.TextureSlotIndex = 1;
	}

	void Renderer::_EndBatch()
	{
		for (uint32_t i = 0; i < s_QuadData.TextureSlotIndex; i++)
		{
			s_QuadData.TextureSlots[i]->Bind(i);
		}

		size_t size = (uint8_t*)s_QuadData.StagingBufferPtr - (uint8_t*)s_QuadData.StagingBuffer;
		s_QuadData.Buffer->SetData(s_QuadData.StagingBuffer, size, 0);

		s_QuadData.Shader->Draw(s_QuadData.IndexCount);
	}

	float Renderer::_GetTextureIndex(Ref<Image> texture)
	{
		float texIndex = 0.0f;
		for (uint32_t i = 1; i < s_QuadData.TextureSlotIndex; i++)
		{
			if (*(float*)s_QuadData.TextureSlots[i]->GetRenderID() == *(float*)texture->GetRenderID())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_QuadData.TextureSlotIndex;
			s_QuadData.TextureSlots[(size_t)texIndex] = texture;
			s_QuadData.TextureSlotIndex++;
		}

		return texIndex;
	}
}