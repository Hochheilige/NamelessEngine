#pragma once

#include "JsonSerializers.h"
#include "JsonInclude.h"
#include "MathInclude.h"
#include <string>

struct Rotator
{
protected:
	Quaternion Quat;
	friend void to_json(json& out, const Rotator& rotator);
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

static void to_json(json& out, const Rotator& rotator) {
	out = rotator.Quat;
}

static void from_json(const json& in, Rotator& rotator) {
	rotator = Rotator(in.get<Quaternion>());
}

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

static void to_json(json& out, const Transform& transform) {
	out = json::object();
	out["pos"] = transform.Position;
	out["rot"] = transform.Rotation;
	out["scale"] = transform.Scale;
}

static void from_json(const json& in, Transform& transform) {
	assert(in.is_object());
	transform = Transform(in.at("pos"), in.at("rot"), in.at("scale"));
}
