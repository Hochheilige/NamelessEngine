#include "MovementComponent.h"
#include "PhysicsModule.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

MovementComponent::MovementComponent()
{
	Init();
	setGravity();
}

MovementComponent::~MovementComponent()
{
	
	btDynamicsWorld* world = PhysicsModuleData::GetInstance()->GetDynamicsWorls();
	world->removeAction(btController);
	world->removeCollisionObject(ghostObject);

	delete(ghostObject);
	delete(characterCapsule);
	delete(btController);

	//also delete mono component after it's added ?
	//delete(mMonoComponent);
}

auto MovementComponent::Init() -> void
{
	ghostObject = new btPairCachingGhostObject();
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	characterCapsule = new btCapsuleShape(capsuleRadius, 2 * halfHeight);
	ghostObject->setCollisionShape(characterCapsule);
	Transform t = GetTransform();
	auto q = t.Rotation.GetQuaterion();
	ghostObject->setWorldTransform(btTransform(btQuaternion(q.x, q.y, q.z, q.w), 
		btVector3(t.Position.x, t.Position.y, t.Position.z)));
	btDynamicsWorld* collisionWorld = PhysicsModuleData::GetInstance()->GetDynamicsWorls();
	collisionWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, 
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	btController = new btKinematicCharacterController(ghostObject, characterCapsule, stepHeight, btVector3(up.x, up.y, up.z));
	collisionWorld->addAction(btController);
}

auto MovementComponent::Update(float deltaTime) -> void {
	btTransform bttr = ghostObject->getWorldTransform();
	Transform tr = GetTransform();
	tr.Position = { bttr.getOrigin().x(),bttr.getOrigin().y(), bttr.getOrigin().z() };
	tr.Rotation = Quaternion(bttr.getRotation());
	SetTransform(tr);
}

///btActionInterface interface

auto MovementComponent::updateAction(btScalar deltaTime) -> void
{
	btCollisionWorld* collisionWorld = PhysicsModuleData::GetInstance()->GetDynamicsWorls();
	btController->updateAction(collisionWorld, deltaTime);
}

inline void MovementComponent::setUp(const btVector3& up)
{
	btController->setUp(up);
}

void MovementComponent::reset()
{ 
	btController->reset(PhysicsModuleData::GetInstance()->GetDynamicsWorls());
}

inline void MovementComponent::preStep() {
	btController->preStep(PhysicsModuleData::GetInstance()->GetDynamicsWorls());
}

inline void MovementComponent::playerStep(btScalar dt) {
	btController->playerStep(PhysicsModuleData::GetInstance()->GetDynamicsWorls(), dt);
}




