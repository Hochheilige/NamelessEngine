#pragma once

#include "Singleton.h"
#include <memory>
#include "RecastInputMesh.h"
#include "Recast.h"
#include "MathInclude.h"
#include "DetourNavMesh.h"
#include "JsonInclude.h"

static const int MAX_POLYS = 256;
static const int MAX_SMOOTH = 2048;

struct NavPath
{
	dtPolyRef m_polys[MAX_POLYS];
	int m_npolys = 0;
	float m_smoothPath[MAX_SMOOTH * 3];
	int m_nsmoothPath = 0;

};

class RecastInputMesh;

enum SamplePartitionType
{
	SAMPLE_PARTITION_WATERSHED,
	SAMPLE_PARTITION_MONOTONE,
	SAMPLE_PARTITION_LAYERS
};

class RecastNavigationManager : public Singleton<RecastNavigationManager>
{
public:
	
	auto GenerateNavMesh() -> bool;

	auto DrawDebugInputMesh() const -> void;

	auto DrawDebugNavMesh() const -> void;

	auto FindPath(const Vector3& startPos, const Vector3& endPos, NavPath& outPath) -> bool;

	auto Serialize() -> json;

	auto Deserialize(const json& inJson) -> void;

	// settings that should be modified by gui
public:

	float navMeshBMin[3]{ -20.0f, -20.0f, -20.0f };
	float navMeshBMax[3]{  20.0f,  20.0f,  20.0f };

	float m_cellSize = 0.3f;
	float m_cellHeight = 0.2f;
	float m_agentHeight = 2.0f;
	float m_agentRadius = 0.6f;
	float m_agentMaxClimb = 0.9f;
	float m_agentMaxSlope = 45.0f;
	float m_regionMinSize = 8;
	float m_regionMergeSize = 20;
	float m_edgeMaxLen = 12.0f;
	float m_edgeMaxError = 1.3f;
	float m_vertsPerPoly = 6.0f;
	float m_detailSampleDist = 6.0f;
	float m_detailSampleMaxError = 1.0f;
	int m_partitionType = SAMPLE_PARTITION_WATERSHED;

	bool m_filterLowHangingObstacles = true;
	bool m_filterLedgeSpans = true;
	bool m_filterWalkableLowHeightSpans = true;

	bool m_keepInterResults = false;
	float m_totalBuildTimeMs;

	bool bDrawNavMeshDebug = false;
	bool bDrawInputMeshDebug = false;

protected:

	auto Cleanup() -> void;

	auto GenerateRecastInputMesh() -> void;

	std::unique_ptr<RecastInputMesh> meshFromPhysicsWorld;
	rcConfig m_cfg;
	rcContext* m_ctx = nullptr;
	rcHeightfield* m_solid;
	unsigned char* m_triareas;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcPolyMeshDetail* m_dmesh;
	class dtNavMesh* m_navMesh;
	class dtNavMeshQuery* m_navQuery = nullptr;
};
