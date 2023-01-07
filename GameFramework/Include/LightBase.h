#pragma once

#include "RenderingSystemTypes.h"
#include "SceneComponent.h"
#include "MonoObjects/MonoPhysicsComponent.h"
#include "EngineContentRegistry.h"
#include "Renderer.h"

class Renderer;

enum class LightType
{
	Ambient = 0,
	Directional, 
	Point,
	Spot
};

//@TODO: update to use different data for different lights?


// @TODO: uncomment and remove cyclic include
class LightBase : public SceneComponent
{
public:

	friend class ImGuiSubsystem;

	LightBase();
	~LightBase();

	// @TODO: should this be virtual? Should I return a reference?
	virtual LightData GetLightData() { return lightData; };

	virtual LightType GetLightType() const = 0;

	virtual Renderer* GetLightRenderer() { return renderer; }

	Renderer* renderer;

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	void SetRenderer(Renderer* InRenderer) { renderer = InRenderer; }

	json Serialize() const
	{
		auto json = SceneComponent::Serialize();
		assert(json.is_object());

		json["color"] = lightData.Color;
		json["Intensity"] = lightData.Intensity;

		return json;
	}

	void Deserialize(const json* in)
	{
		lightData.Color = (in->at("color").get<Vector4>());
		lightData.Intensity = (in->at("Intensity").get<float>());
		

		SceneComponent::Deserialize(in);



	}

private:
	//TODO: POPRAVIT'
	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;
protected:
	LightData lightData = LightData();
};

//@TODO: move this to a separate file
class AmbientLight : public LightBase
{
public:

	LightType GetLightType() const override { return LightType::Ambient; }

	ComponentType GetComponentType() override { return AmbientLightType; };

	virtual LightData GetLightData() 
	{ 
		lightData.Intensity = 0.45f;
		return lightData; 
	};

	static auto Create() -> Component* { return new AmbientLight(); }

	// todo: mb this should be the base implementation?
	virtual auto GetLightRenderer() -> Renderer* override { return EngineContentRegistry::GetInstance()->GetQuadRenderer(); }
};

//@TODO: move this to a separate file
class DirectionalLight : public LightBase
{
public:

	LightType GetLightType() const override { return LightType::Directional; }

	ComponentType GetComponentType() override { return DirectionalLightType; };

	virtual LightData GetLightData() { return lightData; };

	static auto Create() -> Component* { return new DirectionalLight(); }

	virtual auto GetLightRenderer() -> Renderer* override { return EngineContentRegistry::GetInstance()->GetQuadRenderer(); }
};


class PointLight : public LightBase
{
public:

	friend class ImGuiSubsystem;

	PointLight() {
		lightData.Params = Vector3(0.3f, 0.3f, 0.3f);
		lightData.Intensity = 1.0f;
		lightData.Color = color;
	}

	LightType GetLightType() const override { return LightType::Point; }

	ComponentType GetComponentType() override { return LightPointType; };

	virtual LightData GetLightData()
	{ 
		Transform t = GetTransform();
		lightData.Position = Vector4(t.Position.x, t.Position.y, t.Position.z, 1.0f);
		return lightData;
	};

	Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	static auto Create() -> Component* { return new PointLight(); }

	virtual auto GetLightRenderer() -> Renderer* override { 
		Renderer* r = EngineContentRegistry::GetInstance()->GetBoxLightRenderer();
		// todo: calculate size based on intensity
		r->SetRelativeScale(Vector3::One * 50.0f);
		r->SetRelativePosition(GetTransform().Position);
		return r; 
	}

};