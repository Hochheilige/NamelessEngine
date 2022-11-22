#include <Serializator.h>

#include "Game.h"

json Serializator::Serialize(Game* game)
{
	return game->Serialize();
}

void Serializator::SaveToFile(Path path, Game* game)
{
	std::ofstream out(path);
	out << std::setw(4) << Serialize(game) << std::endl;
}
