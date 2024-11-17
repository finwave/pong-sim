/**
 * ObjectBall.cpp source file
 */

// Game classes
#include "TheApp.h"
#include "ObjectPad.h"
#include "ObjectBall.h"

void ObjectBall::Initialize(CTheApp* app, int playAreaBorderHeight, int padWidth)
{
	m_TheApp = app;

	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	// VALUE GENERATORS

	// Will be used to obtain a seed for the random number engine
	std::random_device rd;
	// Standard mersenne_twister_engine seeded with rd()
	m_GeneratorVelocityY.seed(rd());

	std::uniform_real_distribution<> dis(-m_dMaxVelocityAngle, m_dMaxVelocityAngle);
	m_DistributionVelocityY = dis;

	// PIXEL TO METER RATIO

	float designAspectRatio = 1024.0f / 768.0f;
	float currentAspectRatio = (float)windowWidth / (float)windowHeight;

	m_fPixelToMeterRatio = designAspectRatio / currentAspectRatio * BALL_SPEED;

	// PLAY AREA DIMENSIONS

	m_fScreenWidth = (float)windowWidth;
	m_fScreenHeight = (float)windowHeight;
	m_fPlayAreaBorderHeight = (float)playAreaBorderHeight;
	m_fPadWidth = (float)padWidth;

	m_fBallRadius = GetObjectBallRadius(windowHeight);

	// COLLISION POINTS

	m_bIsUpdateCollisionPoint = true;
}

void ObjectBall::SetStartDirection(EDirection direction, float startPosY)
{
	// POSITION

	m_CurrentPos.x = m_fScreenWidth / 2.0f;
	m_CurrentPos.y = startPosY;

	// VELOCITY

	switch (direction)
	{
	case Left:
		m_CurrentVelocity.x = -1.0f;
		break;
	case Right:
		m_CurrentVelocity.x = 1.0f;
		break;
	}

	float randomAngleY = m_DistributionVelocityY(m_GeneratorVelocityY);

	float currentBounceAngle = 100.0f * abs(randomAngleY);
	float deltaVerticalVelocity = currentBounceAngle / DEFAULT_PAD_BOUNCE_ANGLE;
	float verticalVelocity = deltaVerticalVelocity * abs(m_CurrentVelocity.x);

	if (randomAngleY > 0)
	{
		// top of the screen is 0 value
		// traveling up = negative velocity
		verticalVelocity = -verticalVelocity;
	}

	m_CurrentVelocity.y = verticalVelocity;
	NormalizeBallVelocity();
}

