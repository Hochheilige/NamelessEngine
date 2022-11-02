#include "MonoActor.h"

MonoActor::MonoActor()
{
    auto mono = MonoSystem::GetInstance();
    auto image = mono->GetImage();
    auto klass = mono->FindClass("Scripts", "Actor");
    CsInstance = mono->CreateClassInstance(klass);
}

void MonoActor::AddComponent(MonoComponent* component)
{
    
}
