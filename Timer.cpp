/**
 * Timer.cpp source file
 * 
 * This class implements timer that can be
 * used to compute time between two function calls.
 */

#include "Timer.h"

CTimer::CTimer(void)
{
	m_ddwStartTicks = 0;
	m_ddwTickFrequency = 0;
	m_fElapsedSeconds = 0.0f;
}

CTimer::~CTimer(void)
{
}

void CTimer::Create(void)
{
	// get the performance timer frequency from windows
	::QueryPerformanceFrequency( (LARGE_INTEGER*)&m_ddwTickFrequency);
	m_fElapsedSeconds = 0.0f;
}


void CTimer::Release(void)
{
	// nothing to do really...
}


void CTimer::BeginTimer(void)
{
	// get the start ticks
	::QueryPerformanceCounter( (LARGE_INTEGER*)&m_ddwStartTicks);
}


void CTimer::EndTimer(void)
{
	if(m_ddwStartTicks == 0)
	{
		// maybe BeginTimer was not called???
		m_fElapsedSeconds = 0.0f;
	}
	else
	{
		// compute elapsed seconds
		INT64 ddwEndTicks;
		float ftemp;

		do
		{
			::QueryPerformanceCounter( (LARGE_INTEGER*)&ddwEndTicks);
			ftemp = (float)(ddwEndTicks - m_ddwStartTicks);
		} while (ftemp < 0.0000001f);

		m_fElapsedSeconds = ftemp / (float)m_ddwTickFrequency;
	}
}

