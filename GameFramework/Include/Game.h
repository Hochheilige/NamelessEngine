#pragma once

#include <wrl/client.h>

#include <d3d.h>
#include <d3d11.h>
#include "MathInclude.h"

#include <vector>

#include "Camera.h"

#include "EditorContext.h"

#include <filesystem>

#include "ComponentRegistry.h"
#include "JsonInclude.h"
#include "UUIDGenerator.h"
#include "JsonSerializers.h"



using Path = std::filesystem::path;

class Actor;
class AssetManager;
class RenderingSystem;
class ImGuiSubsystem;
class EngineContentRegistry;
class Serializer;
class CameraComponent;
class RecastNavigationManager;

using namespace Microsoft::WRL;

// TODO: try to remove everything apart from game from this header to include it in less files
#pragma pack(push, 4)
struct DirLight
{
	Vector3 direction = Vector3(0.0f, -1.0f, 0.0f);
	float intensity = 1.0f;
	Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Matrix WorldToLightClip;
};
#pragma pack(pop)

enum class PlayState : uint8_t
{
	Editor,
	Playing,
	Paused
};

class Game
{

public:
	friend int main();
	friend class Actor;
	friend ImGuiSubsystem;
	template<class T>
	friend auto CreateActor()->T*;

	void AddPendingFunction(std::function<void()>&& func);
	
	// Called after the base sysyems have been initialized
	virtual void Initialize();

	void Run();

	void CreateBackBuffer();

	void Exit();

	LRESULT HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	virtual void PrepareResources();

	ComPtr<ID3D11Device> GetD3DDevice();

	ComPtr<ID3D11DeviceContext> GetD3DDeviceContext();

	ComPtr<ID3D11Buffer> GetPerObjectConstantBuffer();

	void UpdateInternal(float DeltaTime);
	virtual void Update(float DeltaTime);
	

	void Render();
	void SetDebugRender(bool doDebug) { doDebugRender = doDebug; }

	virtual void DestroyResources();

	class InputDevice* GetInputDevice();
	class DisplayWin32* GetDisplay();

	class Collider* GetOverlapping(const class Collider* Col);
	
	template<class T>
	T* CreateGameComponent(class GameComponent* Parent = nullptr)
	{
		T* comp = new T();

		GameComponents.push_back(comp);

		comp->SetParent(Parent);

		//if constexpr (std::is_base_of<class Renderer, T>::value)
		//{
		//	MyRenderingSystem->RegisterRenderer(comp);
		//}
		//else if constexpr (std::is_base_of<class Collider, T>::value)
		//{
		//	Colliders.push_back(comp);
		//}

		return comp;
	}

	/*
	
	void EndFrame();
	
	
	
	
	void PrepareFrame();
	
	void RestoreTargets();
	
	
	*/

	auto CreateCustomActor(const char* nameSpace, const char* className) -> Actor*;

	static Game* GetInstance();

	float GetTotalElapsedTime();

	virtual ~Game();

	const Camera* GetCurrentPOV() const;

	auto GetPlayerCamera() const -> CameraComponent* { return PlayerCamera; }
	auto SetPlayerCamera(CameraComponent* Cam) -> void;


	ComPtr<ID3D11SamplerState> GetDefaultSamplerState() { return DefaultSamplerState; }

	//void DestroyComponent(GameComponent* GC);

	DirLight DirectiLight;
	Camera LightCam;

	ComPtr<ID3D11ShaderResourceView> GetShadowMapSRV() { return ShadowMapSRV; }
	ComPtr<ID3D11SamplerState> GetShadowmapSamplerState() { return ShadowmapSamplerState; }

	bool bIsRenderingShadowMap = false;

	int GetScreenHeight() const;
	int GetScreenWidth() const;

	ComPtr<ID3D11RenderTargetView> RenderTargetView;
	ComPtr<ID3D11Texture2D> ShadowMapTex = nullptr;
	ComPtr<ID3D11DepthStencilView> ShadowMapView = nullptr;
	ComPtr<ID3D11ShaderResourceView> ShadowMapSRV = nullptr;

	RenderingSystem* MyRenderingSystem = nullptr;


	void HandleWindowResize(int Width, int Height);

	EditorContext MyEditorContext;

	void CreateNormalMapTextureFromFile(const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);

	auto GetPlayState() const -> PlayState { return mPlayState; }

	// TODO: make propter implementations of these functions
	/*auto StartPlay() -> void;
	auto PausePlay() -> void;
	auto ResumePlay() -> void;
	auto StopPlay() -> void;*/

	virtual auto OnBeginPlay() -> void;
	virtual auto StartPlay() -> void;
	virtual auto PausePlay() -> void;
	virtual auto ResumePlay() -> void;
	virtual auto StopPlay() -> void;

	json Serialize() const;
	void Deserialize(const json* in, bool destructive = false);
	UUIDGenerator* GetUuidGenerator() const;

	auto GetImGuiSubsystem() const -> ImGuiSubsystem* { return mImGuiSubsystem; }
protected:

	uint32_t csGameInstance;

	Game();

	class DirectionalLight* dr;
	class AmbientLight* al;

	std::vector<class Renderer*> Renderers;
	std::vector<class Collider*> Colliders;

	std::vector<class GameComponent*> GameComponents;

	CameraComponent* PlayerCamera = nullptr;
	Camera EditorPOV = Camera();

	class DisplayWin32* Display = nullptr;

	ComPtr<ID3D11DepthStencilState> pDSState = nullptr;

	ComPtr<ID3D11SamplerState> DefaultSamplerState = nullptr;
	ComPtr<ID3D11SamplerState> ShadowmapSamplerState = nullptr;


	std::vector<Actor*> Actors;


private:

	void InitializeInternal();

private:
	
	class InputDevice* Input = nullptr;

	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> Context;
	ComPtr<IDXGISwapChain> SwapChain;

	ComPtr<ID3D11Texture2D> BackBuf;


	ComPtr<ID3D11Buffer> PerDrawCB;
	ComPtr<ID3D11Buffer> PerObjectCB;
	ComPtr<ID3D11Buffer> LightsCB;


	bool ExitRequested = false;

	std::vector<std::function<void()>> pendingFunctions; 

	static Game* Instance;


	float StartTime = 0.0f;

	Camera DefaultPOV = Camera();
	
	ImGuiSubsystem* mImGuiSubsystem = nullptr;

	EngineContentRegistry* mEngineContentRegistry = nullptr;

	PlayState mPlayState = PlayState::Editor;

	UUIDGenerator* uuidGenerator = nullptr;
	std::unique_ptr<AssetManager> assetManager;

	std::unique_ptr<RecastNavigationManager> recastNavigationManager;

private:
	json tempGameSave;


	bool bUseEditorCamera = true;
	bool doDebugRender = false;
	
	// applies only to playing state
	bool bShowMouseCursor = false;

public:
	auto GetAssetManager() const -> AssetManager* { return assetManager.get(); }

	auto SetUseEditorCamera(const bool InUseEditorCamera) -> void;
	auto GetUseEditorCamera() const -> bool { return bUseEditorCamera; }

	auto UpdateCamerasAspectRatio(float NewAspectRatio) -> void;

	auto GetRecastNavigationManager() const -> RecastNavigationManager* { return recastNavigationManager.get(); }

	auto LoadGameFacade() -> void;

	auto GetTasksJson() const -> json;

	// applies only to playing state
	auto GetShowMouseCursor() const -> bool { return bShowMouseCursor || mPlayState != PlayState::Playing; }

	auto SetShowMouseCursor(bool InShow) -> void { bShowMouseCursor = InShow; }
};

