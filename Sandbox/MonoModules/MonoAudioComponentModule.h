#pragma once

#include "../Actor.h"

class MonoAudioComponentModule
{
public:
	MonoAudioComponentModule()
	{
		mono_add_internal_call("Scripts.Components.AudioComponent::InternalLoadSound", &LoadSound);
		mono_add_internal_call("Scripts.Components.AudioComponent::InternalUnLoadSound", &UnLoadSound);
		mono_add_internal_call("Scripts.Components.AudioComponent::InternalPlay", &Play);
		mono_add_internal_call("Scripts.Components.AudioComponent::InternalStopChannel", &StopChannel);
		mono_add_internal_call("Scripts.Components.AudioComponent::InternalSetVolume", &SetVolume);
	}

private:
	static void LoadSound(AudioComponent* component, MonoObject* soundName, bool b3d, bool looping, bool stream)
	{
		auto str = mono_string_to_utf8(mono_object_to_string(soundName, nullptr));
		component->LoadSound(str, b3d, looping, stream);
	}

	static void UnLoadSound(AudioComponent* component, MonoObject* soundName) 
	{
		auto str = mono_string_to_utf8(mono_object_to_string(soundName, nullptr));
		component->UnLoadSound(str); 
	}
	static void Play(AudioComponent* component) { component->Play(); }
	static void StopChannel(AudioComponent* component) { component->StopChannel(); }
	static void SetVolume(AudioComponent* component, float volume) { component->SetVolume(volume); }
};