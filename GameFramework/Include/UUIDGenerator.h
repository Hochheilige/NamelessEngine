#pragma once
#include <random>
#include <uuid.h>

class UUIDGenerator {
public:
    UUIDGenerator() {
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size>{};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

        random_generator = std::mt19937(seq);
        generator =
            std::make_unique<uuids::uuid_random_generator>(random_generator);
    }

    uuids::uuid generate() { return (*generator)(); }

private:
    std::mt19937 random_generator;
    std::unique_ptr<uuids::uuid_random_generator> generator;
};
