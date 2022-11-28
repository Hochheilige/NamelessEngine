#include "Component.h"
#include "Game.h"

Component::Component(): id(Game::GetInstance()->GetUuidGenerator()->generate())
{
	
}
