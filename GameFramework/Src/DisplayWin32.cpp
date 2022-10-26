#include "DisplayWin32.h"

DisplayWin32::DisplayWin32(LONG ClientHeight, LONG ClientWidth, WNDPROC WndProc, LPCWSTR ApplicationName) : ClientHeight(ClientHeight), ClientWidth(ClientWidth)
{
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = ApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class
	RegisterClassEx(&wc);

	RECT windowRect = { 0, 0, ClientWidth, ClientHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - ClientWidth) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - ClientHeight) / 2;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, ApplicationName, ApplicationName,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	
	ShowCursor(true);
}

HWND DisplayWin32::GetWindowHandle()
{
	return hWnd;
}

float DisplayWin32::GetAspectRatio() const
{
    return static_cast<float>(ClientWidth) / static_cast<float>(ClientHeight);
}

LONG DisplayWin32::GetClientHeight() const
{
	return ClientHeight;
}

LONG DisplayWin32::GetClientWidth() const
{
	return ClientWidth;
}
