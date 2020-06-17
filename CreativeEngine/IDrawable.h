#pragma once

namespace dae
{
	struct RenderTransform;
	class IDrawable
	{
	public:

		IDrawable() = default;
		virtual ~IDrawable() = default;

		template<typename T = IDrawable>
		std::shared_ptr<std::enable_if_t<std::is_base_of_v<IDrawable, T>, T>> GetSmart();

		template<typename T = const IDrawable>
		std::shared_ptr<std::enable_if_t<std::is_base_of_v<IDrawable, T>, T>> GetSmart() const;

		virtual void Render(const RenderTransform&) const = 0;
	};

	template <typename T>
	std::shared_ptr<std::enable_if_t<std::is_base_of_v<IDrawable, T>, T>> IDrawable::GetSmart()
	{
		std::shared_ptr<T> temp{ std::shared_ptr<T>(this,[](T*) {}) };
		return std::static_pointer_cast<T>(temp);
	}

	template <typename T>
	std::shared_ptr<std::enable_if_t<std::is_base_of_v<IDrawable, T>, T>> IDrawable::GetSmart() const
	{
		std::shared_ptr<const T> temp{ std::shared_ptr<const T>(this,[](const T*){}) };
		return std::static_pointer_cast<const T>(temp);
	}
}

