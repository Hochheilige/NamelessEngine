#pragma once

#include "RenderingSystemTypes.h"
#include "SceneComponent.h"
#include "MonoObjects/MonoPhysicsComponent.h"

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
	LightBase();
	~LightBase();

	// @TODO: should this be virtual? Should I return a reference?
	virtual LightData GetLightData() { return LightData(); };

	virtual LightType GetLightType() const = 0;

	virtual Renderer* GetLightRenderer() { return renderer; }

	Renderer* renderer;

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	void SetRenderer(Renderer* InRenderer) { renderer = InRenderer; }
private:
	//TODO: POPRAVIT'
	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;
};

//@TODO: move this to a separate file
class AmbientLight : public LightBase
{
public:

	LightType GetLightType() const override { return LightType::Ambient; }

	ComponentType GetComponentType() override { return AmbientLightType; };

	virtual LightData GetLightData() 
	{ 
		LightData light;
		light.Intensity = 0.45f;
		return light; 
	};

	static auto Create() -> Component* { return new AmbientLight(); }
};

//@TODO: move this to a separate file
class DirectionalLight : public LightBase
{
public:

	LightType GetLightType() const override { return LightType::Directional; }

	ComponentType GetComponentType() override { return DirectionalLightType; };

	virtual LightData GetLightData() { return LightData(); };

	static auto Create() -> Component* { return new DirectionalLight(); }
};


class PointLight : public LightBase
{
public:

	LightType GetLightType() const override { return LightType::Point; }

	ComponentType GetComponentType() override { return LightPointType; };

	virtual LightData GetLightData()
	{ 
		LightData light;
		Transform t = GetTransform();
		light.Position = Vector4(t.Position.x, t.Position.y, t.Position.z, 1.0f);
		light.Params = Vector3(0.3f, 0.3f, 0.3f);
		light.Intensity = 1.0f;
		light.Color = color;
		return light;
	};

	Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	static auto Create() -> Component* { return new PointLight(); }
};