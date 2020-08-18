#pragma once
#include <variant>

namespace dae
{
	class BaseComponent;
	class Scene;
	class GameObject;
	class SDLTextureData;
	struct IMulticastCondition;
	struct IMuticastAction;
	class SDLFontData;
	class PhysicsComponent;
	class Collider;
	
	template<typename T>
	using GameComponent = typename std::enable_if<std::is_base_of_v<BaseComponent, T>, T>::type;

	template<typename T>
	using GameObjectType = typename std::enable_if<std::is_base_of_v<GameObject, T>, T>::type;

	template<typename T>
	using SceneType = typename std::enable_if<std::is_base_of_v<Scene, T>, T>::type;

	template<typename T>
	using PhysicsObject = std::enable_if_t<std::is_base_of_v<PhysicsComponent, T>, T>;

	template<typename T>
	using ColliderType = std::enable_if_t<std::is_base_of_v<Collider, T>, T>;
	
	using ConditionFunc = std::shared_ptr<IMulticastCondition>;
	using ActionFunc = std::shared_ptr<IMuticastAction>;
	
	template<typename T>
	static constexpr bool IsValidAudioDataTypes{
		std::is_same_v<T,FMOD::Sound>
	};
	
	using AudioRawData = std::variant<std::shared_ptr<FMOD::Channel>>;
	using DefaultTextureData = SDLTextureData;
	using DefaultFontData = SDLFontData;
}
