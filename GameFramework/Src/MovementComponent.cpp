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
	
	btDynamicsWorld* world = PhysicsModuleData::GetInstance()->GetDynamicsWorld();
	world->removeAction(btController);
	world->removeCollisionObject(ghostObject);

	PhysicsModuleData::GetInstance()->RemoveGhostObject(ghostObject);

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
	characterCapsule = new btCapsuleShape(capsuleRadius, 2 * halfHeight - 2 * capsuleRadius);
	ghostObject->setCollisionShape(characterCapsule);
	Transform t = GetTransform();
	auto q = t.Rotation.GetQuaterion();
	btDynamicsWorld* collisionWorld = PhysicsModuleData::GetInstance()->GetDynamicsWorld();
	collisionWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, 
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	btController = new btKinematicCharacterController(ghostObject, characterCapsule, stepHeight, btVector3(up.x, up.y, up.z));
	collisionWorld->addAction(btController);

	btController->getGhostObject()->setWorldTransform(btTransform(btQuaternion(q.x, q.y, q.z, q.w),
		btVector3(t.Position.x, t.Position.y, t.Position.z)));

	collisionWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback);
	PhysicsModuleData::GetInstance()->AddGhostObject(ghostObject);
	ghostObject->setUserPointer(this);
}

auto MovementComponent::Update(float deltaTime) -> void {
	// attempt to move along the NavPath
	if (bIsFollowingNavPath)
	{
		float distanceMoved = 0.0f;
		const float shouldMoveDistance = navPathMovementSpeed * deltaTime;
		
		Transform t = GetTransform();
		while (distanceMoved < shouldMoveDistance && floor(curSmoothPathPosIndex) < navPath.m_nsmoothPath - 1)
		{
			size_t currIndex = static_cast<size_t>(floor(curSmoothPathPosIndex));
			size_t nextIndex = static_cast<size_t>(ceil(curSmoothPathPosIndex));
			if (nextIndex == currIndex) ++nextIndex;
			double alpha = curSmoothPathPosIndex - currIndex;

			float* currPathPosPtr = navPath.m_smoothPath + currIndex * 3;
			float* nextPathPosPtr = navPath.m_smoothPath + nextIndex * 3;

			Vector3 cpp{ currPathPosPtr[0], currPathPosPtr[1], currPathPosPtr[2] };
			Vector3 npp{ nextPathPosPtr[0], nextPathPosPtr[1], nextPathPosPtr[2] };

			Vector3 currentPos = Vector3::Lerp(cpp, npp, alpha);

			const float segmentLength = (npp - cpp).Length();
			if (segmentLength == 0.0f)
			{
				curSmoothPathPosIndex = nextIndex;
				continue;
			}
			const float segmentTravelledDist = segmentLength * alpha;
			const float segmentLeftDist = segmentLength - segmentTravelledDist;

			float distToMoveThisStep = segmentLeftDist;
			if (distToMoveThisStep > shouldMoveDistance - distanceMoved)
			{
				distToMoveThisStep = shouldMoveDistance - distanceMoved;
			}

			distanceMoved += distToMoveThisStep;
			alpha = (segmentTravelledDist + distToMoveThisStep) / segmentLength;
			t.Position = Vector3::Lerp(cpp, npp, alpha) + Vector3(0.0f, yDelta, 0.0f);

			if (abs(alpha - 1.0f) <= 0.01f)
			{
				curSmoothPathPosIndex = nextIndex;
			}
			else
			{
				curSmoothPathPosIndex = currIndex + alpha;
			}
		}

		auto q = t.Rotation.GetQuaterion();

		btController->getGhostObject()->setWorldTransform(btTransform(btQuaternion(q.x, q.y, q.z, q.w),
			btVector3(t.Position.x, t.Position.y, t.Position.z)));

		SceneComponent::SetTransform(t);

		if (curSmoothPathPosIndex >= navPath.m_nsmoothPath - 1)
		{
			bIsFollowingNavPath = false;
		}
	}
	else
	{
		btTransform bttr = ghostObject->getWorldTransform();
		Transform tr = GetTransform();
		tr.Position = { bttr.getOrigin().x(),bttr.getOrigin().y(), bttr.getOrigin().z() };
		tr.Rotation = Quaternion(bttr.getRotation());
		SceneComponent::SetTransform(tr);
	}
}

auto MovementComponent::TryMoveTo(const Vector3& Position) -> bool
{
	if (RecastNavigationManager::GetInstance()->FindPath(GetTransform().Position, Position, navPath))
	{
		bIsFollowingNavPath = true;
		curSmoothPathPosIndex = 0.0;
		yDelta = abs(GetTransform().Position.y - navPath.m_smoothPath[1]);
		if (yDelta > halfHeight)
			yDelta = halfHeight;
		return true;
	}
	return false;
}

auto MovementComponent::StopMoveTo() -> void
{
	bIsFollowingNavPath = false;
}

///btActionInterface interface

auto MovementComponent::updateAction(btScalar deltaTime) -> void
{
	btCollisionWorld* collisionWorld = PhysicsModuleData::GetInstance()->GetDynamicsWorld();
	btController->updateAction(collisionWorld, deltaTime);
}

inline void MovementComponent::setUp(const btVector3& up)
{
	btController->setUp(up);
}

void MovementComponent::reset()
{ 
	btController->reset(PhysicsModuleData::GetInstance()->GetDynamicsWorld());
}

inline void MovementComponent::preStep() {
	btController->preStep(PhysicsModuleData::GetInstance()->GetDynamicsWorld());
}

inline void MovementComponent::playerStep(btScalar dt) {
	btController->playerStep(PhysicsModuleData::GetInstance()->GetDynamicsWorld(), dt);
}

auto MovementComponent::SetTransform(const Transform& InTransform, TeleportType InTeleportType) -> void
{
	SceneComponent::SetTransform(InTransform, InTeleportType);

	Transform t = GetTransform();
	auto q = t.Rotation.GetQuaterion();

	btController->getGhostObject()->setWorldTransform(btTransform(btQuaternion(q.x, q.y, q.z, q.w),
		btVector3(t.Position.x, t.Position.y, t.Position.z)));

	if (InTeleportType == TeleportType::ResetPhysics)
	{
		reset();
	}
}

void MovementComponent::OnDeserializationCompleted()
{
	SetTransform(GetTransform(), TeleportType::ResetPhysics);
}
