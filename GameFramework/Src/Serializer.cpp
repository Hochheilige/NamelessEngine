#include <Serializer.h>

#include "Component.h"
#include "Game.h"

json Serializer::Serialize(const Game* game)
{
	return game->Serialize();
}

void Serializer::Deserialize(const json* in, Game& game)
{
	game.Deserialize(in);
}

void Serializer::SaveToFile(Path path, const Game* game)
{
	create_directories(path.parent_path());

	std::ofstream out(path);
	out << std::setw(4) << Serialize(game) << std::endl;
	out.close();
}

void Serializer::ReadFromFile(Path path, Game* game)
{
	assert(exists(path) && "Provided file doesn't exist");

	std::ifstream in(path);
	const json data = json::parse(in);

	game->Deserialize(&data);
}
