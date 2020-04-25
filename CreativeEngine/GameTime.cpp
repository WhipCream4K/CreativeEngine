#include "pch.h"
#include "GameTime.h"

#include <windows.h>

dae::GameTime::GameTime()
	: m_TotalGameTime{}
	, m_DeltaTime{}
	, m_TimeScale{}
	, m_FPS{}
	, m_FPSTimer{}
	, m_BaseTime{}
	, m_PausedTime{}
	, m_SecondsPerCount{}
	, m_StopTime{}
	, m_CurrTime{}
	, m_PrevTime{}
	, m_FPSCount{}
	, m_IsStopped{ true }
{
	__int64 countsPerSecond;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSecond));
	m_SecondsPerCount = 1.0f / static_cast<float>(countsPerSecond);
}

void dae::GameTime::Update()
{
	if (m_IsStopped)
	{
		m_FPS = 0;
		m_DeltaTime = 0.0f;
		m_TotalGameTime = static_cast<float>((m_StopTime - m_PausedTime - m_BaseTime) * m_BaseTime);
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
	m_CurrTime = currTime;

	m_DeltaTime = static_cast<float>((m_CurrTime - m_PrevTime) * m_SecondsPerCount);
	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0f)
		m_DeltaTime = 0.0f;


	m_TotalGameTime = static_cast<float>(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);

	//FPS LOGIC
	m_FPSTimer += m_DeltaTime;
	++m_FPSCount;
	if (m_FPSTimer >= 1.0f)
	{
		m_FPS = m_FPSCount;
		m_FPSCount = 0;
		m_FPSTimer -= 1.0f;
	}
}

void dae::GameTime::Start()
{
	if(m_IsStopped)
	{
		__int64 startTime;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&startTime));

		m_PausedTime += (startTime - m_StopTime);

		m_PrevTime = startTime;
		m_StopTime = 0;
		m_IsStopped = false;
	}
}

void dae::GameTime::Stop()
{
	if(!m_IsStopped)
	{
		__int64 currTime;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&currTime));

		m_StopTime = currTime;
		m_IsStopped = true;
	}
}

void dae::GameTime::Reset()
{
	__int64 currTime;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&currTime));

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_FPSTimer = 0.0f;
	m_FPSCount = 0;
	m_IsStopped = false;
}
