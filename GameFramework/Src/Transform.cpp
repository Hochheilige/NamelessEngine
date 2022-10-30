#include "Transform.h"

#include <sstream>

using namespace DirectX;
using namespace std;

Rotator::Rotator()
{
	SetEulerAngles(XMFLOAT3(0.0f, 0.0f, 0.0f));
}

Rotator::Rotator(Vector3 EulerAngles)
{
	SetEulerAngles(EulerAngles);
}

Rotator::Rotator(Quaternion InQuat)
{
	Quat = InQuat;
}

void Rotator::SetEulerAngles(Vector3 Degrees)
{
	const Vector3 radians = Degrees * (XM_PI / 180.0f);

	Quat = Quaternion::CreateFromYawPitchRoll(radians);
}

void Rotator::SetEulerAngles(float X, float Y, float Z)
{
	SetEulerAngles(Vector3(X, Y, Z));
}

Vector3 Rotator::GetEulerDegrees() const
{
	Vector3 radians = GetEulerRadians();
	
	return radians * (180.0f / XM_PI);
}

Vector3 Rotator::GetEulerRadians() const
{
	return Quat.ToEuler();
}

Vector3 Rotator::GetForwardVector() const
{
	return XMVector3Rotate(Vector3::Forward, Quat);
}

Vector3 Rotator::GetUpVector() const
{
	return XMVector3Rotate(Vector3::Up, Quat);
}

Vector3 Rotator::GetRightVector() const
{
	return XMVector3Rotate(Vector3::Right, Quat);
}

void Rotator::RotateAroundLocalAxis(const Vector3& axis, float Angle)
{
	Quat.Concatenate(Quat, DirectX::XMQuaternionRotationAxis(axis, Angle), Quat);
}

void Rotator::RotateAroundAxis(const Vector3& axis, float Angle)
{
	Quaternion q;
	Quat.Inverse(q);
	RotateAroundLocalAxis(DirectX::XMVector3Rotate(axis, q), Angle);
}

Quaternion Rotator::GetQuaterion() const
{
	return Quat;
}

Transform::Transform(Matrix mat)
{
	SetFromMatrix(mat);
}

Matrix Transform::GetTransformMatrix() const
{
	const Matrix translate = Matrix::CreateTranslation(Position);

	const Matrix rotate = Matrix::CreateFromQuaternion(Rotation.GetQuaterion());

	const Matrix scale = Matrix::CreateScale(Scale);

	return scale * rotate * translate;
}

Matrix Transform::GetTransformMatrixTransposed() const
{
	XMFLOAT4X4 mat = GetTransformMatrix();
	XMMATRIX m = XMLoadFloat4x4(&mat);

	m = DirectX::XMMatrixTranspose(m);

	XMStoreFloat4x4(&mat, m);
	
	return mat;
}

Matrix Transform::GetInverseTransformMatrix() const
{
	XMFLOAT4X4 mat = GetTransformMatrix();
	XMMATRIX m = XMLoadFloat4x4(&mat);

	m = DirectX::XMMatrixInverse(nullptr, m);

	XMStoreFloat4x4(&mat, m);
	
	return mat;
}

Matrix Transform::GetInverseTransformMatrixTransposed() const
{
	const Matrix mat = GetInverseTransformMatrix();
	return mat.Transpose();
}

bool Transform::SetFromMatrix(Matrix Mat)
{
	return Mat.Decompose(Scale, Rotation.Quat, Position);
}

bool Transform::SetFromMatrixIgnoreScale(Matrix Mat)
{
	Vector3 scale;
	return Mat.Decompose(scale, Rotation.Quat, Position);
}

Transform Transform::TransformToWorld(const Transform& ParentTransform) const
{
	return GetTransformMatrix() * ParentTransform.GetTransformMatrix();
}

auto Transform::TransformToLocal(const Transform& ParentTransform) const -> Transform
{
    return GetTransformMatrix() * ParentTransform.GetInverseTransformMatrix();
}

Matrix Transform::GetNormalMatrix() const
{
	Transform tr = *this;
	tr.Position = Vector3::Zero;
	return tr.GetTransformMatrix().Invert().Transpose();
}

Matrix Transform::GetNormalMatrixTransposed() const
{
	Transform tr = *this;
	tr.Position = Vector3::Zero;
	return tr.GetTransformMatrix().Invert();
}

auto Transform::ToString() const->std::string
{
	stringstream sstream;
	std::string str = "Position: x=";
	sstream << "Position: x = " << Position.x << "\t" << "y = " << Position.y << "\t" << "z = " << Position.x << endl;
	sstream << "Scale: x = " << Scale.x << "\t" << "y = " << Scale.y << "\t" << "z = " << Scale.z << endl;
	Vector3 euler = Rotation.GetEulerDegrees();
	sstream << "Euler Degrees =  = " << euler.x << "\t" << "y = " << euler.y << "\t" << "z = " << euler.x << endl;

	return sstream.str();
}

