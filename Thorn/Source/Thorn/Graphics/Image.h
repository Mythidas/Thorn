#pragma once

#include "Thorn/Lib/Pointers.h"
#include "Thorn/Math/Vector.h"

namespace Thorn
{
	enum class ImageType
	{
		e1D,
		e2D,
		e3D
	};

	enum class ImageFormat
	{
		R8,
		RGBA8,
		RGBA32F
	};

	enum class ImageWrap
	{
		Repeat,
		ClampToEdge,
		MirroredRepeat
	};

	enum class ImageTiling
	{
		Linear,
		Nearest
	};

	struct ImageSpecs
	{
		uint32_t Binding{ 0 };
		ImageType Type{ ImageType::e2D };
		ImageFormat Format{ ImageFormat::RGBA8 };
		ImageWrap Wrap{ ImageWrap::Repeat };
		ImageTiling Tiling{ ImageTiling::Nearest };
		UVector3 Size{ 1, 1, 1 };
		bool FlipVertical = false;
	};

	class Image
	{
	public:
		THORN_MAKE_REF(Image)

		void Bind(uint32_t slot) const;
		void SetData(void* data, size_t size);
		void LoadData(const std::string& path);

		UVector3 GetSize() const { return m_Specs.Size; }
		void* GetRenderID() const;

	private:
		Image(const ImageSpecs& specs);

		uint32_t m_RenderID;
		ImageSpecs m_Specs;
	};
}