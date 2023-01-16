#pragma once

#include "AssetManager.h"
#include "../Actor.h"
#include "../MonoSystem.h"
#include "InputDevice.h"
#include "Serializer.h"

class MonoMasterModule
{
public:
    MonoMasterModule()
    {
        mono_add_internal_call("Scripts.Internal.ExternalApi::IsMouseDown", &IsMouseDown);
        mono_add_internal_call("Scripts.Engine.LevelLoader::LoadLevelInternal", &LoadLevel);
    }

private:
    static bool IsMouseDown(int button)
    {
        return Game::GetInstance()->GetInputDevice()->GetMouse()->IsDown(static_cast<Button>(button));
    }
    
    static void LoadLevel(MonoObject* name)
    {
        auto filename = mono_string_to_utf8(mono_object_to_string(name, nullptr));
        auto game = Game::GetInstance();
        auto currentLevel = game->GetAssetManager()->GetProjectRootPath() / filename;
        
        game->AddPendingFunction([currentLevel]()
        {
            Serializer::ReadFromFile(currentLevel, Game::GetInstance(), true);
            Game::GetInstance()->MyEditorContext.SetSelectedActor(nullptr);
        });
    }
};
