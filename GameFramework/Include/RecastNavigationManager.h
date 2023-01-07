#pragma once

#include "Singleton.h"
#include <memory>
#include "RecastInputMesh.h"

class RecastInputMesh;

class RecastNavigationManager : public Singleton<RecastNavigationManager>
{
public:
	
	auto GenerateNavMesh() -> void;

	auto DrawDebugInputMesh() const -> void;

protected:

	auto GenerateRecastInputMesh() -> void;

	std::unique_ptr<RecastInputMesh> meshFromPhysicsWorld;
};