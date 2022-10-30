#pragma once

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
private:
	Game* MyGame;

};