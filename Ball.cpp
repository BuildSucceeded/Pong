#include "framework.h"
#include "Point2D.h"
#include "Ball.h"
#include "Paddle.h"
#include "Engine.h"

Ball::Ball() : m_pRedBrush(NULL)
{
	// Initial ball position and speed
	this->Reset();
}

Ball::~Ball()
{
	SafeRelease(&m_pRedBrush);
}

void Ball::Reset()
{
	// Sets an initial ball position and speed. X is fixed, Y is random
	position.x = 100;
	position.y = (double)rand() / (RAND_MAX + 1) * RESOLUTION_Y;

	speed.x = 300;
	speed.y = 200;
}

void Ball::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Initializes Direct2D red brush for drawing
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pRedBrush
	);
}

void Ball::Advance(double elapsedTime)
{
	// Logic of the ball movement. If it hits the upper or lower walls it bounces back
	position.x += speed.x * elapsedTime;
	position.y += speed.y * elapsedTime;

	if (position.y > RESOLUTION_Y - 10)
		speed.y = -abs(speed.y);
	if (position.y < 10)
		speed.y = abs(speed.y);
}

void Ball::CheckHitLeftPaddle(float paddleY)
{
	// Checks if the ball hits the left paddle. If it does, it bounces back
	if (position.x < 20 && speed.x < 0)
	{
		if (position.y > paddleY - PADDLE_WIDTH / 2 && position.y < paddleY + PADDLE_WIDTH / 2)
		{
			// We don't simply want to reverse the x speed. In order to make the game more interesting, we change the X and Y speeds based on where on the paddle the ball hita
			float paddleHitPos = (position.y - paddleY) / (PADDLE_WIDTH / 2);
			speed.x = abs(speed.x + speed.x * (1 - abs(paddleHitPos)) * RECOIL_X_MAX - abs(speed.x * paddleHitPos * RECOIL_X_MIN));
			speed.y = speed.y + abs(speed.y) * paddleHitPos * RECOIL_Y_MAX;
			return;
		}
	}
}

void Ball::CheckHitRightPaddle(float paddleY)
{
	// Checks if the ball hits the right paddle. If it does, it bounces back
	if (position.x > RESOLUTION_X - 20 && speed.x > 0)
	{
		if (position.y > paddleY - PADDLE_WIDTH / 2 && position.y < paddleY + PADDLE_WIDTH / 2)
		{
			// We don't simply want to reverse the x speed. In order to make the game more interesting, we change the X and Y speeds based on where on the paddle the ball hita
			float paddleHitPos = (position.y - paddleY) / (PADDLE_WIDTH / 2);
			speed.x = -abs(speed.x + speed.x * (1 - abs(paddleHitPos)) * RECOIL_X_MAX - abs(speed.x * paddleHitPos * RECOIL_X_MIN));
			speed.y = speed.y + abs(speed.y) * paddleHitPos * RECOIL_Y_MAX;
			return;
		}
	}
}

boolean Ball::IsOutsideLeft()
{
	// Returns true if the ball is outside the play area on the left side
	return position.x < 0;
}

boolean Ball::IsOutsideRight()
{
	// Returns true if the ball is outside the play area on the right side
	return position.x > RESOLUTION_X;
}

void Ball::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Draws the ball using Direct2D
	D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
		D2D1::Point2F(position.x, position.y),
		10, 10
	);
	m_pRenderTarget->FillEllipse(&ellipseBall, m_pRedBrush);
}