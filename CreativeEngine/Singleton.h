#pragma once
#include <memory>

namespace dae
{
	template<typename T>
	class Singleton
	{
	public:
		static T& GetInstance();

		virtual ~Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) noexcept = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) noexcept = delete;
		
	protected:
		Singleton() = default;
	};

	template <typename T>
	T& Singleton<T>::GetInstance()
	{
		static T instance{};
		return instance;
	}
}

