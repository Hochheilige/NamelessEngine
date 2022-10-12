#pragma once

#include <wrl/client.h>

#include <d3d.h>
#include <d3d11.h>
#include "MathInclude.h"
#include <DirectXMath.h>

#include <vector>

#include "Camera.h"

using namespace Microsoft::WRL;

class Graphics
{
public:

	void Initialize();

	static Graphics* GetInstance();
	 
	ComPtr<ID3D11Device>           GetDevice();
	ComPtr<ID3D11DeviceContext>    GetContext();
	ComPtr<IDXGISwapChain>         GetSwapChain();
	ComPtr<ID3D11Texture2D>        GetBackBuffer();
	ComPtr<ID3D11RenderTargetView> GetRTV();
	ComPtr<ID3D11DepthStencilView> GetDSV();

	class DisplayWin32* GetDisplay();
	class InputDevice*  GetInputDevice();

	int GetScreenHeight() const;
	int GetScreenWidth()  const;

	LRESULT HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	//ComPtr<ID3D11Buffer> GetPerObjectConstantBuffer();

	const Camera& GetCurrentCamera();
	void SetCurrentCamera(const Camera& camera);
	
private:

	Graphics() = default;
	Graphics(const Graphics&);
	Graphics& operator=(Graphics&);

	ComPtr<ID3D11Device>           Device;
	ComPtr<ID3D11DeviceContext>    Context;
	ComPtr<IDXGISwapChain>         SwapChain;
	ComPtr<ID3D11Texture2D>        BackBuffer;
	ComPtr<ID3D11RenderTargetView> RenderTargetView;
	ComPtr<ID3D11Texture2D>        DepthStencil;
	ComPtr<ID3D11DepthStencilView> DepthStencilView;

	class DisplayWin32* Display = nullptr;
	class InputDevice*  Input   = nullptr;

	Camera* CurrentCamera = nullptr;
	Camera DefaultCamera = Camera();

	// Not sure about it
	ComPtr<ID3D11Buffer> PerDrawCB;
	ComPtr<ID3D11Buffer> PerObjectCB;

	static Graphics* Instance;
};
