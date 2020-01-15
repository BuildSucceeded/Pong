#pragma once

#include "resource.h"
#include "Point2D.h"

#define RECOIL_Y_MAX 0.5
#define RECOIL_X_MIN -0.2
#define RECOIL_X_MAX 0.1

class Ball
{
public:
	Ball();
	~Ball();

	void Reset();
	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Advance(double elapsedTime);
	void CheckHitLeftPaddle(float paddleY);
	void CheckHitRightPaddle(float paddleY);
	boolean IsOutsideLeft();
	boolean IsOutsideRight();
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);

	Point2D position;
	Point2D speed;
private:

	ID2D1SolidColorBrush* m_pRedBrush;
};

