#pragma once
#include <unordered_set>
#include <Windows.h>

class InputDevice;

class Keyboard
{
	friend InputDevice;
public:
	bool IsDown(uint64_t key);
private:
	void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);

	std::unordered_set<uint64_t> pressedKeys;
};

enum Keys
{
	KEY_ESC = 27,
	KEY_F1 = 112,
	KEY_F2 = 113,
	KEY_F3 = 114,
	KEY_F4 = 115,
	KEY_F5 = 116,
	KEY_F6 = 117,
	KEY_F7 = 118,
	KEY_F8 = 119,
	KEY_F9 = 120,
	// F10 - doesn't work
	KEY_F11 = 122,
	// F12 - triggers breakpoints
	KEY_PAUSE_BREAK = 19,
	KEY_GRAVE = 192,
	KEY_ONE = 49,
	KEY_TWO = 50,
	KEY_THREE = 51,
	KEY_FOUR = 52,
	KEY_FIVE = 53,
	KEY_SIX = 54,
	KEY_SEVEN = 55,
	KEY_EIGHT = 56,
	KEY_NINE = 57,
	KEY_ZERO = 48,
	KEY_MINUS = 189,
	KEY_PLUS = 187,
	KEY_BACKSLASH = 220,
	KEY_BACKSPACE = 8,
	KEY_TAB = 9,
	KEY_Q = 81,
	KEY_W = 87,
	KEY_E = 69,
	KEY_R = 82,
	KEY_T = 84,
	KEY_Y = 89,
	KEY_U = 85,
	KEY_I = 73,
	KEY_O = 79,
	KEY_P = 80,
	KEY_OPEN_BRACKET = 219,
	KEY_CLOSE_BRACKET = 221,
	KEY_ENTER = 13,
	KEY_CAPSLOCK = 20,
	KEY_A = 65,
	KEY_S = 83,
	KEY_D = 68,
	KEY_F = 70,
	KEY_G = 71,
	KEY_H = 72,
	KEY_J = 74,
	KEY_K = 75,
	KEY_L = 76,
	KEY_SEMICOLON = 186,
	KEY_SINGLE_QUOTE = 222,
	KEY_SHIFT = 16,
	KEY_Z = 90,
	KEY_X = 88,
	KEY_C = 67,
	KEY_V = 86,
	KEY_B = 66,
	KEY_N = 78,
	KEY_M = 77,
	KEY_COMMA = 188,
	KEY_PERIOD = 190,
	KEY_SLASH = 191,
	KEY_CTRL = 17,
	KEY_START = 91,
	//ALT doesn't work
	KEY_RIGHT_START = 92,
	KEY_MENU = 93,
	KEY_INSERT = 45,
	KEY_HOME = 36,
	KEY_PAGE_UP = 33,
	KEY_DELETE = 46,
	KEY_END = 35,
	KEY_PAGE_DOWN = 34,
	KEY_ARROW_LEFT = 37,
	KEY_ARROW_UP = 38,
	KEY_ARROW_DOWN = 40,
	KEY_ARROW_RIGHT = 39,
	KEY_NUM_LOCK = 144,
	KEY_NUM_SLASH = 111,
	KEY_NUM_STAR = 106,
	KEY_NUM_MINUS = 109,
	KEY_NUM_PLUS = 107,
	KEY_NUM_ZERO = 96,
	KEY_NUM_PERIOD = 110,
	KEY_NUM_ONE = 97,
	KEY_NUM_TWO = 98,
	KEY_NUM_THREE = 99,
	KEY_NUM_FOUR = 100,
	KEY_NUM_FIVE = 101,
	KEY_NUM_SIX = 102,
	KEY_NUM_SEVEN = 103,
	KEY_NUM_EIGHT = 104,
	KEY_NUM_NINE = 105,
	KEY_SPACE = 32,
};
