#include "framework.h"
#include "Ball.h"
#include "Paddle.h"
#include "Engine.h"

#pragma comment(lib, "d2d1")

Engine::Engine() : m_pDirect2dFactory(NULL), m_pRenderTarget(NULL)
{
    // Initializes everything in the game: ball, paddles, positions
    ball = new Ball();
    leftPaddle = new Paddle(5);
    rightPaddle = new Paddle(RESOLUTION_X - 5);
    mouseYPos = RESOLUTION_Y / 2;

    playing = true;
}

Engine::~Engine()
{
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
}

HRESULT Engine::InitializeD2D(HWND m_hwnd)
{
    // Initializes Direct2D, to draw with
    D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
    m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &m_pRenderTarget
    );

    ball->Initialize(m_pRenderTarget);

    leftPaddle->Initialize(m_pRenderTarget);
    rightPaddle->Initialize(m_pRenderTarget);

    return S_OK;
}

void Engine::Reset()
{
    // This method reset the game, given that the game was won or lost
    if (!playing)
    {
        ball->Reset();
        playing = true;
    }
}

void Engine::MousePos(int x, int y)
{
    // receives the mouse position, to be used on the right paddle
    mouseYPos = y;
}

void Engine::Logic(double elapsedTime)
{
    // This is the logic part of the engine. It receives the elapsed time from the app class, in seconds.
    // It uses this value for a smooth and consistent movement, regardless of the CPU or graphics speed
    if (playing)
    {
        // Moves the ball forward
        ball->Advance(elapsedTime);

        // Moves the left paddle using AI
        leftPaddle->MoveAI(elapsedTime, ball->position.y);

        // Moves the right paddle based on the received mouse position
        rightPaddle->MoveToPosition(mouseYPos);

        // Checks if the ball hits the left/right paddle and apply the bounce
        ball->CheckHitLeftPaddle(leftPaddle->position.y);
        ball->CheckHitRightPaddle(rightPaddle->position.y);

        // If it goes outisde of th eplaying area, the game stops
        if (ball->IsOutsideLeft())
        {
            playing = false;
        }
        if (ball->IsOutsideRight())
        {
            playing = false;
        }
    }
}

HRESULT Engine::Draw()
{
    // This is the drawing method of the engine.
    // It simply draws all the elements in the game using Direct2D
    HRESULT hr;

    m_pRenderTarget->BeginDraw();

    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


    m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    ball->Draw(m_pRenderTarget);

    leftPaddle->Draw(m_pRenderTarget);
    rightPaddle->Draw(m_pRenderTarget);
    
    hr = m_pRenderTarget->EndDraw();

    return S_OK;
}