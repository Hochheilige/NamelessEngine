#include "Game.h"

#include "AssetManager.h"
#include "DisplayWin32.h"
#include "InputDevice.h"
#include "MeshRenderer.h"
#include "ShaderCompiler.h"
#include "AABB2DCollider.h"
#include "RenderingSystem.h"
#include "ImGuiSubsystem.h"
#include "WICTextureLoader.h"
#include "EngineContentRegistry.h"
#include "DirectoryTree.h"


#include <chrono>

#include <iostream>

#include "Actor.h"
#include "Component.h"
#include "RigidBodyComponent.h"
#include "UUIDGenerator.h"


Game* Game::Instance = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Game::GetInstance()->HandleMessage(hwnd, umessage, wparam, lparam);
}

void Game::InitializeInternal()
{
	uuidGenerator = new UUIDGenerator();
	RegisterComponents(GetComponentRegistry());
	StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.0f;

	Display = new DisplayWin32(800, 1200, &WndProc, L"NamelessEngine");
	Input = new InputDevice();

	CreateBackBuffer();

	mEngineContentRegistry = new EngineContentRegistry(this);

	PrepareResources();

	mImGuiSubsystem = new ImGuiSubsystem();
	mImGuiSubsystem->Initialize(this);

	assetManager.reset(new AssetManager());
	assetManager->Initialize();

	Initialize();
}

json Game::Serialize() const
{
	json out = json::object();

	json actorsArr = json::array();
	for (const auto actor : Actors) {
		actorsArr.push_back(actor->Serialize());
	}
	out["actors"] = actorsArr;
	return out;
}

void Game::Deserialize(const json* in)
{
	assert(in->is_object());

	auto actorsArr = in->at("actors");
	assert(actorsArr.is_array());

	for (const json actorObj : actorsArr) {
		assert(actorObj.is_object());


		uuid id = actorObj.at("id").get<uuid>();

		bool exists = false;
		for (const auto actor : Actors) {
			if(actor->GetId() == id) {
				exists = true;
				actor->Deserialize(&actorObj);
			}
		}

		if(!exists) {
			//TODO
		}
	}
}

UUIDGenerator* Game::GetUuidGenerator() const
{
	return this->uuidGenerator;
}

void Game::Initialize()
{
	
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

	res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(BackBuf.ReleaseAndGetAddressOf()));
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
	InitializeInternal();

	auto PrevTime = std::chrono::steady_clock::now();


	MSG msg = {};
	while (!ExitRequested)
	{
		auto now = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - PrevTime).count() / 1000000.0f;
		PrevTime = now;

		// Process input
		Input->Prepare();
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT || ExitRequested)
		{
			Exit();
			break;
		}

		mImGuiSubsystem->NewFrame();

		mImGuiSubsystem->DoLayout();

		// Update
		UpdateInternal(deltaTime);

		// Render
		Render();

		mImGuiSubsystem->EndFrame();
	}
}

void Game::Exit()
{
	ExitRequested = true;
}

LRESULT Game::HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		Exit();
		return 0;
	}
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
	MyRenderingSystem->Draw(0.0f, GetCurrentCamera());

	ID3D11RenderTargetView* views[8] = { RenderTargetView.Get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	Context->OMSetRenderTargets(8, views, nullptr);
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Display->GetClientWidth());
	viewport.Height = static_cast<float>(Display->GetClientHeight());
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	Context->ClearRenderTargetView(RenderTargetView.Get(), Color());
	mImGuiSubsystem->Render();

	SwapChain->Present(1, 0);
}

void Game::DestroyResources()
{
	delete mImGuiSubsystem;
	delete Display; 
	delete Input;
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

Camera* Game::GetCurrentCamera()
{
	if (CurrentCamera == nullptr)
	{
		return &DefaultCamera;
	}

	return CurrentCamera;
}

//void Game::DestroyComponent(GameComponent* GC)
//{
//	if (GC == nullptr)
//	{
//		return;
//	}
//
//	GC->SetParent(nullptr);
//	for (GameComponent* gc : GC->Children)
//	{
//		if (gc != nullptr)
//		{
//			gc->SetParent(nullptr);
//		}
//	}
//
//	GameComponents.erase(remove(GameComponents.begin(), GameComponents.end(), GC), GameComponents.end());
//	
//	if (Collider* col = dynamic_cast<Collider*>(GC))
//	{
//		Colliders.erase(remove(Colliders.begin(), Colliders.end(), col), Colliders.end());
//	}
//	else if (Renderer* rend = dynamic_cast<Renderer*>(GC))
//	{
//		Renderers.erase(remove(Renderers.begin(), Renderers.end(), rend), Renderers.end());
//	}
//
//	delete GC;
//}

int Game::GetScreenHeight() const
{
	return Display->GetClientHeight();
}

int Game::GetScreenWidth() const
{
	return Display->GetClientWidth();
}

void Game::HandleWindowResize(int Width, int Height)
{
	if (Context == nullptr)
	{
		return;
	}

	Context->OMSetRenderTargets(0, 0, 0);

	// Release all outstanding references to the swap chain's buffers.
	RenderTargetView->Release();
	BackBuf->Release();

	SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(BackBuf.GetAddressOf()));
	Device->CreateRenderTargetView(BackBuf.Get(), nullptr, RenderTargetView.GetAddressOf());
}

void Game::CreateNormalMapTextureFromFile(const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView)
{
	DirectX::CreateWICTextureFromFileEx(
		GetD3DDevice().Get(),
		fileName,
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0, 0,
		DirectX::WIC_LOADER_IGNORE_SRGB,
		texture,
		textureView);
}

auto Game::StartPlay() -> void
{
	if (mPlayState == PlayState::Editor)
	{
		mPlayState = PlayState::Playing;
		OnBeginPlay();
	}
}

auto Game::OnBeginPlay() -> void {
	std::cout << "Play has begun";
}

auto Game::PausePlay() -> void
{
	if (mPlayState == PlayState::Playing)
	{
		mPlayState = PlayState::Paused;
	}
}

auto Game::ResumePlay() -> void
{
	if (mPlayState == PlayState::Paused)
	{
		mPlayState = PlayState::Playing;
	}
}

auto Game::StopPlay() -> void
{
	if (mPlayState == PlayState::Playing || mPlayState == PlayState::Paused)
	{
		mPlayState = PlayState::Editor;
	}
}

Game::Game()
{
	Instance = this;
}
