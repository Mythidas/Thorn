#include "Thorn/pch.h"
#include "Image.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Thorn
{
	namespace Utils
	{
		static GLenum TypeToGL(ImageType type)
		{
			switch (type)
			{
			case ImageType::e1D: return GL_TEXTURE_1D;
			case ImageType::e2D: return GL_TEXTURE_2D;
			case ImageType::e3D: return GL_TEXTURE_3D;
			default: return GL_TEXTURE_2D;
			}
		}

		static GLenum WrapToGL(ImageWrap wrap)
		{
			switch (wrap)
			{
			case ImageWrap::Repeat: return GL_REPEAT;
			case ImageWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
			case ImageWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
			}
		}

		static GLenum TilingToGL(ImageTiling tiling)
		{
			switch (tiling)
			{
			case ImageTiling::Linear: return GL_LINEAR;
			case ImageTiling::Nearest: return GL_NEAREST;
			}
		}

		static GLenum FormatToGL(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::R8: return GL_RED;
			case ImageFormat::RGBA8: return GL_RGBA;
			case ImageFormat::RGBA32F: return GL_RGBA;
			}
		}

		static GLenum FormatToGLInternal(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::R8: return GL_R8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			case ImageFormat::RGBA32F: return GL_RGBA32F;
			}
		}

		static uint32_t FormatSize(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::R8: return 1;
			case ImageFormat::RGBA8: return 4;
			case ImageFormat::RGBA32F: return 4 * 4;
			}

			return 0;
		}

		static void SetGLImage(const ImageSpecs& specs)
		{
			if (specs.Type == ImageType::e1D)
			{
				glTexImage1D(GL_TEXTURE_1D, 0, FormatToGL(specs.Format),
					specs.Size.x, 0, FormatToGL(specs.Format), GL_UNSIGNED_BYTE, nullptr);
			}
			else if (specs.Type == ImageType::e2D)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, FormatToGL(specs.Format),
					specs.Size.x, specs.Size.y, 0, FormatToGL(specs.Format), GL_UNSIGNED_BYTE, nullptr);
			}
			else if (specs.Type == ImageType::e3D)
			{
				glTexImage3D(GL_TEXTURE_3D, 0, FormatToGL(specs.Format),
					specs.Size.x, specs.Size.y, specs.Size.z, 0, FormatToGL(specs.Format), GL_UNSIGNED_BYTE, nullptr);
			}

			glTexParameteri(Utils::TypeToGL(specs.Type), GL_TEXTURE_WRAP_S, Utils::WrapToGL(specs.Wrap));
			glTexParameteri(Utils::TypeToGL(specs.Type), GL_TEXTURE_WRAP_T, Utils::WrapToGL(specs.Wrap));
			glTexParameteri(Utils::TypeToGL(specs.Type), GL_TEXTURE_WRAP_R, Utils::WrapToGL(specs.Wrap));

			glTexParameteri(Utils::TypeToGL(specs.Type), GL_TEXTURE_MIN_FILTER, Utils::TilingToGL(specs.Tiling));
			glTexParameteri(Utils::TypeToGL(specs.Type), GL_TEXTURE_MAG_FILTER, Utils::TilingToGL(specs.Tiling));
		}

		static void SetGLImageData(const ImageSpecs& specs, void* data)
		{
			if (specs.Type == ImageType::e1D)
			{
				glTexSubImage1D(GL_TEXTURE_2D, 0, 0, specs.Size.x, FormatToGL(specs.Format), GL_UNSIGNED_BYTE, data);
			}
			else if (specs.Type == ImageType::e2D)
			{
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, specs.Size.x, specs.Size.y, FormatToGL(specs.Format), GL_UNSIGNED_BYTE, data);
			}
			else if (specs.Type == ImageType::e3D)
			{
				glTexSubImage3D(GL_TEXTURE_2D, 0, 0, 0, 0, specs.Size.x, specs.Size.y, specs.Size.z, FormatToGL(specs.Format), GL_UNSIGNED_BYTE, data);
			}
		}
	}

	Image::Image(const ImageSpecs& specs)
		: m_RenderID(0)
	{
		glGenTextures(1, &m_RenderID);
		glBindTexture(Utils::TypeToGL(specs.Type), m_RenderID);
		Utils::SetGLImage(specs);
	}

	void Image::Bind(uint32_t slot) const
	{
		glBindTexture(Utils::TypeToGL(m_Specs.Type), m_RenderID);
		glBindTextureUnit(slot, m_RenderID);
	}

	void Image::SetData(void* data, size_t size)
	{
		size_t imageSize = static_cast<size_t>((m_Specs.Size.x * m_Specs.Size.y * m_Specs.Size.z) * Utils::FormatSize(m_Specs.Format));
		if (size != imageSize)
		{
			Log::Error("Size does not match Image Size! Expected: {}", imageSize);
			return;
		}

		glBindTexture(Utils::TypeToGL(m_Specs.Type), m_RenderID);
		Utils::SetGLImageData(m_Specs, data);
	}

	// Only supports 2D or 1D texture data
	void Image::LoadData(const std::string& path)
	{
		int nrChannels, x, y;
		stbi_set_flip_vertically_on_load(m_Specs.FlipVertical);
		unsigned char* data = stbi_load(path.c_str(), &x, &y, &nrChannels, 0);

		glBindTexture(Utils::TypeToGL(m_Specs.Type), m_RenderID);
		m_Specs.Size = UVector3((uint32_t)x, (uint32_t)y, 1);
		Utils::SetGLImage(m_Specs);
		Utils::SetGLImageData(m_Specs, data);

		stbi_image_free(data);
	}

	void* Image::GetRenderID() const
	{
		return reinterpret_cast<void*>((uint32_t)m_RenderID);
	}
}