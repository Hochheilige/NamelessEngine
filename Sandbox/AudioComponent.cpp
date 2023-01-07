#include "AudioComponent.h"

void AudioComponent::LoadSound(const std::string& soundName, bool b3d, bool looping, bool stream)
{
	auto audio = AudioModule::GetInstance();
	auto& sounds = audio->GetSounds();

	name = soundName;
	is_3d = b3d;
	is_looping = looping;
	is_stream = stream;
	
	if (auto found = sounds.find(soundName); found != sounds.end())
	{
		sound = sounds[soundName];
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
		channel->setVolume(volume ? volume : kDefaultVolume);
		channel->setPaused(false);
		channels[channelID] = channel;
	}

}

void AudioComponent::StopChannel()
{
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

void AudioComponent::SetVolume(float volume)
{
	this->volume = volume;
}

json AudioComponent::Serialize() const
{
	auto out = SceneComponent::Serialize();
	out["name"] = name;
	out["channel_id"] = channelID;
	out["is_3d"] = is_3d;
	out["loop"] = is_looping;
	out["stream"] = is_stream;
	//out["audio_path"] = "";
	return out;
}

void AudioComponent::Deserialize(const json* in)
{
	channelID = in->at("channel_id").get<int>();
	LoadSound(in->at("name").get<std::string>(), in->at("is_3d").get<bool>(), 
		in->at("loop").get<bool>(), in->at("stream").get<bool>());
	SceneComponent::Deserialize(in);
}

ComponentType AudioComponent::GetComponentType()
{
	return AudioComponentType;
}
