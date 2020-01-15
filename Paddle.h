#pragma once

#include "resource.h"
#include "Point2D.h"

#define MAX_AI_SPEED 200
#define PADDLE_WIDTH 100

class Paddle
{
public:
	Paddle(float xPos);
	~Paddle();

	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget);
	void MoveToPosition(int yPos);
	void MoveAI(double elapsedTime, float ballYPos);
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);

	Point2D position;

private:

	ID2D1SolidColorBrush* m_pBlueBrush;
};

