#include "MonoSystem.h"
#include "CSBind.h"
#include <filesystem>

MonoSystem::MonoSystem()
{
	auto exePath = __argv[0];
	std::filesystem::path path(exePath);
	auto parentPath = path.parent_path().string();
	auto result = parentPath.c_str();


	mono_set_dirs(result, result);

	m_ptrMonoDomain = mono_jit_init("Katamari");

	if (m_ptrMonoDomain) {
		auto pathStr = parentPath + "\\Scripts.dll";
		m_ptrGameAssembly = mono_domain_assembly_open(m_ptrMonoDomain, pathStr.c_str());
		if (m_ptrGameAssembly) {
			m_ptrGameAssemblyImage = mono_assembly_get_image(m_ptrGameAssembly);
			if (m_ptrGameAssemblyImage) {
				mono_add_internal_call("Scripts.Script::CreateCubeObject", &CSBind::CS_CreateObj);
				mono_add_internal_call("Scripts.AudioComponent::InternalOnRegister", &CSBind::CS_AudioOnCreate);
			}
		}
	}
}

MonoImage* MonoSystem::GetImage()
{
	return m_ptrGameAssemblyImage;
}
