#include "framework.h"
#include "Point2D.h"
#include "Paddle.h"
#include "Engine.h"

Paddle::Paddle(float xPos) : m_pBlueBrush(NULL)
{
	// Initializes the position of the paddle with a fixed X position and Y being at the middle of the screen
	position.x = xPos;
	position.y = RESOLUTION_Y / 2;
}

Paddle::~Paddle()
{
	SafeRelease(&m_pBlueBrush);
}

void Paddle::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a blue brush for drawing
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&m_pBlueBrush
	);
}

void Paddle::MoveToPosition(int yPos)
{
	// Moves the paddle to a specified position, also making sure it doesn't go outside the screen
	position.y = yPos;
	if (position.y < PADDLE_WIDTH / 2)
	{
		position.y = PADDLE_WIDTH / 2;
	}
	if (position.y > RESOLUTION_Y - PADDLE_WIDTH / 2)
	{
		position.y = RESOLUTION_Y - PADDLE_WIDTH / 2;
	}
}

void Paddle::MoveAI(double elapsedTime, float ballYPos)
{
	// Moves the paddle up or down, using a basic AI, also making sure it doesn't go outside the screen
	if (ballYPos < position.y)
	{
		// If the ball is higher, it moves up at a max speed
		position.y -= min(position.y - ballYPos, MAX_AI_SPEED * elapsedTime);
		if (position.y < PADDLE_WIDTH / 2)
		{
			position.y = PADDLE_WIDTH / 2;
		}
	}
	if (ballYPos > position.y)
	{
		// If the ball is lower, it moves down at a max speed
		position.y += min(ballYPos - position.y, MAX_AI_SPEED * elapsedTime);
		if (position.y > RESOLUTION_Y - PADDLE_WIDTH / 2)
		{
			position.y = RESOLUTION_Y - PADDLE_WIDTH / 2;
		}
	}
}

void Paddle::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Draws a rectangle representing the paddle
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		position.x - 5, position.y - PADDLE_WIDTH / 2,
		position.x + 5, position.y + PADDLE_WIDTH / 2
	);
	m_pRenderTarget->FillRectangle(&rectangle1, m_pBlueBrush);
}