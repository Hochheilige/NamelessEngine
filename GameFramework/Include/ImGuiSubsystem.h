#pragma once

#include "MathInclude.h"
#include "imgui.h"
class Game;

class ImGuiSubsystem
{
public:
	auto Initialize(Game* const InGame) -> void;

	auto NewFrame() -> void;
	auto Render() -> void;
	auto EndFrame() -> void;

	auto DoLayout() -> void;

	auto Shutdown() -> void;

	~ImGuiSubsystem();

private:
	auto DrawDockspace() -> void;
	auto DrawViewport() -> void;
	auto DrawActorExplorer() -> void;
	auto DrawActorInspector() -> void;
private:
	Game* MyGame;

	Vector2 ViewportMousePos;

	ImVec2 ViewportStart;
	ImVec2 ViewportSize;
};
