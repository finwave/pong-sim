/**
 * ObjectPad.cpp source file
 */

// Game classes
#include "ObjectBall.h"
#include "ObjectPad.h"

void ObjectPad::Initialize(int windowWidth, int windowHeight, int playAreaBorderHeight, 
	int padWidth, int padHeight, double padCornerRadius, EPadSide padSide)
{
	// PLAY AREA DIMENSIONS

	m_fScreenWidth = (float)windowWidth;
	m_fScreenHeight = (float)windowHeight;
	m_fPlayAreaBorderHeight = (float)playAreaBorderHeight;
	m_fPadWidth = (float)padWidth;
	m_fPadHeight = (float)padHeight;
	m_fPadCornerRadius = padCornerRadius;

	// PIXEL TO METER RATIO

	float designAspectRatio = 1024.0f / 768.0f;
	float currentAspectRatio = (float)windowWidth / (float)windowHeight;

	m_fPixelToMeterRatio = designAspectRatio / currentAspectRatio * PAD_SPEED;

	// POSITION

	m_PadSide = padSide;
	m_CurrentPos.x = 0;
	m_CurrentPos.y = windowHeight / 2.0f;

	// BALL COLLISION MARGIN

	// Will be used to obtain a seed for the random number engine
	std::random_device rd;
	// Standard mersenne_twister_engine seeded with rd()
	m_GeneratorBallCollisionMargin.seed(rd());

	float maxMargin = m_fPadHeight / 2.0f * 0.85f;
	std::uniform_real_distribution<> dis(-maxMargin, maxMargin);
	m_DistributionBallCollisionMargin = dis;

	SetNextBallCollisionMargin();
}

void ObjectPad::SetNextBallCollisionMargin(void)
{
	m_fBallCollisionMargin = m_DistributionBallCollisionMargin(m_GeneratorBallCollisionMargin);
	m_TrackedBallIndex = -1;
}

void ObjectPad::Update(float deltaTime, std::vector<ObjectBall>* arrayBallObjects)
{
	float padMoveDelta = m_fPixelToMeterRatio * deltaTime * 1.0f;
	bool isTrackingIncomingBall = false;

	// Ball objects exist.
	if (!arrayBallObjects->empty())
	{
		int iterateBallIndex = -1;

		// Track ball objects.
		for (std::vector<ObjectBall>::iterator it = arrayBallObjects->begin(); it != arrayBallObjects->end(); ++it)
		{
			iterateBallIndex++;
			bool isBallToTrack = false;

			ObjectBall::MyVector ballVelocity = it->GetCurrentVelocity();

			// Already tracking a ball with specific index.
			if (m_TrackedBallIndex > 0)
			{
				if (m_TrackedBallIndex == iterateBallIndex)
				{
					isBallToTrack = true;
				}
			}
			// Pad on the left side of the screen and ball moving to the left.
			// Pad on the right side of the screen and ball moving to the right.
			else if (((m_PadSide == Left) && (ballVelocity.x < 0)) ||
				((m_PadSide == Right) && (ballVelocity.x > 0)))
			{
				// Set new ball index to track.
				m_TrackedBallIndex = iterateBallIndex;

				isBallToTrack = true;
			}

			if (isBallToTrack)
			{
				ObjectBall::MyVector ballCollisionPoint = it->GetCurrentSideCollisionPoint();
				ballCollisionPoint.y += m_fBallCollisionMargin;

				float padDistanceToCollisionPoint = abs(m_CurrentPos.y - ballCollisionPoint.y);
				float moveDelta = padMoveDelta;

				if (padDistanceToCollisionPoint < moveDelta)
				{
					moveDelta = padDistanceToCollisionPoint;
				}

				// Collision point is more up than the pad.
				if (ballCollisionPoint.y > m_CurrentPos.y)
				{
					m_CurrentPos.y += moveDelta;
				}
				// Collision point is more down than the pad.
				else if (ballCollisionPoint.y < m_CurrentPos.y)
				{
					m_CurrentPos.y -= moveDelta;
				}

				isTrackingIncomingBall = true;
				break;
			}
		}
	}

	// Move the pad towards the center of the screen.
	if (!isTrackingIncomingBall)
	{
		float centerPointY = m_fScreenHeight / 2.0f;
		float padDistanceToCenterPoint = abs(m_CurrentPos.y - centerPointY);

		if (padDistanceToCenterPoint > 0)
		{
			if (padDistanceToCenterPoint < padMoveDelta)
			{
				padMoveDelta = padDistanceToCenterPoint;
			}

			// Center point is more up than the pad.
			if (centerPointY > m_CurrentPos.y)
			{
				m_CurrentPos.y += padMoveDelta;
			}
			// Center point is more down than the pad.
			else if (centerPointY < m_CurrentPos.y)
			{
				m_CurrentPos.y -= padMoveDelta;
			}
		}
	}

	CheckBounds();
}

void ObjectPad::CheckBounds()
{
	// Top of the screen.
	float screenEdgeTop = m_fPlayAreaBorderHeight;
	float padTopY = m_CurrentPos.y - m_fPadHeight / 2;

	if (padTopY < screenEdgeTop)
	{
		m_CurrentPos.y = screenEdgeTop + m_fPadHeight / 2;
		return;
	}

	// Bottom of the screen.
	float screenEdgeBottom = m_fScreenHeight - m_fPlayAreaBorderHeight;
	float padBottomY = m_CurrentPos.y + m_fPadHeight / 2;

	if (padBottomY > screenEdgeBottom)
	{
		m_CurrentPos.y = screenEdgeBottom - m_fPadHeight / 2;
		return;
	}
}

void ObjectPad::Render(BLContext* ctx, int windowWidth, int windowHeight)
{
	if (m_PadSide == Left)
	{
		RenderLeft(ctx, windowWidth, windowHeight);
	}
	else if (m_PadSide == Right)
	{
		RenderRight(ctx, windowWidth, windowHeight);
	}
}

void ObjectPad::RenderLeft(BLContext* ctx, int windowWidth, int windowHeight)
{
	BLRgba32 color(COLOR_OBJECT_PAD);

	int width = m_fPadWidth + m_fPadCornerRadius;
	int height = m_fPadHeight;

	int currentPosX = -m_fPadCornerRadius;
	int currentPosY = m_CurrentPos.y;

	// top left corner
	float topLeftCornerX = currentPosX;
	float topLeftCornerY = currentPosY - height / 2;

	ctx->setFillStyle(color);
	ctx->fillRoundRect(topLeftCornerX, topLeftCornerY, width, height, m_fPadCornerRadius);
}

void ObjectPad::RenderRight(BLContext* ctx, int windowWidth, int windowHeight)
{
	BLRgba32 color(COLOR_OBJECT_PAD);

	// corner radius

	int width = m_fPadWidth + m_fPadCornerRadius;
	int height = m_fPadHeight;

	int currentPosX = windowWidth - width + m_fPadCornerRadius;
	int currentPosY = m_CurrentPos.y;

	// top left corner 
	float topLeftCornerX = currentPosX;
	float topLeftCornerY = currentPosY - height / 2;

	ctx->setFillStyle(color);
	ctx->fillRoundRect(topLeftCornerX, topLeftCornerY, width, height, m_fPadCornerRadius);
}