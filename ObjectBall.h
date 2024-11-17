/**
 * ObjectBall.h header file
 */

#pragma once

#ifndef __OBJECTBALL_H__
#define __OBJECTBALL_H__

#include <random>

// Blend2d
#include <blend2d.h>

// Game classes (forward declaration)
class CTheApp;
class ObjectPad;

class ObjectBall
{
public:

	struct MyVector
	{
		float x, y;
	};

	enum EDirection
	{
		Left,
		Right
	};

	void Initialize(CTheApp* app, int playAreaBorderHeight, int padWidth);
	void SetStartDirection(EDirection direction, float startPosY);

	void Update(float deltaTime, ObjectPad* objectPadLeft, ObjectPad* objectPadRight);
	void Render(BLContext* ctx, int windowWidth, int windowHeight);

	inline MyVector GetCurrentPosition(void) { return m_CurrentPos; }
	inline MyVector GetCurrentVelocity(void) { return m_CurrentVelocity; }
	inline MyVector GetCurrentSideCollisionPoint(void) { return m_CurrentSideCollisionPoint; }

	inline float GetPixelToMeterRatio(void) { return m_fPixelToMeterRatio; }
	inline float GetBallRadius(void) { return m_fBallRadius; }

private:

	CTheApp* m_TheApp;

	// VISUAL DESIGN

	const uint32_t COLOR_OBJECT_BALL = 0xFFFFFFFF;
	const uint32_t COLOR_DEBUG = 0xFF962018;

	inline float GetObjectBallRadius(int windowHeight) { return (float)windowHeight / 42.0f; }

	// VALUE GENERATORS

	const double m_dMaxVelocityAngle = 0.85;

	std::mt19937 m_GeneratorVelocityY;
	std::uniform_real_distribution<> m_DistributionVelocityY;

	// PIXEL TO METER RATIO

	const float BALL_SPEED = 650.0f;
	float m_fPixelToMeterRatio;

	// PLAY AREA DIMENSIONS

	float m_fScreenWidth;
	float m_fScreenHeight;
	float m_fPlayAreaBorderHeight;
	float m_fPadWidth;

	float m_fBallRadius;

	// POSITION AND VELOCITY

	void NormalizeBallVelocity(void);

	const float DEFAULT_PAD_BOUNCE_ANGLE = 45.0f;
	const float MAX_PAD_BOUNCE_ANGLE = 85.0f;

	MyVector m_CurrentPos;
	MyVector m_CurrentVelocity;
	MyVector m_NormalizedVelocity;

	// COLLISION POINTS

	void UpdateVerticalVelocityFromPadCollision(ObjectPad* objectPad);
	void UpdateSideCollisionPoint(void);

	MyVector m_TempVector;
	MyVector m_CurrentSideCollisionPoint;

	bool m_bIsUpdateCollisionPoint;

	// DEBUG VISUALS

	const bool ENABLE_DEBUG_VISUALS = false;
};

#endif //__OBJECTBALL_H__