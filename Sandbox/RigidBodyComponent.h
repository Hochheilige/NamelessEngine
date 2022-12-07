#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "PhysicsModule.h"
#include "SceneComponent.h"


enum class RigidBodyType
{
	STATIC,
	DYNAMIC,
	KINEMATIC
};

enum class RigidBodyUsage
{
	PHYSICS,
	COLLISIONS,
	COLLISIONS_AND_PHYSICS
};

enum class CollisionShapeType
{
	BOX,
	SPHERE,
	CAPSULE
};

class RigidBodyComponent : public SceneComponent
{
public:

	RigidBodyComponent();

	virtual void Init() override;

	virtual ~RigidBodyComponent();

	btRigidBody* GetRigidBody();

	btCollisionShape* GetCollisionShape();

	btScalar GetMass();

	RigidBodyType GetType();

	void SetMass(float mass);

	void SetGravity(float gravity);

	void SetRigidBodyType(RigidBodyType type);

	void SetRigidBodyUsage(RigidBodyUsage usage);

	void SetCollisionShapeType(CollisionShapeType type);

	void SetCollisionShape(CollisionShapeType type, Vector3 scale);

	void SetLinearVelocity(btVector3 velocity);

	void RegisterRigidBodyType();

	virtual void Update(float DeltaTime) override;

	void MakeKinematic();

	void MakeDynamic();

	void MakeStatic();

	void CreateShape(Vector3 scale);

	void Callback(btDynamicsWorld* world, btScalar timeSleep);

	virtual auto SetTransform(const Transform& InTransform, TeleportType InTeleportType) -> void;

protected:
	btCollisionShape* Shape;
	//btRigidBody* Body;
	btScalar Mass;
	btTransform PhysicsTransform;
	//btGhostObject* Ghost;

	struct RigidBody
	{
		btRigidBody* Body;
		btGhostObject* Collision;
	} rigidBody;
	
	RigidBodyUsage Usage;
	CollisionShapeType ShapeType;

	// todo: do we need this? - we can query type using Body->isKinematicObject(), Body->isStaticObject()
	RigidBodyType rbType;
	RigidBodyType OriginType;

public:
	// todo: do we need this?
	bool is_kinematic = false;
};


