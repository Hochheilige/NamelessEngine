#pragma once

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <unordered_map>
#include <vector>

using SoundMap = std::unordered_map<std::string, FMOD::Sound*>;
using ChannelMap = std::unordered_map<int, FMOD::Channel*>;

class AudioModule
{
public:

	~AudioModule();

	static AudioModule* GetInstance();

	void Update();

	FMOD::Studio::System* GetStudioSystem() { return StudioSystem; }

	FMOD::System* GetSystem() { return System; }

	int GetNextChannelID() { return nextChannelID; }

	SoundMap& GetSounds() { return Sounds; }

	ChannelMap& GetChannels() { return Channels; }

private:
	AudioModule();

	static AudioModule* instance;

	FMOD::Studio::System* StudioSystem;
	FMOD::System*         System;

	SoundMap   Sounds;
	ChannelMap Channels;

public:
	int nextChannelID;
};