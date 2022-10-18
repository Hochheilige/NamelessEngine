#include "MonoSystem.h"
#include "Mappings.h"
#include <filesystem>

MonoSystem::MonoSystem()
{
	auto exePath = __argv[0];
	std::filesystem::path path(exePath);
	auto parentPath = path.parent_path().string();
	auto result = parentPath.c_str();


	mono_set_dirs(result, result);
	mono_set_assemblies_path("mono/lib");

	//TODO add exceptions/message if domain/assembly/image can't be loaded

	rootDomain = mono_jit_init("NamelessEngine");

	if (rootDomain) {
		auto pathStr = parentPath + "\\Scripts.dll";

		char appDomainName[] = "EngineAppDomain";
		appDomain = mono_domain_create_appdomain(appDomainName, nullptr);
		mono_domain_set(appDomain, true);

		m_ptrGameAssembly = mono_domain_assembly_open(rootDomain, pathStr.c_str());
		if (m_ptrGameAssembly) {
			image = mono_assembly_get_image(m_ptrGameAssembly);
			if (image) {
				mono_add_internal_call("Scripts.Script::CreateCubeObject", &Mappings::CS_CreateObj);
				mono_add_internal_call("Scripts.AudioComponent::InternalOnRegister", &Mappings::CS_AudioOnCreate);
			}
		}
	}
}


MonoImage* MonoSystem::GetImage()
{
	return image;
}

MonoClass* MonoSystem::FindClass(const char* nameSpace, const char* className) const
{
	return mono_class_from_name(image, nameSpace, className);
}

MonoMethodDesc* MonoSystem::MakeMethodDescriptor(const char* descriptor, bool includeNamespace) const
{
	return mono_method_desc_new(descriptor, includeNamespace);
}

MonoMethod* MonoSystem::GetMethodByClass(MonoClass* clazz, MonoMethodDesc* desc) const
{
	return mono_method_desc_search_in_class(desc, clazz);
}

MonoObject* MonoSystem::InvokeMethod(MonoMethod* method, void* obj, void** params,
	MonoObject** exception) 
{
	return mono_runtime_invoke(method, obj, params, exception);
}

MonoMethod* MonoSystem::GetMethod(const char* nameSpace, const char* className, const char* desc)
{
	const auto clazz = FindClass(nameSpace, className);

	auto fullDesc = std::string(nameSpace);
	fullDesc.append(".");
	fullDesc.append(className);
	fullDesc.append(":");
	fullDesc.append(desc);

	const auto desciptor = MakeMethodDescriptor(fullDesc.c_str(), true);
	return GetMethodByClass(clazz, desciptor);
}