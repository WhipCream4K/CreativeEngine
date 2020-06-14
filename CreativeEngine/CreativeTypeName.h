#pragma once
#include <variant>

namespace dae
{
	class BaseComponent;
	class Scene;
	class GameObject;
	
	template<typename T>
	using GameComponent = typename std::enable_if<std::is_base_of_v<BaseComponent, T>, T>::type;

	template<typename T>
	using GameObjectType = typename std::enable_if<std::is_base_of_v<GameObject, T>, T>::type;

	template<typename T>
	using SceneType = typename std::enable_if<std::is_base_of_v<Scene, T>, T>::type;

	using ConditionFunc = std::shared_ptr<IMulticastCondition>;
	using ActionFunc = std::shared_ptr<IMuticastAction>;

	template<typename T>
	static constexpr bool IsValidAudioDataTypes{
		std::is_same_v<T,FMOD::Sound>
	};
	
	using AudioRawData = std::variant<std::shared_ptr<FMOD::Channel>>;
}
