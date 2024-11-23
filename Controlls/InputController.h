#pragma once

#include <windows.h>
#include <memory>
#include <unordered_map>

#include "Camera/Camera.h"
#include "Timer/GameTimer.h"

class Camera;

class InputController
{
public:
    InputController(
        std::shared_ptr<Camera> camera,
        HWND hMainWnd)
    {
        mCamera = camera;
        mhMainWnd = hMainWnd;
        
        mMousePosition = { 0, 0 };
        mLastMousePosition = { 0, 0 };
    }
    
    void OnKeyDown(WPARAM key);
    void OnKeyUp(WPARAM key);
    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);
    
    bool IsKeyDown(WPARAM key) const;
    bool IsMouseButtonDown(WPARAM btnState) const;
    
    POINT GetMousePosition() const;
    POINT GetMouseDelta() const;
    void OnKeyboardInput(const GameTimer& gt);

private:
    std::unordered_map<WPARAM, bool> mKeyState;
    std::unordered_map<WPARAM, bool> mMouseButtonState;
    POINT mMousePosition;
    POINT mLastMousePosition;
    std::shared_ptr<Camera> mCamera;
    HWND mhMainWnd = nullptr;
};
