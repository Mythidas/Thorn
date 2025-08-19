#pragma once

#include <string>

namespace Thorn
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			: m_DebugName(name)
		{
		}
		virtual ~Layer() = default;

		const std::string& GetName() const { return m_DebugName; }

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnFixedUpdate() {}
		virtual void OnRenderUI() {}

	protected:
		std::string m_DebugName;
	};
}