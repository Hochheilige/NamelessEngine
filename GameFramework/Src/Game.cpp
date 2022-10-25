#include "Game.h"

#include "DisplayWin32.h"
#include "InputDevice.h"
#include "MeshRenderer.h"
#include "ShaderCompiler.h"
#include "AABB2DCollider.h"
#include "RenderingSystem.h"

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#include <chrono>

Game* Game::Instance = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Game::GetInstance()->HandleMessage(hwnd, umessage, wparam, lparam);
}

void Game::Initialize()
{
	StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.0f;

	Display = new DisplayWin32(800, 1200, &WndProc, L"NamelessEngine");
	Input = new InputDevice();

	CreateBackBuffer();

	PrepareResources();
	

	// Init imGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(Display->GetWindowHandle());
	ImGui_ImplDX11_Init(GetD3DDevice().Get(), GetD3DDeviceContext().Get());
}

void Game::CreateBackBuffer()
{
	D3D_FEATURE_LEVEL featureLevel[]{ D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display->GetClientWidth();
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

	res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(BackBuf.GetAddressOf()));
	res = Device->CreateRenderTargetView(BackBuf.Get(), nullptr, RenderTargetView.GetAddressOf());

	// Step 11. Set back buffer for output
	

	// todo: move to mesh material?
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rastState;
	res = Device->CreateRasterizerState(&rastDesc, &rastState);

	Context->RSSetState(rastState);

	//Create per draw constant buffer.
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

	// Create lights buffer
	cbDesc.ByteWidth = sizeof(CBLights);
	Device->CreateBuffer(&cbDesc, nullptr, &LightsCB);

	Context->VSSetConstantBuffers(1, 1, LightsCB.GetAddressOf());
	Context->PSSetConstantBuffers(1, 1, LightsCB.GetAddressOf());

	
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
	res = Device->CreateTexture2D(&descDepth, NULL, pDepthStencil.GetAddressOf());

	// Create the depth stencil view

	res = Device->CreateDepthStencilView(pDepthStencil.Get(), // Depth stencil texture
		nullptr, // Depth stencil desc
		DepthStencilView.GetAddressOf());  // [out] Depth stencil view


	// Create default sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	GetD3DDevice()->CreateSamplerState(&sampDesc, DefaultSamplerState.GetAddressOf());

	// Create shadowmap sampler state
	D3D11_SAMPLER_DESC smSampDesc;
	ZeroMemory(&smSampDesc, sizeof(smSampDesc));
	smSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	smSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	smSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	smSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	smSampDesc.BorderColor[0] = 1.0f;
	smSampDesc.BorderColor[1] = 1.0f;
	smSampDesc.BorderColor[2] = 1.0f;
	smSampDesc.BorderColor[3] = 1.0f;
	smSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	smSampDesc.MinLOD = 0;
	smSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	GetD3DDevice()->CreateSamplerState(&smSampDesc, ShadowmapSamplerState.GetAddressOf());
	
	// Create shadowmap texture
	D3D11_TEXTURE2D_DESC descSM;
	descSM.Width = 2048;
	descSM.Height = 2048;
	descSM.MipLevels = 1;
	descSM.ArraySize = 1;
	descSM.Format = DXGI_FORMAT_R32_TYPELESS;
	descSM.SampleDesc.Count = 1;
	descSM.SampleDesc.Quality = 0;
	descSM.Usage = D3D11_USAGE_DEFAULT;
	descSM.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descSM.CPUAccessFlags = 0;
	descSM.MiscFlags = 0;
	res = Device->CreateTexture2D(&descSM, NULL, ShadowMapTex.GetAddressOf());

	// Create the depth stencil view for shadowmap
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	res = Device->CreateDepthStencilView(ShadowMapTex.Get(), &descDSV, ShadowMapView.GetAddressOf());  

	D3D11_SHADER_RESOURCE_VIEW_DESC smSRVDesc;
	//ZeroMemory(&smSRVDesc, sizeof(smSRVDesc));
	smSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	smSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	smSRVDesc.Texture2D.MostDetailedMip = 0;
	smSRVDesc.Texture2D.MipLevels = descSM.MipLevels;
	Device->CreateShaderResourceView(ShadowMapTex.Get(), &smSRVDesc, ShadowMapSRV.GetAddressOf());

	// @TODO: move this to run?
	MyRenderingSystem = new RenderingSystem(this);
}

