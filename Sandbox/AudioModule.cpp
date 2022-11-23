#include "AudioModule.h"

AudioModule* AudioModule::instance = nullptr;

AudioModule::AudioModule() : 
	StudioSystem(nullptr), System(nullptr), nextChannelID(0)
{
	FMOD::Studio::System::create(&StudioSystem);
	StudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, nullptr);
	StudioSystem->getCoreSystem(&System);
}

AudioModule::~AudioModule()
{
	StudioSystem->unloadAll();
	StudioSystem->release();
}

AudioModule* AudioModule::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new AudioModule();
	}
	return instance;
}

void AudioModule::Update()
{
	std::vector<ChannelMap::iterator> stoppedChannels;
	for (auto it = Channels.begin(), itEnd = Channels.end(); it != itEnd; ++it)
	{
		bool isPlaying = false;
		it->second->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			stoppedChannels.push_back(it);
		}
	}

	for (auto& channel : stoppedChannels)
	{
		Channels.erase(channel);
	}

	StudioSystem->update();
}