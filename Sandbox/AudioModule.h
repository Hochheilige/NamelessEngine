#pragma once

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class AudioModule
{
public:

	~AudioModule();

	static AudioModule* GetInstance();

	void Update();

private:
	AudioModule();

	static AudioModule* instance;

	FMOD::Studio::System* StudioSystem;
	FMOD::System*         System;

	int nextChannelID;

	std::unordered_map<std::string, FMOD::Sound*> Sounds;
	std::unordered_map<int, FMOD::Channel*>       Channels;
};