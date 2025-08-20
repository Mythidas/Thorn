#include "Thorn/Engine.h"

class MainLayer : public Thorn::Layer
{
public:
	Thorn::Camera camera{};
	Thorn::Transform transform{};

	void OnAttach()
	{
		camera.ClearColor = { 1.0f, 0.7f, 0.4f, 1.0f };
		transform.Position.z = 3.0f;
	}

	void OnUpdate()
	{
		Thorn::Renderer::BeginFrame(camera, transform);
		Thorn::Renderer::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		Thorn::Renderer::EndFrame();
	}
};

class Sandbox : public Thorn::Application
{
public:
	Sandbox()
	{
		PushLayer(new MainLayer());
	}
};

Thorn::Application* Thorn::CreateApplication()
{
	return new Sandbox();
}