#pragma once

#include <d3dcommon.h>
#include <d3d11.h>

#include <wrl/client.h>
using namespace Microsoft::WRL;

#include <vector>

#include "MathInclude.h"

#pragma pack(push, 4)
struct ColoredVertex
{
	Vector3 Position;
	Color Color;
};

struct TexturedVertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector3 Binormal;
	Vector3 Tangent;
	Vector2 TexCoord;
};

struct BasicVertex
{
	Vector3 Position;
};
#pragma pack (pop)

template <class T>
class Mesh
{
public:
	/*
	* Allocates MeshProxy on the heap and expects the calling code to take ownership of it
	* todo: use some smart pointer ?
	*/
	virtual class MeshProxy* CreateMeshProxy() = 0;

	virtual ~Mesh() = default;

	// returns the index of the inserted vertex
	UINT AddVertex(const T& InVertex)
	{
		Vertices.push_back(InVertex);
		return static_cast<UINT>(Vertices.size() - 1);
	}

	void AddIndex(UINT InIndex)
	{
		Indices.push_back(InIndex);
	}

	void AddFace(UINT I1, UINT I2, UINT I3)
	{
		Indices.push_back(I1);
		Indices.push_back(I2);
		Indices.push_back(I3);
	}

	T& GetVertex(size_t InIndex)
	{
		return Vertices[InIndex];
	}

protected:
	std::vector<T> Vertices;

	std::vector<UINT> Indices;
};


/*
* Mesh that has position and color vertex data
*/
class ColoredMesh : public Mesh<ColoredVertex>
{
public:
	virtual class MeshProxy* CreateMeshProxy() override;
};


/*
* Mesh that has position, normal and uv coordinates vertex data
*/
class TexturedMesh : public Mesh<TexturedVertex>
{
public:

	virtual class MeshProxy* CreateMeshProxy() override;
};

// todo: move to PongMeshes.h and PongMeshes.cpp files
#include <DirectXMath.h>
#include <xutility>
using namespace DirectX;
class SquareMesh : public ColoredMesh
{
public:
	SquareMesh()
	{
		AddVertex({ Vector3(1.0f, 1.0f, 0.5f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) });
		AddVertex({ Vector3(-1.0f, -1.0f, 0.5f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) });
		AddVertex({ Vector3(1.0f, -1.0f, 0.5f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) });
		AddVertex({ Vector3(-1.0f, 1.0f, 0.5f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) });

		AddIndex(0);
		AddIndex(1);
		AddIndex(2);
		AddIndex(3);
		AddIndex(0);
		AddIndex(1);
	}
};

class TexturedSquareMesh : public TexturedMesh
{
public:
	TexturedSquareMesh()
	{
		AddVertex({ Vector3( 1.0f,  1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) });
		AddVertex({ Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f) });
		AddVertex({ Vector3( 1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f) });
		AddVertex({ Vector3(-1.0f,  1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) });

		AddIndex(0);
		AddIndex(1);
		AddIndex(2);
		AddIndex(1);
		AddIndex(0);
		AddIndex(3);
	}
};

#include <vector>
class CircleMesh : public ColoredMesh
{
public:
	CircleMesh()
	{

		AddVertex({ Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) });

		int numPoints = 20;
		int i = 1;
		for (float alpha = 0.0f; alpha < 6.28f; alpha += 6.28f / numPoints, ++i)
		{
			const Vector3 point = Vector3(cosf(alpha), sinf(alpha), 0.0f);
			AddVertex({ point, Vector4(1.0f, 0.5f, 0.5f, 1.0f) });

			AddIndex(0);
			AddIndex(i);
			AddIndex((i % numPoints) + 1);
		}
	}
};

class BoxMesh : public ColoredMesh
{
public:
	BoxMesh()
	{
		AddVertex({ Vector3(0.5f, 0.5f, 0.5f),    Color(1.0f, 0.0f, 0.0f, 1.0f) }); // 0
		AddVertex({ Vector3(-0.5f, -0.5f, 0.5f),  Color(0.0f, 0.0f, 1.0f, 1.0f) }); // 1
		AddVertex({ Vector3(0.5f, -0.5f, 0.5f),   Color(0.0f, 1.0f, 0.0f, 1.0f) }); // 2
		AddVertex({ Vector3(-0.5f, 0.5f, 0.5f),   Color(1.0f, 1.0f, 1.0f, 1.0f) }); // 3
		AddVertex({ Vector3(0.5f, 0.5f, -0.5f),   Color(1.0f, 0.0f, 0.0f, 1.0f) }); // 4
		AddVertex({ Vector3(-0.5f, -0.5f, -0.5f), Color(0.0f, 0.0f, 1.0f, 1.0f) }); // 5
		AddVertex({ Vector3(0.5f, -0.5f, -0.5f),  Color(0.0f, 1.0f, 0.0f, 1.0f) }); // 6
		AddVertex({ Vector3(-0.5f, 0.5f, -0.5f),  Color(1.0f, 1.0f, 1.0f, 1.0f) }); // 7

		AddFace(0, 1, 2);
		AddFace(1, 0, 3);
		AddFace(4, 5 ,6);
		AddFace(5, 4, 7);
		AddFace(4, 0, 2);
		AddFace(4, 2, 6);
		AddFace(7, 5, 1);
		AddFace(7, 1, 3);
		AddFace(4, 0, 3);
		AddFace(4, 3, 7);
		AddFace(6, 1, 2);
		AddFace(6, 5, 1);
	}
};

