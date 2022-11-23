#include "AudioComponent.h"

void AudioComponent::LoadSound(const std::string& soundName, bool b3d, bool looping, bool stream)
{
	auto audio = AudioModule::GetInstance();
	auto& sounds = audio->GetSounds();
	
	if (auto found = sounds.find(soundName); found != sounds.end())
	{
		return;
	}

	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= b3d ? FMOD_3D : FMOD_2D;
	mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	mode |= stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	auto result = audio->GetSystem()->createSound(soundName.c_str(), mode, nullptr, &sound);
	if (sound)
	{
		sounds[soundName] = sound;
	}
}

void AudioComponent::UnLoadSound(const std::string& soundName)
{
	auto audio = AudioModule::GetInstance();
	auto& sounds = audio->GetSounds();

	auto found = sounds.find(soundName);
	if (found == sounds.end())
	{
		return;
	}

	found->second->release();
	sounds.erase(found);
}

void AudioComponent::Play()
{
	auto audio = AudioModule::GetInstance();
	auto& sounds = audio->GetSounds();
	auto& channels = audio->GetChannels();

	if (channelID == -1)
	{
		channelID = audio->nextChannelID++;
	}

	//auto found = sounds.find(name);
	//if (found == sounds.end())
	//{
	//	LoadSound(name);
	//}

	audio->GetSystem()->playSound(sound, nullptr, true, &channel);
	if (channel)
	{
		FMOD_MODE currMode;
		sound->getMode(&currMode);
		if (currMode & FMOD_3D)
		{
			Transform trans = GetTransform();
			FMOD_VECTOR position(trans.Position.x, trans.Position.y, trans.Position.z);
			channel->set3DAttributes(&position, nullptr);
		}
		channel->setVolume(20 * log10f(10));
		channel->setPaused(false);
		channels[channelID] = channel;
	}

}

void AudioComponent::StopChannel()
{
	auto audio = AudioModule::GetInstance();
	auto& sounds = audio->GetSounds();
	auto& channels = audio->GetChannels();

	channel->stop();
}

void AudioComponent::Update(float DeltaTime)
{
	Transform trans = GetTransform();
	FMOD_VECTOR position(trans.Position.x, trans.Position.y, trans.Position.z);
	channel->set3DAttributes(&position, nullptr);

	//bool isPlaying = false;
	//channel->isPlaying(&isPlaying);
	//if (!isPlaying)
	//{
	//	Play();
	//}

	AudioModule::GetInstance()->Update();
}