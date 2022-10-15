#include "AudioComponent.h"

AudioComponent::AudioComponent(MonoImage* image)
{
	ptrMainClass = mono_class_from_name(image, "Scripts", "AudioComponent");
}

void AudioComponent::Init()
{
}

void AudioComponent::OnRegister()
{
	if (ptrMainClass) {
		MonoMethodDesc* ptrMainMethodDesc = mono_method_desc_new("Scripts.AudioComponent:OnRegister()", true);
		if (ptrMainMethodDesc) {
			MonoMethod* ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);
			if (ptrMainMethod) {
				mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, nullptr);
			}
		}
	}
}
