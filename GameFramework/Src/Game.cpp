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



void drawDockspace(bool temp);

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
	Initialize();

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

		if (msg.message == WM_QUIT)
		{
			Exit();
			break;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//temp
		bool temp = true;
		
		drawDockspace(temp);
		ImGui::ShowDemoWindow(&temp);
		ImGui::Begin("Viewport");
		const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		MyRenderingSystem->HandleScreenResize({ viewportSize.x, viewportSize.y });
		ImGui::Image(MyRenderingSystem->GetViewportTextureID(), viewportSize);
		ImGui::End();


		// Update
		UpdateInternal(deltaTime);

		// Render
		Render();

		// needs to be called after imgui::Render
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
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
	MyRenderingSystem->Draw(0.0f, GetCurrentCamera());

	ImGui::Render();
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
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	SwapChain->Present(1, 0);
}

void Game::DestroyResources()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

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

Game::Game()
{
	Instance = this;
}

void Game::InitializeInternal()
{
	
	Initialize();
}


void drawDockspace(bool temp) {


	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
	// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
	// In this specific demo, we are not using DockSpaceOverViewport() because:
	// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
	// - we allow the host window to have padding (when opt_padding == true)
	// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
	// TL;DR; this demo is more complicated than what you would normally use.
	// If we removed all the options we are showcasing, this demo would become:
	//     void ShowExampleAppDockSpace()
	//     {
	//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	//     }

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static bool dockspaceOpen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, dockspaceOpen != NULL))
				dockspaceOpen = false;
			ImGui::EndMenu();
		}
		

		ImGui::EndMenuBar();
	}



	ImGui::End();
}