void Game::Run()
{
	Initialize();

	auto PrevTime = std::chrono::steady_clock::now();


	MSG msg = {};
	while (!ExitRequested)
	{
		auto now = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - PrevTime).count() / 1000000.0f;
		PrevTime = now;

		// Process input
		Input->PreInputProcess();
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			Exit();
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//temp
		bool temp = true;
		ImGui::ShowDemoWindow(&temp);

		// Update
		UpdateInternal(deltaTime);

		// Render
		Render();

		// needs to be called after imgui::Render
		ImGuiIO& io = ImGui::GetIO();
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}

void Game::Exit()
{
	ExitRequested = true;
}

LRESULT Game::HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Input->HandleMessage(hwnd, umessage, wparam, lparam);
}

void Game::PrepareResources()
{

}

ComPtr<ID3D11Device> Game::GetD3DDevice()
{
	return Device;
}

ComPtr<ID3D11DeviceContext> Game::GetD3DDeviceContext()
{
	return Context;
}

ComPtr<ID3D11Buffer> Game::GetPerObjectConstantBuffer()
{
	return PerObjectCB;
}

void Game::UpdateInternal(float DeltaTime)
{
	for (GameComponent* gc : GameComponents)
	{
		if (gc != nullptr && gc->bShouldUpdate)
		{
			gc->Update(DeltaTime);
		}
	}
	
	Update(DeltaTime);
}

void Game::Update(float DeltaTime)
{
	
}

void Game::Render()
{
	MyRenderingSystem->Draw(0.0f, &GetCurrentCamera());

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	SwapChain->Present(1, 0);
}

void Game::DestroyResources()
{
	delete Display; 
	delete Input;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

InputDevice* Game::GetInputDevice()
{
	return Input;
}

DisplayWin32* Game::GetDisplay()
{
	return Display;
}

Collider* Game::GetOverlapping(const Collider* Col)
{
	if (Col == nullptr)
	{
		return nullptr;
	}
	for (Collider* c : Colliders)
	{
		if (c == Col)
		{
			continue;
		}
		if (Col->DoesOverlap(*c))
		{
			return c;
		}
	}
	return nullptr;
}

Game* Game::GetInstance()
{
	return Instance;
}

float Game::GetTotalElapsedTime()
{
	float now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000000.0f;

	return now - StartTime;
}

Game::~Game()
{
	for (GameComponent* gc : GameComponents)
	{
		delete gc;
	}
	Context->Flush();
}

const Camera& Game::GetCurrentCamera()
{
	if (CurrentCamera == nullptr)
	{
		return DefaultCamera;
	}

	return *CurrentCamera;
}

void Game::DestroyComponent(GameComponent* GC)
{
	if (GC == nullptr)
	{
		return;
	}

	GC->SetParent(nullptr);
	for (GameComponent* gc : GC->Children)
	{
		if (gc != nullptr)
		{
			gc->SetParent(nullptr);
		}
	}

	GameComponents.erase(remove(GameComponents.begin(), GameComponents.end(), GC), GameComponents.end());
	
	if (Collider* col = dynamic_cast<Collider*>(GC))
	{
		Colliders.erase(remove(Colliders.begin(), Colliders.end(), col), Colliders.end());
	}
	else if (Renderer* rend = dynamic_cast<Renderer*>(GC))
	{
		Renderers.erase(remove(Renderers.begin(), Renderers.end(), rend), Renderers.end());
	}

	delete GC;
}

int Game::GetScreenHeight() const
{
	return Display->GetClientHeight();
}

int Game::GetScreenWidth() const
{
	return Display->GetClientWidth();
}

Game::Game()
{
	Instance = this;
}

void Game::InitializeInternal()
{
	
	Initialize();
}