void ObjectBall::Update(float deltaTime, ObjectPad* objectPadLeft, ObjectPad* objectPadRight)
{
	if (m_bIsUpdateCollisionPoint)
	{
		m_bIsUpdateCollisionPoint = false;
		UpdateSideCollisionPoint();
	}

	float offsetX = m_fPixelToMeterRatio * deltaTime * m_NormalizedVelocity.x;
	float offsetY = m_fPixelToMeterRatio * deltaTime * -m_NormalizedVelocity.y;

	float newPosX = m_CurrentPos.x + offsetX;
	float newPosY = m_CurrentPos.y + offsetY;

	bool isHorizontalCollision = false;
	bool isVerticalCollision = false;

	// HORIZONTAL COLLISION
	
	// Left side pad.
	if ((m_CurrentVelocity.x < 0) && 
		((newPosX - m_fBallRadius) < m_fPadWidth) &&
		(newPosY >= objectPadLeft->GetCurrentTopPoint()) && 
		(newPosY <= objectPadLeft->GetCurrentBottomPoint()))
	{
		objectPadLeft->SetNextBallCollisionMargin();

		UpdateVerticalVelocityFromPadCollision(objectPadLeft);

		float ballLeftEdgePos = newPosX - m_fBallRadius;
		float distanceOffset = m_fPadWidth - ballLeftEdgePos;
		newPosX += distanceOffset;

		isHorizontalCollision = true;

		// play sound effect
		m_TheApp->GetSoundManager()->PlaySoundHitPad();
	}
	// Right side pad.
	else if ((m_CurrentVelocity.x > 0) && 
		((newPosX + m_fBallRadius) > (m_fScreenWidth - m_fPadWidth)) &&
		(newPosY >= objectPadRight->GetCurrentTopPoint()) &&
		(newPosY <= objectPadRight->GetCurrentBottomPoint()))
	{
		objectPadRight->SetNextBallCollisionMargin();

		UpdateVerticalVelocityFromPadCollision(objectPadRight);

		float distanceOffset = (newPosX + m_fBallRadius) - (m_fScreenWidth - m_fPadWidth);
		newPosX -= distanceOffset;

		isHorizontalCollision = true;

		// play sound effect
		m_TheApp->GetSoundManager()->PlaySoundHitPad();
	}
	// Left edge of the screen.
	else if ((m_CurrentVelocity.x < 0) && (newPosX - m_fBallRadius) < 0)
	{
		float ballLeftEdgePos = newPosX - m_fBallRadius;
		float distanceOffset = -1.0f * ballLeftEdgePos;
		newPosX += distanceOffset;

		isHorizontalCollision = true;
	}
	// Right edge of the screen.
	else if ((m_CurrentVelocity.x > 0) && (newPosX + m_fBallRadius) > m_fScreenWidth)
	{
		float distanceOffset = (newPosX + m_fBallRadius) - m_fScreenWidth;
		newPosX -= distanceOffset;

		isHorizontalCollision = true;
	}

	// VERTICAL COLLISION

	// Bottom edge of the screen.
	if ((newPosY + m_fBallRadius) > (m_fScreenHeight - m_fPlayAreaBorderHeight))
	{
		float distanceOffset = (newPosY + m_fBallRadius) - (m_fScreenHeight - m_fPlayAreaBorderHeight);
		newPosY -= distanceOffset;

		isVerticalCollision = true;

		// play sound effect
		m_TheApp->GetSoundManager()->PlaySoundHitWall();
	}
	// Top edge of the screen.
	else if ((newPosY - m_fBallRadius) < m_fPlayAreaBorderHeight)
	{
		float distanceOffset = m_fPlayAreaBorderHeight - (newPosY - m_fBallRadius);
		newPosY += distanceOffset;

		isVerticalCollision = true;

		// play sound effect
		m_TheApp->GetSoundManager()->PlaySoundHitWall();
	}

	// UPDATE POSITION VALUES

	m_CurrentPos.x = newPosX;
	m_CurrentPos.y = newPosY;

	// UPDATE VELOCITY VALUES
	
	// flip X velocity vector
	if (isHorizontalCollision)
	{
		m_CurrentVelocity.x = -m_CurrentVelocity.x;
		m_bIsUpdateCollisionPoint = true;
	}

	// flip Y velocity vector
	if (isVerticalCollision)
	{
		m_CurrentVelocity.y = -m_CurrentVelocity.y;
	}

	if (isHorizontalCollision || isVerticalCollision)
	{
		NormalizeBallVelocity();
	}
}

void ObjectBall::UpdateVerticalVelocityFromPadCollision(ObjectPad* objectPad)
{
	ObjectPad::MyVector padPosition = objectPad->GetCurrentPosition();

	float padHalfHeight = objectPad->GetPadHeight() / 2.0f;
	float padPosCenter = padPosition.y;
	float padPosTop = padPosition.y + padHalfHeight;
	float padPosBottom = padPosition.y - padHalfHeight;

	float ballPosY = m_CurrentPos.y;
	float ballDeltaY = 0;

	if (ballPosY > padPosCenter)
	{
		ballDeltaY = padHalfHeight - (padPosTop - ballPosY);
	}
	else if (ballPosY < padPosCenter)
	{
		ballDeltaY = padHalfHeight - (ballPosY - padPosBottom);
	}

	if (ballDeltaY > 0)
	{
		float delta = ballDeltaY / padHalfHeight;

		float currentBounceAngle = MAX_PAD_BOUNCE_ANGLE * delta;
		float deltaVerticalVelocity = currentBounceAngle / DEFAULT_PAD_BOUNCE_ANGLE;
		float verticalVelocity = deltaVerticalVelocity * abs(m_CurrentVelocity.x);

		if (ballPosY > padPosCenter)
		{
			// top of the screen is 0 value
			// traveling up = negative velocity
			verticalVelocity = -verticalVelocity;
		}

		m_CurrentVelocity.y = verticalVelocity;
	}
	else
	{
		m_CurrentVelocity.y = 0;
	}
}

