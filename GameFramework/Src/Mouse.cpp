#include "Mouse.h"

#include <iostream>
#include <windowsx.h>

#include "DisplayWin32.h"
#include "Game.h"


void Mouse::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_LBUTTONDOWN:
        SetPressStateByButton(LEFT, true);
        break;

    case WM_LBUTTONUP:
        SetPressStateByButton(LEFT, false);
        break;

    case WM_MBUTTONDOWN:
        SetPressStateByButton(MIDDLE, true);
        break;

    case WM_MBUTTONUP:
        SetPressStateByButton(MIDDLE, false);
        break;

    case WM_RBUTTONDOWN:
        SetPressStateByButton(RIGHT, true);
        break;

    case WM_RBUTTONUP:
        SetPressStateByButton(RIGHT, false);
        break;

    case WM_MOUSEWHEEL:
        OnScroll(GET_WHEEL_DELTA_WPARAM(wParam));
        return;

    case WM_MOUSEMOVE:
    {
        DisplayWin32* disp = Game::GetInstance()->GetDisplay();
        const int posX = GET_X_LPARAM(lParam);
        const int posY = GET_Y_LPARAM(lParam);
        deltaX = (prevX - posX) / static_cast<float>(disp->GetClientWidth());
        deltaY = (prevY - posY) / static_cast<float>(disp->GetClientHeight());
        prevX = posX;
        prevY = posY;
        return;
    }
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    default:
        // Not a mouse message, so exit
        return;
    }
}

void Mouse::OnScroll(int scrollDelta)
{
    this->scrollDelta = scrollDelta;
}

void Mouse::SetPressStateByButton(Button button, bool press)
{
    if (press && IsPressed(button) || !press && !IsPressed(button)) return;

    if (button == LEFT) leftButtonPressed = press;
    if (button == MIDDLE) middleButtonPressed = press;
    rightButtonPressed = press;
}

bool Mouse::IsPressed(Button button) const
{
    if (button == LEFT) return leftButtonPressed;
    if (button == MIDDLE) return middleButtonPressed;
    return rightButtonPressed;
}

void Mouse::GetDeltas(float& outX, float& outY)
{
    outX = deltaX;
    outY = deltaY;
}

void Mouse::Prepare()
{
    deltaX = 0.0F;
    deltaY = 0.0F;
}
