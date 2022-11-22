#include "Utils.h"
#include "uuid.h"

std::mt19937 random;
uuids::uuid_random_generator uuidGenerator = uuids::uuid::gen;

void Utils::init() {
	std::random_device rd;
	random = std::mt19937(rd());
	uuidGenerator = uuids::uuid_random_generator(&random);
}

uuids::uuid genUuid()
{
	return 
}