#pragma once

#ifdef THORN_ENTRY_POINT
extern Thorn::Application* Thorn::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Thorn::CreateApplication();
	app->Run();
	delete app;
}
#endif