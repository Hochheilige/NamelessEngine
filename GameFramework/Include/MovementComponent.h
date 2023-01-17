#pragma once
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "SceneComponent.h"
#include "btBulletDynamicsCommon.h"
#include "MonoMovementComponent.h"

#include "RecastNavigationManager.h"

class btGhostObject;
class btPairCachingGhostObject;

class MovementComponent : public SceneComponent {
public:

	MovementComponent();
	~MovementComponent();

	ComponentType GetComponentType() override { return MovementComponentType; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	auto Init() -> void;

	auto Update(float deltaTime) -> void override;

	btKinematicCharacterController* btController;

	///btActionInterface interface
	auto updateAction(btScalar deltaTime) -> void;

	void setUp(const btVector3& up);

	const btVector3& getUp() { return btController->getUp(); }

	static auto Create() -> Component* {
		return new MovementComponent();
	}

	/// This should probably be called setPositionIncrementPerSimulatorStep.
	/// This is neither a direction nor a velocity, but the amount to
	///	increment the position each simulation iteration, regardless
	///	of dt.
	/// This call will reset any velocity set by setVelocityForTimeInterval().
	virtual void setWalkDirection(const Vector3& walkDirection) {

		btController->setWalkDirection(btVector3(walkDirection.x, walkDirection.y, walkDirection.z));
	}

	/// Caller provides a velocity with which the character should move for
	///	the given time period.  After the time period, velocity is reset
	///	to zero.
	/// This call will reset any walk direction set by setWalkDirection().
	/// Negative time intervals will result in no motion.
	virtual void setVelocityForTimeInterval(const btVector3& velocity, btScalar timeInterval)
	{
		btController->setVelocityForTimeInterval(velocity, timeInterval);
	}

	virtual void setAngularVelocity(const btVector3& velocity)
	{
		btController->setAngularVelocity(velocity);
	}
	virtual const btVector3& getAngularVelocity() const { return btController->getAngularVelocity(); }

	virtual void setLinearVelocity(const Vector3& velocity)
	{
		btController->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
	}
	virtual Vector3 getLinearVelocity() const
	{ 
		btVector3 v = btController->getLinearVelocity();
		return Vector3(v.x(), v.y(), v.z());
	}

	void setLinearDamping(btScalar d) { btController->setLinearDamping(d); }
	btScalar getLinearDamping() const { return btController->getLinearDamping(); }
	void setAngularDamping(btScalar d) { btController->setAngularDamping(d); }
	btScalar getAngularDamping() const { return btController->getAngularDamping(); }

	void reset();
	void warp(const btVector3& origin) { btController->warp(origin); }

	void preStep();
	void playerStep(btScalar dt);

	void setStepHeight(btScalar h) { btController->setStepHeight(h); }
	btScalar getStepHeight() const { return btController->getStepHeight(); }
	void setFallSpeed(btScalar fallSpeed) { btController->setFallSpeed(fallSpeed); }
	btScalar getFallSpeed() const { return btController->getFallSpeed(); }
	void setJumpSpeed(btScalar jumpSpeed) { btController->setJumpSpeed(jumpSpeed); }
	btScalar getJumpSpeed() const { return btController->getJumpSpeed(); }
	void setMaxJumpHeight(btScalar maxJumpHeight) { btController->setMaxJumpHeight(maxJumpHeight); }
	bool canJump() const { return btController->canJump(); }

 	void jump(const Vector3& v = Vector3(0, 10, 0))
	{ 
		btController->jump(btVector3(v.x, v.y, v.z)); 
	}

	void applyImpulse(const btVector3& v) { btController->applyImpulse(v); }

	void setGravity(const Vector3& gravity = Vector3(0,-9.81,0)) { btController->setGravity(btVector3(gravity.x, gravity.y, gravity.z)); }
	btVector3 getGravity() const { btController->getGravity(); }

	/// The max slope determines the maximum angle that the controller can walk up.
	/// The slope angle is measured in radians.
	void setMaxSlope(btScalar slopeRadians) { btController->setMaxSlope(slopeRadians); }
	btScalar getMaxSlope() const { btController->getMaxSlope(); }

	void setMaxPenetrationDepth(btScalar d) { btController->setMaxPenetrationDepth(d); }
	btScalar getMaxPenetrationDepth() const { btController->getMaxPenetrationDepth(); }

	btPairCachingGhostObject* getGhostObject() { return btController->getGhostObject(); }
	void setUseGhostSweepTest(bool useGhostObjectSweepTest)
	{
		btController->setUseGhostSweepTest(useGhostObjectSweepTest);
	}

	bool onGround() const { return btController->onGround(); }
	void setUpInterpolate(bool value) { btController->setUpInterpolate(value); }


	virtual auto SetTransform(const Transform& InTransform, TeleportType InTeleportType) -> void;

	virtual void OnDeserializationCompleted() override;

	auto TryMoveTo(const Vector3& Position) -> bool;

	auto StopMoveTo() -> void;

	auto IsFollowingNavPath() const -> bool { return bIsFollowingNavPath; }

private:
	//character capsule shape ??
	btScalar capsuleRadius = .6f;
	btScalar halfHeight = 1.0f;
	btConvexShape* characterCapsule = nullptr;

	btPairCachingGhostObject* ghostObject = nullptr;

	btScalar stepHeight = 0.2f;
	Vector3 up = { 0.0f, 1.0f, 0.0f };

	MonoMovementComponent* mMonoComponent = new MonoMovementComponent;

	NavPath navPath;


	float navPathMovementSpeed = 8.0f;
	auto GetNextNavPathPosition(float deltaTime) -> Vector3;
	double curSmoothPathPosIndex = 0.0;
	bool bIsFollowingNavPath = false;
	float yDelta = 0.0f;
};

