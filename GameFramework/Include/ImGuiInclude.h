#pragma once

#include "MathInclude.h"

#define IM_VEC2_CLASS_EXTRA                                                     \
        constexpr ImVec2(const Vector2& f) : x(f.x), y(f.y) {}                   \
        operator Vector2() const { return Vector2(x,y); }

#define IM_VEC4_CLASS_EXTRA                                                     \
        constexpr ImVec4(const Vector4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {}   \
        operator Vector4() const { return Vector4(x,y,z,w); }

#include "imgui.h"