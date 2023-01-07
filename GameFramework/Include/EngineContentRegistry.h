#pragma once

#include <wrl/client.h>

#include <map>
#include <string>
#include <vector>

class Actor;
class Game;
class RenderPrimitiveProxy;
class VertexShader;
class PixelShader;
class QuadRenderer;
class MeshRenderer;
class Renderer;

struct Transform;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;

using namespace Microsoft::WRL;

template<class T>
class Singleton
{
public:
	static auto GetInstance() -> T* { return Instance; }

protected:
	Singleton()
	{
		Instance = static_cast<T*>(this);
	}

	static T* Instance;
};

template<class T>
T* Singleton<T>::Instance = nullptr;

class EngineContentRegistry : public Singleton<EngineContentRegistry>
{
public:

	EngineContentRegistry(Game* InGame);
	~EngineContentRegistry();

	auto CreateBasicActor(const std::string& BasicActorName, const Transform& transform)->Actor*;

	auto CreateBox(const Transform& transform) -> Actor*;
	auto CreatePointLight(const Transform& transform)->Actor*;
	auto CreateSphere(const Transform& transform)->Actor*;

	auto CreateNormalMapTextureFromFile(const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView) -> void;

	auto GetWhiteTexSRV()->ComPtr<ID3D11ShaderResourceView> { return WhiteTexSRV; }
	auto GetBasicNormalTexSRV()->ComPtr<ID3D11ShaderResourceView> { return BasicNormalTexSRV; }

	auto GetFolderTexSRV()->ComPtr<ID3D11ShaderResourceView> { return FolderTexSRV; }
	auto GetGenericFileTexSRV()->ComPtr<ID3D11ShaderResourceView> { return GenericFileTexSRV; }
	auto GetAssetColTexSRV()->ComPtr<ID3D11ShaderResourceView> { return AssetColTexSRV; }

	auto GetBasicActorNames() const-> const std::vector<std::string>& { return BasicActorNames; }

	auto GetDefaultPixelShader() const -> PixelShader* {
		return DefaultPixelShader;
	}

	auto GetDefaultVertexShader() const -> VertexShader* {
		return DefaultVertexShader;
	}

	auto GetPosColorVertexShader() const -> VertexShader*
	{
		return PosColorVertexShader;
	}

	auto GetPosColorPixelShader() const -> PixelShader*
	{
		return PosColorPixelShader;
	}

	auto GetQuadRenderer() const -> QuadRenderer*
	{
		return quadRenderer;
	}

	auto GetBoxLightRenderer() const->Renderer*;

private:

	typedef Actor* (EngineContentRegistry::*CreateFuncType)(const Transform& transform);
	std::map<std::string, CreateFuncType> CreateActionsMap;
	std::vector<std::string> BasicActorNames;

private:

	Game* MyGame;

	ComPtr<ID3D11Resource> WhiteTex;
	ComPtr<ID3D11ShaderResourceView> WhiteTexSRV;

	ComPtr<ID3D11Resource> BasicNormalTex;
	ComPtr<ID3D11ShaderResourceView> BasicNormalTexSRV;

	ComPtr<ID3D11Resource> FolderTex;
	ComPtr<ID3D11ShaderResourceView> FolderTexSRV;

	ComPtr<ID3D11Resource> GenericFileTex;
	ComPtr<ID3D11ShaderResourceView>GenericFileTexSRV;

	ComPtr<ID3D11Resource> AssetColTex;
	ComPtr<ID3D11ShaderResourceView> AssetColTexSRV;

	RenderPrimitiveProxy* TexturedBoxMeshProxy;
	RenderPrimitiveProxy* SphereMeshProxy;

	VertexShader* DefaultVertexShader;
	PixelShader* DefaultPixelShader;

	VertexShader* PosColorVertexShader;
	PixelShader* PosColorPixelShader;

	QuadRenderer* quadRenderer;
	MeshRenderer* boxRenderer;
};