#include "Thorn/pch.h"
#include "File.h"

#include <fstream>
#include <Windows.h>
#include <ShObjIdl.h>

namespace Thorn
{
	namespace Utils
	{
		std::vector<COMDLG_FILTERSPEC> GetFilterSpec(const std::vector<DialogFilter>& filters)
		{
			std::vector<COMDLG_FILTERSPEC> specs;
			for (auto filter : filters)
			{
				switch (filter)
				{
				case DialogFilter::Project: specs.push_back({ L"Project Files (*.rebound)", L"*.rebound" }); break;
				case DialogFilter::Scene: specs.push_back({ L"Scene Files (*.rbscene)", L"*.rbscene" }); break;
				}
			}

			return specs;
		}
	}

	File::File(FS::path path)
	{
		if (!path.has_extension())
		{
			Log::Warn("Path cannot be set to a directory!");
			return;
		}

		m_Path = path;
		m_Invalid = false;
	}

	File::File(const std::string& path)
		: File(FS::path(path))
	{
	}

	File::File(const char* path)
		: File(FS::path(path))
	{
	}

	void File::Write(const char* data) const
	{
		if (m_Invalid) return;

		std::ofstream fout(m_Path);
		if (fout.is_open())
		{
			fout << data;
			fout.close();
		}
		else
		{
			Log::Error("Failed to create file!");
		}
	}

	void File::Write(const std::string& data) const
	{
		Write(data.c_str());
	}

	std::string File::ReadString() const
	{
		if (m_Invalid) return std::string();

		std::ifstream stream = _GetStream();

		std::stringstream buffer;
		buffer << stream.rdbuf();
		stream.close();
		return buffer.str();
	}

	std::vector<unsigned char> File::ReadBuffer() const
	{
		if (m_Invalid) return std::vector<unsigned char>();

		std::ifstream stream = _GetStream(true);

		std::vector<unsigned char> buffer;
		unsigned char value = 0;
		while (stream.read(reinterpret_cast<char*>(&value), sizeof(value)))
		{
			buffer.push_back(value);
		}

		stream.close();
		return buffer;
	}

	std::vector<char> File::ReadSignedBuffer() const
	{
		if (m_Invalid) return std::vector<char>();

		std::ifstream stream = _GetStream(true);

		std::vector<char> buffer;
		char value = 0;
		while (stream.read(reinterpret_cast<char*>(&value), sizeof(value)))
		{
			buffer.push_back(value);
		}

		stream.close();
		return buffer;
	}

	File File::GetFileDialog(const std::vector<DialogFilter>& filter)
	{
		FS::path path;
		HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(result))
		{
			IFileOpenDialog* pFileOpen{};
			result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
			if (SUCCEEDED(result))
			{
				result = pFileOpen->SetFileTypes((UINT)filter.size(), Utils::GetFilterSpec(filter).data());
				if (SUCCEEDED(result))
				{
					result = pFileOpen->Show(NULL);

					if (SUCCEEDED(result))
					{
						IShellItem* pItem;
						result = pFileOpen->GetResult(&pItem);
						if (SUCCEEDED(result))
						{
							PWSTR pszFilePath;
							result = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
							if (SUCCEEDED(result))
							{
								path = pszFilePath;
								CoTaskMemFree(pszFilePath);
							}
							pItem->Release();
						}
						pFileOpen->Release();
					}
					CoUninitialize();
				}
			}
		}

		return File(path);
	}

	std::ifstream File::_GetStream(bool binary) const
	{
		if (binary)
		{
			std::ifstream stream(m_Path.c_str(), std::ios::binary);
			if (!stream.is_open())
			{
				Log::Warn("Failed to open file");
			}

			return stream;
		}
		else
		{
			std::ifstream stream(m_Path.c_str());
			if (!stream.is_open())
			{
				Log::Warn("Failed to open file!");
			}

			return stream;
		}
	}
}