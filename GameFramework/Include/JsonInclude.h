#pragma once

#include <uuid.h>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using uuid = uuids::uuid;

namespace uuids {
    void to_json(json& out, const uuid& id) {
        out = to_string(id);
    }

    void from_json(const json& in, uuid& id) {
        id = uuids::uuid::from_string(in.get<std::string>()).value();
    }
}
