/**
 * Timer.h header file
 * 
 * This class implements timer that can be
 * used to compute time between two function calls.
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>

class CTimer
{
public:
	CTimer(void);
	virtual ~CTimer(void);

	/**
	 * Create
	 * initialise the timer
	 */
	void Create(void);

	/**
	 * Release
	 */
	void Release(void);

	/**
	 * BeginTimer
	 * get the start value of the timer
	 */
	void BeginTimer(void);

	/**
	 * EndTimer
	 * compute time elapsed from BeginTimer call
	 */
	void EndTimer(void);

	/**
	 * GetElapsedSeconds
	 * @return seconds elapsed in between last calls to
	 * BeginTimer and EndTimer functions
	 */
	inline float GetElapsedSeconds() const { return m_fElapsedSeconds; }

private:
	// timer values
	INT64			m_ddwStartTicks;
	INT64			m_ddwTickFrequency;

	// seconds elapsed in between BeginTimer and
	// EndTimer function calls
	float			m_fElapsedSeconds;
};

#endif	//__TIMER_H__