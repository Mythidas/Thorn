#pragma once

#include <memory>

#define THORN_MAKE_REF(ClassName) \
	template <typename... Args> \
	inline static Ref<ClassName> Create(Args&&... args) { \
		ClassName* obj = new ClassName(std::forward<Args>(args)...); \
		return Ref<ClassName>(obj); \
	}

#define THORN_MAKE_SCOPE(ClassName) \
	template <typename... Args> \
	inline static Scope<ClassName> Create(Args&&... args) { \
		ClassName* obj = new ClassName(std::forward<Args>(args)...); \
		return Scope<ClassName>(obj); \
	}

namespace Thorn
{
	// Scope for unique pointers
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	Scope<T> CreateScope(Args ...args)
	{
		return std::make_unique<T>(args...);
	}

	// Reference for shared pointers
	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ...Args>
	Ref<T> CreateRef(Args... args)
	{
		return std::make_shared<T>(args...);
	}

	// Weak reference for weak pointers
	template <typename T>
	using WeakRef = std::weak_ptr<T>;
}