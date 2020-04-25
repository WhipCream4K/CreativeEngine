#pragma once
namespace dae
{
	class GameTime final
	{
	public:

		GameTime();
		~GameTime() = default;
		GameTime(const GameTime&) = delete;
		GameTime(GameTime&&) noexcept = delete;
		GameTime& operator=(const GameTime&) = delete;
		GameTime& operator=(GameTime&&) noexcept = delete;

		void Update();
		void Start();
		void Stop();
		void Reset();
		
		float GetDeltaSeconds() const { return m_DeltaTime; }
		float GetTotal() const { return m_TotalGameTime; }
		float GetTimeScale() const { return m_TimeScale; }
		int GetFPS() const { return m_FPS; }
	private:

		float m_TotalGameTime;
		float m_DeltaTime;
		float m_SecondsPerCount;
		float m_TimeScale;

		__int64 m_BaseTime;
		__int64 m_PausedTime;
		__int64 m_StopTime;
		__int64 m_PrevTime;
		__int64 m_CurrTime;

		int m_FPS;
		float m_FPSTimer;
		int m_FPSCount;

		bool m_IsStopped;
	};
}

