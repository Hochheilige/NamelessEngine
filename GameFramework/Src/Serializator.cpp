#include <Serializator.h>

#include "Game.h"

json Serializator::Serialize(const Game* game)
{
	return game->Serialize();
}

void Serializator::SaveToFile(Path path, const Game* game)
{
	create_directories(path.parent_path());

	std::ofstream out(path);
	out << std::setw(4) << Serialize(game) << std::endl;
	out.close();
}
