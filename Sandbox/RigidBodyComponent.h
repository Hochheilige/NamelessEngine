#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "PhysicsModule.h"
#include "SceneComponent.h"


#include "MonoObjects/MonoPhysicsComponent.h"

#include <memory>

enum class RigidBodyType
{
	STATIC,
	DYNAMIC,
	KINEMATIC
};

enum class RigidBodyUsage
{
	PHYSICS, // NO COLLISIONs
	COLLISIONS, // OVERLAP ALL
	COLLISIONS_AND_PHYSICS // BLOCK ALL
};

enum class CollisionShapeType
{
	BOX,
	SPHERE,
	CAPSULE
};

struct SimulationContactResultCallback : public btCollisionWorld::ContactResultCallback
{

	bool bCollision;
	SimulationContactResultCallback() : bCollision(false) {
		m_closestDistanceThreshold = 0.3;
	}
	
	btScalar addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1)
	{
	//If cp distance less than threshold
		bCollision = cp.getDistance() < m_closestDistanceThreshold ? true : false;
		return bCollision;
	}
};

class RigidBodyComponent : public SceneComponent
{
public:

	friend class ImGuiSubsystem;

	RigidBodyComponent();

	virtual void Init() override;

	virtual ~RigidBodyComponent();

	btRigidBody* GetRigidBody();

	btGhostObject* GetGhostObject() { return rigidBody.Collision; }

	btCollisionShape* GetCollisionShape();

	btScalar GetMass();

	RigidBodyType GetType();

	RigidBodyUsage GetUsage() { return Usage; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	void SetMass(float mass);

	void SetGravity(float gravity);

	void SetRigidBodyType(RigidBodyType type);

	void SetRigidBodyUsage(RigidBodyUsage usage);

	void SetCollisionShapeType(CollisionShapeType type);

	void SetCollisionShape(CollisionShapeType type);

	void SetLinearVelocity(btVector3 velocity);

	void SetGenerateHitEvents(bool generate) { generateHitEvents = generate; }

	void SetGenerateOverlapEvents(bool generate) { generateOverlapEvents = generate; }

	bool GetGenerateHitEvents() { return generateHitEvents; }

	bool GetGenerateOverlapEvents() { return generateOverlapEvents; }

	void RegisterRigidBodyType();

	virtual void Update(float DeltaTime) override;

	void MakeKinematic();

	void MakeDynamic();

	void MakeStatic();

	void CreateShape(Vector3 scale);

	virtual auto SetTransform(const Transform& InTransform, TeleportType InTeleportType) -> void;

	ComponentType GetComponentType() override;

	json Serialize() const override;
	void Deserialize(const json* in) override;

	auto EnablePhysicsSimulation(const bool force = false) -> void;
	auto DisablePhysicsSimulation() -> void;

	static Component* Create()
	{
		return new RigidBodyComponent();
	}

protected:
	std::unique_ptr<btCollisionShape> Shape{};
	btScalar Mass;
	btTransform PhysicsTransform;

	struct RigidBody
	{
		btRigidBody* Body = nullptr;
		btGhostObject* Collision = nullptr;
	} rigidBody;
	
	RigidBodyUsage Usage;
	CollisionShapeType ShapeType;

	MonoPhysicsComponent* mMonoComponent = new MonoPhysicsComponent;

	// todo: do we need this? - we can query type using Body->isKinematicObject(), Body->isStaticObject()
	RigidBodyType rbType;

	bool isPhysicsSimulationEnabled = false;

	bool generateHitEvents = false;
	bool generateOverlapEvents = false;


public:
	// todo: do we need this?
	bool is_kinematic = false;

	auto applyCentralImpulse(const Vector3& impulse) -> void {
		rigidBody.Body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
	}

	auto Reinit() -> void;

	bool in_world = true;	

private:

	auto Deinit() -> void;
};


