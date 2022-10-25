#include "AudioComponent.h"
#include "MonoSystem.h"

AudioComponent::AudioComponent(MonoSystem* mono) : _mono(mono)
{
	auto image = mono->GetImage();
	_klass = mono_class_from_name(image, "Scripts", "AudioComponent");
	_instance = _mono->CreateClassInstance(_klass);
}

void AudioComponent::Init()
{
}

void AudioComponent::OnRegister()
{
	MonoMethodDesc* methodDescr = _mono->MakeMethodDescriptor("Scripts.AudioComponent:OnRegister()", true);

	MonoMethod* method = _mono->GetMethod(_klass, methodDescr);
	_mono->InvokeMethod(method, _instance, nullptr, nullptr);
}

void AudioComponent::GetTransform(MonoSystem* mono)
{
	MonoMethodDesc* methodDesc = _mono->MakeMethodDescriptor("Scripts.AudioComponent:GetParentTransform()", true);
	MonoMethod* method = mono->GetMethod(_klass, methodDesc);
	MonoObject* result = _mono->InvokeMethod(method, _instance, nullptr, nullptr);

	auto m = (Transform*)mono_object_unbox(result);
}
