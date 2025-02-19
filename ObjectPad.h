/**
 * ObjectPad.h header file
 */

#pragma once

#ifndef __OBJECTPAD_H__
#define __OBJECTPAD_H__

#include <random>
#include <windows.h>

// Blend2d
#include <blend2d.h>

// Game classes (forward declaration)
class ObjectBall;

class ObjectPad
{
public:

	struct MyVector
	{
		float x, y;
	};

	enum EPadSide
	{
		Left,
		Right
	};

	enum EPlayerMovement
	{
		None,
		Up,
		Down
	};

	void Initialize(int windowWidth, int windowHeight, int playAreaBorderHeight,
		int padWidth, int padHeight, double padCornerRadius, EPadSide padSide);

	void SetNextBallCollisionMargin(void);

	void UpdateKeyboard(void);
	void Update(float deltaTime, std::vector<ObjectBall>* arrayBallObjects);
	void Render(BLContext* ctx, int windowWidth, int windowHeight);

	inline void SetPlayerControlled(bool enabled) { m_bPlayerControlled = enabled; }

	inline MyVector GetCurrentPosition(void) { return m_CurrentPos; }
	inline float GetPadHeight(void) { return m_fPadHeight; }

	inline float GetCurrentTopPoint(void)
	{
		float padHalfHeight = m_fPadHeight / 2.0f;
		return m_CurrentPos.y - padHalfHeight;
	}

	inline float GetCurrentBottomPoint(void)
	{
		float padHalfHeight = m_fPadHeight / 2.0f;
		return m_CurrentPos.y + padHalfHeight;
	}

private:

	void CheckBounds(void);

	void RenderLeft(BLContext* ctx, int windowWidth, int windowHeight);
	void RenderRight(BLContext* ctx, int windowWidth, int windowHeight);

	// PLAYER CONTROLS

	EPlayerMovement m_PlayerMovement;
	bool m_bPlayerControlled;

	// VISUAL DESIGN

	const uint32_t COLOR_OBJECT_PAD = 0xFF3C7DFF;

	// PIXEL TO METER RATIO

	const float PAD_SPEED = 450.0f;
	float m_fPixelToMeterRatio;

	// POSITION

	EPadSide m_PadSide;
	MyVector m_CurrentPos;

	// BALL COLLISION MARGIN

	std::mt19937 m_GeneratorBallCollisionMargin;
	std::uniform_real_distribution<> m_DistributionBallCollisionMargin;
	float m_fBallCollisionMargin;

	// PLAY AREA DIMENSIONS

	float m_fScreenWidth;
	float m_fScreenHeight;
	float m_fPlayAreaBorderHeight;
	float m_fPadWidth;
	float m_fPadHeight;
	double m_fPadCornerRadius;

	// BALL TRACKING VALUES

	int m_TrackedBallIndex;
};

#endif //__OBJECTPAD_H__