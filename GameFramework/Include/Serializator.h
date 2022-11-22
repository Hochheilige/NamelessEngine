﻿#pragma once
#include <filesystem>
#include <fstream>

#include "JsonInclude.h"
using Path = std::filesystem::path;

class Game;

class Serializator
{
public:
   static json Serialize(const Game* game);
   static void Deserialize(const json* in, Game& game);
   static void SaveToFile(Path path, const Game* game);
   static void ReadFromFile(Path path, Game* game);
};