void ObjectBall::UpdateSideCollisionPoint(void)
{
	m_TempVector.x = m_NormalizedVelocity.x;
	m_TempVector.y = m_NormalizedVelocity.y;

	m_CurrentSideCollisionPoint.x = -1.0f;
	m_CurrentSideCollisionPoint.y = -1.0f;

	float currentPosX = m_CurrentPos.x;
	float currentPosY = m_CurrentPos.y;

	float offsetX = 0;
	float offsetY = 0;

	float deltaTimeX = 0;
	float deltaTimeY = 0;

	bool isUpdateCycle = true;

	while (isUpdateCycle)
	{
		// OFFSET

		// moving right
		if (m_TempVector.x > 0)
		{
			offsetX = m_fScreenWidth - m_fPadWidth - m_fBallRadius - currentPosX;
		}
		// moving left
		else if (m_TempVector.x < 0)
		{
			offsetX = currentPosX - m_fPadWidth - m_fBallRadius;
		}

		// moving down
		if (m_TempVector.y < 0)
		{
			offsetY = m_fScreenHeight - m_fPlayAreaBorderHeight - m_fBallRadius - currentPosY;
		}
		// moving up
		else if (m_TempVector.y > 0)
		{
			offsetY = currentPosY - m_fBallRadius - m_fPlayAreaBorderHeight;
		}

		// DELTATIME

		deltaTimeX = 1000;
		deltaTimeY = 1000;

		if (offsetX > 0)
		{
			deltaTimeX = abs(offsetX / m_fPixelToMeterRatio / m_TempVector.x);
		}

		if (offsetY > 0)
		{
			deltaTimeY = abs(offsetY / m_fPixelToMeterRatio / m_TempVector.y);
		}

		// NEXT POINT TO HIT

		float deltaTime = deltaTimeY;

		if (deltaTimeX < deltaTimeY)
		{
			deltaTime = deltaTimeX;
		}

		float targetOffsetX = m_fPixelToMeterRatio * deltaTime * m_TempVector.x;
		float targetOffsetY = m_fPixelToMeterRatio * deltaTime * -m_TempVector.y;

		float newPosX = currentPosX + targetOffsetX;
		float newPosY = currentPosY + targetOffsetY;

		if (deltaTimeX < deltaTimeY)
		{
			m_CurrentSideCollisionPoint.x = newPosX;
			m_CurrentSideCollisionPoint.y = newPosY;

			isUpdateCycle = false;
		}
		else
		{
			// flip Y velocity vector
			m_TempVector.y = -m_TempVector.y;
		}

		currentPosX = newPosX;
		currentPosY = newPosY;
	}
}

void ObjectBall::NormalizeBallVelocity(void)
{
	m_NormalizedVelocity.x = m_CurrentVelocity.x;
	m_NormalizedVelocity.y = m_CurrentVelocity.y;

	float absoluteVelocityX = abs(m_CurrentVelocity.x);
	float absoluteVelocityY = abs(m_CurrentVelocity.y);

	float absoluteTotalVelocity = absoluteVelocityX + absoluteVelocityY;

	if (absoluteTotalVelocity > 1.0f)
	{
		float deltaNormalize = 1.0f;

		if (absoluteVelocityX > absoluteVelocityY)
		{
			deltaNormalize = 1.0f / absoluteVelocityX;
		}
		else
		{
			deltaNormalize = 1.0f / absoluteVelocityY;
		}

		m_NormalizedVelocity.x = deltaNormalize * m_CurrentVelocity.x;
		m_NormalizedVelocity.y = deltaNormalize * m_CurrentVelocity.y;
	}
}

void ObjectBall::Render(BLContext* ctx, int windowWidth, int windowHeight)
{
	BLRgba32 color(COLOR_OBJECT_BALL);

	float size = 2.0f * m_fBallRadius;
	float topLeftCornerX = m_CurrentPos.x - m_fBallRadius;
	float topLeftCornerY = m_CurrentPos.y - m_fBallRadius;

	ctx->setFillStyle(color);
	ctx->fillRoundRect(topLeftCornerX, topLeftCornerY, size, size, m_fBallRadius);

	// DEBUG VISUALS
	if (ENABLE_DEBUG_VISUALS)
	{
		// side collision point
		if ((m_CurrentSideCollisionPoint.x != -1.0f) && (m_CurrentSideCollisionPoint.y != -1.0f))
		{
			BLRgba32 colorDebug(COLOR_DEBUG);

			topLeftCornerX = m_CurrentSideCollisionPoint.x - m_fBallRadius;
			topLeftCornerY = m_CurrentSideCollisionPoint.y - m_fBallRadius;

			ctx->setFillStyle(colorDebug);
			ctx->fillRoundRect(topLeftCornerX, topLeftCornerY, size, size, m_fBallRadius);
		}
	}
}
