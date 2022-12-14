#pragma once

#include "AudioModule.h"
#include "SceneComponent.h"

class AudioComponent : public SceneComponent
{
public:
	AudioComponent() {}

	~AudioComponent();

	virtual void Init() override {};

	virtual void Update(float DeltaTime) override;

	void LoadSound(const std::string& soundName, bool b3d = true, bool looping = false, bool stream = false);

	void UnLoadSound(const std::string& soundName);

	void Play();

	void StopChannel();

	void StopAllChannels();

	bool IsPlaying(int channelID);

	void SetVolume(float volume);

private:
	FMOD::Channel* channel = nullptr;
	FMOD::Sound* sound = nullptr;
	int channelID = -1;
	std::string name;
	
	const float kDefaultVolume = 0.3f;
	float volume = .0f;
};