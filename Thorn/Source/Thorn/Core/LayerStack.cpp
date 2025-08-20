#include "Thorn/pch.h"
#include "LayerStack.h"

namespace Thorn
{
	void LayerStack::Clear()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::Flush()
	{
		for (auto it = m_Trash.begin(); it != m_Trash.end(); it++)
		{
			auto it_l = std::find(m_Layers.begin(), m_Layers.end(), *it);
			if (it_l != m_Layers.end())
			{
				m_Layers.erase(it_l);
			}
		}

		for (auto it = m_Pending.begin(); it != m_Pending.end(); it++)
		{
			m_Layers.push_back(*it);
			m_Layers.back()->OnAttach();
		}

		m_Trash.clear();
		m_Pending.clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Pending.push_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		for (auto it = m_Layers.begin(); it != m_Layers.end(); it++)
		{
			if (*it == layer)
			{
				m_Trash.push_back(layer);
				break;
			}
		}
	}
}