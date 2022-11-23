#pragma once

#include <uuid.h>
#include "JsonInclude.h"
#include "MathInclude.h"

using uuid = uuids::uuid;

namespace uuids {
	static void to_json(json& out, const uuid& id)
	{
		out = to_string(id);
	}
	static void from_json(const json& in, uuid& id)
	{
		id = uuids::uuid::from_string(in.get<std::string>()).value();
	}
}

namespace DirectX {
	namespace SimpleMath {
		static void to_json(json& out, const Quaternion& quaternion) {
			out = json::array();
			out.push_back(quaternion.x);
			out.push_back(quaternion.y);
			out.push_back(quaternion.z);
			out.push_back(quaternion.w);
		}

		static void from_json(const json& in, Quaternion& quaternion) {
			assert(in.is_array());
			assert(in.size() == 4);

			quaternion.x = in[0].get<float>();
			quaternion.y = in[1].get<float>();
			quaternion.z = in[2].get<float>();
			quaternion.w = in[3].get<float>();
		}

		static void to_json(json& out, const Vector3& v3) {
			out = json::array();
			out.push_back(v3.x);
			out.push_back(v3.y);
			out.push_back(v3.z);
		}

		static void from_json(const json& in, Vector3& v3) {
			assert(in.is_array());
			assert(in.size() == 3);

			v3.x = in[0].get<float>();
			v3.y = in[1].get<float>();
			v3.z = in[2].get<float>();
		}
	}
}
