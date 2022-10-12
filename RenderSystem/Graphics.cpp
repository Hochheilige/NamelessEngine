#include "Graphics.h"

#include "DisplayWin32.h"
#include "InputDevice.h"
#include "MeshRenderer.h"
#include "ShaderCompiler.h"
#include "AABB2DCollider.h"
#include "RenderingSystem.h"

#include <chrono>

Graphics* Graphics::Instance = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Graphics::GetInstance()->HandleMessage(hwnd, umessage, wparam, lparam);
}

void Graphics::Initialize()
{
	Display = new DisplayWin32(800, 1200, &WndProc, L"NamelessEngine");
	Input   = new InputDevice();

	D3D_FEATURE_LEVEL featureLevel[]{ D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width  = Display->GetClientWidth();
	swapDesc.BufferDesc.Height = Display->GetClientHeight();
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->GetWindowHandle();
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		SwapChain.GetAddressOf(),
		Device.GetAddressOf(),
		nullptr,
		Context.GetAddressOf());

	res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(BackBuffer.GetAddressOf()));
	res = Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, RenderTargetView.GetAddressOf());

	// Create per draw CB
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(CBPerDraw);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	res = Device->CreateBuffer(&cbDesc, nullptr,
		PerDrawCB.GetAddressOf());

	Context->VSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());
	Context->PSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());

	//Create per object constant buffer.
	cbDesc.ByteWidth = sizeof(CBPerObject);
	res = Device->CreateBuffer(&cbDesc, nullptr,
		PerObjectCB.GetAddressOf());

	Context->VSSetConstantBuffers(2, 1, PerObjectCB.GetAddressOf());
	Context->PSSetConstantBuffers(2, 1, PerObjectCB.GetAddressOf());

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = Display->GetClientWidth();
	descDepth.Height = Display->GetClientHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	res = Device->CreateTexture2D(&descDepth, NULL, DepthStencil.GetAddressOf());

	// Create the depth stencil view

	res = Device->CreateDepthStencilView(DepthStencil.Get(), // Depth stencil texture
		nullptr, // Depth stencil desc
		DepthStencilView.GetAddressOf());

}

Graphics* Graphics::GetInstance()
{
	if (!Instance)
	{
		Instance = new Graphics();
	}
	return Instance;
}

ComPtr<ID3D11Device> Graphics::GetDevice()
{
	return Device;
}

ComPtr<ID3D11DeviceContext> Graphics::GetContext()
{
	return Context;
}

ComPtr<IDXGISwapChain> Graphics::GetSwapChain()
{
	return SwapChain;
}

ComPtr<ID3D11Texture2D> Graphics::GetBackBuffer()
{
	return BackBuffer;
}

ComPtr<ID3D11RenderTargetView> Graphics::GetRTV()
{
	return RenderTargetView;
}

ComPtr<ID3D11DepthStencilView> Graphics::GetDSV()
{
	return DepthStencilView;
}

DisplayWin32* Graphics::GetDisplay()
{
	return Display;
}

InputDevice* Graphics::GetInputDevice()
{
	return Input;
}

int Graphics::GetScreenHeight() const
{
	return Display->GetClientHeight();
}

int Graphics::GetScreenWidth() const
{
	return Display->GetClientWidth();
}

LRESULT Graphics::HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Input->HandleMessage(hwnd, umessage, wparam, lparam);
}

const Camera& Graphics::GetCurrentCamera()
{
	if (CurrentCamera == nullptr)
	{
		return DefaultCamera;
	}
	return *CurrentCamera;
}

void Graphics::SetCurrentCamera(const Camera& camera)
{
	if (CurrentCamera == nullptr)
	{
		CurrentCamera = new Camera(camera);
	} 
	else
	{
		*CurrentCamera = camera;
	}

}

//ComPtr<ID3D11Buffer> Game::GetPerObjectConstantBuffer()
//{
//	return PerObjectCB;
//}