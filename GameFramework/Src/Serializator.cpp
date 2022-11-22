#include <Serializator.h>

#include "Game.h"

json Serializator::Serialize(const Game* game)
{
	return game->Serialize();
}

void Serializator::Deserialize(const json* in, Game& game)
{
	game.Deserialize(in);
}

void Serializator::SaveToFile(Path path, const Game* game)
{
	create_directories(path.parent_path());

	std::ofstream out(path);
	out << std::setw(4) << Serialize(game) << std::endl;
	out.close();
}

void Serializator::ReadFromFile(Path path, Game* game)
{
	assert(exists(path) && "Provided file doesn't exist");

	std::ifstream in(path);
	const json data = json::parse(in);

	game->Deserialize(&data);
}
