#include "InputController.h"

#include "GameTimer.h"

void InputController::OnKeyDown(WPARAM key)
{
    mKeyState[key] = true;
}

void InputController::OnKeyUp(WPARAM key)
{
    mKeyState[key] = false;
}

void InputController::OnMouseDown(WPARAM btnState, int x, int y)
{
    mMouseButtonState[btnState] = true;
    mMousePosition = { x, y };
    SetCapture(mhMainWnd);
}

void InputController::OnMouseUp(WPARAM btnState, int x, int y)
{
    mMouseButtonState[btnState] = false;
    mMousePosition = { x, y };
    ReleaseCapture();
}

void InputController::OnMouseMove(WPARAM btnState, int x, int y)
{
    mLastMousePosition = mMousePosition;
    
    if((btnState & MK_LBUTTON) != 0)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = DirectX::XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePosition.x));
        float dy = DirectX::XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePosition.y));

        mCamera->Pitch(dy);
        mCamera->RotateY(dx);
    }

    mMousePosition = { x, y };
}

bool InputController::IsKeyDown(WPARAM key) const
{
    auto it = mKeyState.find(key);
    return it != mKeyState.end() && it->second;
}

bool InputController::IsMouseButtonDown(WPARAM btnState) const
{
    auto it = mMouseButtonState.find(btnState);
    return it != mMouseButtonState.end() && it->second;
}

POINT InputController::GetMousePosition() const
{
    return mMousePosition;
}

POINT InputController::GetMouseDelta() const
{
    POINT delta = { mMousePosition.x - mLastMousePosition.x, mMousePosition.y - mLastMousePosition.y };
    return delta;
}

void InputController::OnKeyboardInput(const GameTimer& gt)
{
    const float dt = gt.DeltaTime();

    if(GetAsyncKeyState('W') & 0x8000)
        mCamera->Walk(10.0f*dt);

    if(GetAsyncKeyState('S') & 0x8000)
        mCamera->Walk(-10.0f*dt);

    if(GetAsyncKeyState('A') & 0x8000)
        mCamera->Strafe(-10.0f*dt);

    if(GetAsyncKeyState('D') & 0x8000)
        mCamera->Strafe(10.0f*dt);

    mCamera->UpdateViewMatrix();
}
