#pragma once

#include "LineRenderer.h"
#include <vector>

class PlaneComponent : public LineRenderer
{
public:
	PlaneComponent(std::shared_ptr<Transform> transform);

};