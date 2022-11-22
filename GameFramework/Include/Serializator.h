#pragma once
#include <filesystem>
#include <fstream>

#include "JsonInclude.h"
using Path = std::filesystem::path;

class Game;

class Serializator
{
public:
   static json Serialize(Game* game);
   static void SaveToFile(Path path, Game* game);
};
