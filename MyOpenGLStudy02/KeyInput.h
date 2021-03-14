#pragma once
#include <map>
#include <GLFW/glfw3.h>

struct KeyCode
{
public:
	short keycode;
	short lastState;
	short currentState;

	bool IsPress() const;
	bool IsHold() const;
};

class KeyInput
{
public:
	std::map<short, KeyCode> keys;

	void Update();
	inline KeyCode Insert(short key);
	bool IsPress(short key);
	bool IsHold(short key);
};
