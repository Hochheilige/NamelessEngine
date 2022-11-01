#pragma once

#include "MathInclude.h"

#include <DirectXMath.h>
#include <string>

struct Rotator
{
protected:
	Quaternion Quat;

public:

	Rotator();
	Rotator(Vector3 EulerAngles);
	Rotator(Quaternion InQuat);

	// Set rotation with euler angles in degrees
	void SetEulerAngles(Vector3 EulerAngles);
	void SetEulerAngles(float X, float Y, float Z);

	Vector3 GetEulerDegrees() const;

	Vector3 GetEulerRadians() const;

	Vector3 GetForwardVector() const;

	Vector3 GetUpVector() const;

	Vector3 GetRightVector() const;

	// Adds angle degrees rotation around given axis
	// the rotation is local in terms that axis is chosen as if your rotation is zeroed out
	void RotateAroundLocalAxis(const Vector3& axis, float Angle);

	void RotateAroundAxis(const Vector3& axis, float Angle);

	Quaternion GetQuaterion() const;

	friend struct Transform;
};

struct Transform
{
	Vector3 Position = Vector3::Zero;
	Rotator Rotation;
	Vector3 Scale = Vector3::One;

	Transform() = default;
	Transform(Matrix mat);
	Transform(const Vector3& Position, const Rotator& Rotation, const Vector3& Scale)
		: Position(Position), Rotation(Rotation), Scale(Scale) {}

	Matrix GetTransformMatrix() const;

	Matrix GetTransformMatrixTransposed() const;

	Matrix GetInverseTransformMatrix() const;

	Matrix GetInverseTransformMatrixTransposed() const;

	bool SetFromMatrix(Matrix Mat);

	bool SetFromMatrixIgnoreScale(Matrix Mat);

	// todo: rename this function
	Transform TransformToWorld(const Transform& ParentTransform) const;

	auto TransformToLocal(const Transform& ParentTransform) const -> Transform;


	Matrix GetNormalMatrix() const;

	Matrix GetNormalMatrixTransposed() const;

	auto ToString() const -> std::string;

	static Transform Identity;
};