#include "MathInclude.h"
class SphereMesh : public TexturedMesh
{
public:
	SphereMesh()
	{
		const int numPointsVert = 50; // Excluding the "pole" points
		const int numPointsHor = 50;

#pragma region GeneratePoints
		// Generate north pole point
		AddVertex({ Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) });

		// Generate lattitude points
		const float phetaDelta = XM_PI / (numPointsVert + 1);
		for (int j = 0; j < numPointsVert; ++j)
		{
			const float pheta = XM_PIDIV2 - phetaDelta * (j + 1);
			const float y = sinf(pheta);
			const float deltaAlpha = XM_2PI / static_cast<float>(numPointsHor);
			const float curRadius = cosf(pheta);
			for (int i = 0; i < numPointsHor; ++i)
			{
				const float alpha = deltaAlpha * i;
				const Vector3 position = Vector3(curRadius * cosf(alpha), y, curRadius * sinf(alpha));
				Vector3 normal;
				position.Normalize(normal);
				AddVertex({ position, -normal });
			}
		}

		// Generate south pole point
		AddVertex({ Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f) });
#pragma endregion GeneratePoints

#pragma region CreateTriangleIndices
		// Connect the first circle to the north pole
		for (int i = 1; i <= numPointsHor; ++i)
		{
			AddIndex(0);
			AddIndex(i);
			AddIndex((i % numPointsHor) + 1);
		}
		// Connect the intermediate circles to each other
		for (int j = 1; j < numPointsVert; ++j)
		{
				const int curOffset = j * numPointsHor;
				const int prevOffset = curOffset - numPointsHor;
				for (int i = 1; i <= numPointsHor; ++i)
				{
					const int circlePoint1 = i;
					const int circlePoint2 = (i % numPointsHor) + 1;
					AddIndex(prevOffset + circlePoint2);
					AddIndex(prevOffset + circlePoint1);
					AddIndex(curOffset + circlePoint1);
					AddIndex(curOffset + circlePoint1);
					AddIndex(curOffset + circlePoint2);
					AddIndex(prevOffset + circlePoint2);
				}
		}
		// Connect the last circle to the south pole
		const int offset = (numPointsVert - 1) * numPointsHor;
		for (int i = 1; i <= numPointsHor; ++i)
		{
			AddIndex(offset + i);
			AddIndex(offset + (i % numPointsHor) + 1);
			AddIndex(numPointsHor * numPointsVert + 1);
		}
#pragma endregion CreateTriangleIndices
	}
};

class BasicMesh : public Mesh<BasicVertex>
{
public:

	virtual class MeshProxy* CreateMeshProxy() override;
};

class BasicSphereMesh : public BasicMesh
{
public:
	BasicSphereMesh()
	{
		const int numPointsVert = 20; // Excluding the "pole" points
		const int numPointsHor = 20;

#pragma region GeneratePoints
		// Generate north pole point
		AddVertex({ Vector3(0.0f, 1.0f, 0.0f) });

		// Generate lattitude points
		const float phetaDelta = XM_PI / (numPointsVert + 1);
		for (int j = 0; j < numPointsVert; ++j)
		{
			const float pheta = XM_PIDIV2 - phetaDelta * (j + 1);
			const float y = sinf(pheta);
			const float deltaAlpha = XM_2PI / static_cast<float>(numPointsHor);
			const float curRadius = cosf(pheta);
			for (int i = 0; i < numPointsHor; ++i)
			{
				const float alpha = deltaAlpha * i;
				const Vector3 position = Vector3(curRadius * cosf(alpha), y, curRadius * sinf(alpha));
				AddVertex({ position });
			}
		}

		// Generate south pole point
		AddVertex({ Vector3(0.0f, -1.0f, 0.0f)});
#pragma endregion GeneratePoints

#pragma region CreateTriangleIndices
		// Connect the first circle to the north pole
		for (int i = 1; i <= numPointsHor; ++i)
		{
			AddIndex(0);
			AddIndex(i);
			AddIndex((i % numPointsHor) + 1);
		}
		// Connect the intermediate circles to each other
		for (int j = 1; j < numPointsVert; ++j)
		{
			const int curOffset = j * numPointsHor;
			const int prevOffset = curOffset - numPointsHor;
			for (int i = 1; i <= numPointsHor; ++i)
			{
				const int circlePoint1 = i;
				const int circlePoint2 = (i % numPointsHor) + 1;
				AddIndex(prevOffset + circlePoint2);
				AddIndex(prevOffset + circlePoint1);
				AddIndex(curOffset + circlePoint1);
				AddIndex(curOffset + circlePoint1);
				AddIndex(curOffset + circlePoint2);
				AddIndex(prevOffset + circlePoint2);
			}
		}
		// Connect the last circle to the south pole
		const int offset = (numPointsVert - 1) * numPointsHor;
		for (int i = 1; i <= numPointsHor; ++i)
		{
			AddIndex(offset + i);
			AddIndex(offset + (i % numPointsHor) + 1);
			AddIndex(numPointsHor * numPointsVert + 1);
		}
#pragma endregion CreateTriangleIndices
	}
};