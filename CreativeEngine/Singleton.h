#pragma once
#include <memory>

namespace dae
{
	template<typename T>
	class Singleton
	{
	public:

		static std::shared_ptr<T> GetInstance();

		virtual ~Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) noexcept = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) noexcept = delete;

		void Destroy() const;
		
	protected:
		Singleton() = default;
	private:
		static std::shared_ptr<T> m_pInstance;
	};

	template<typename T>
	std::shared_ptr<T> Singleton<T>::m_pInstance{};

	template <typename T>
	std::shared_ptr<T> Singleton<T>::GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = std::make_shared<T>();
		return m_pInstance;
	}

	template <typename T>
	void Singleton<T>::Destroy() const
	{
		m_pInstance.reset();
	}
}

