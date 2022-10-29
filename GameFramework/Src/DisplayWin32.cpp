#include "DisplayWin32.h"

DisplayWin32::DisplayWin32(LONG ClientHeight, LONG ClientWidth, WNDPROC WndProc, LPCWSTR ApplicationName) : ClientHeight(ClientHeight), ClientWidth(ClientWidth)
{
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;
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

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_OVERLAPPEDWINDOW;

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
	int width;
	int height;
	GetClientSize(width, height);
	return height;
}

LONG DisplayWin32::GetClientWidth() const
{
	int width;
	int height;
	GetClientSize(width, height);
	return width;
}

void DisplayWin32::GetClientSize(int& OutWidth, int& OutHeight) const
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	OutWidth = rect.right;
	OutHeight = rect.bottom;
}
