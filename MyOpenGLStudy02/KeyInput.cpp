#include "KeyInput.h"
#include <Windows.h>

bool KeyCode::IsPress() const
{
	return lastState >= 0 && currentState < 0;
}

//-128/-127 press    0 1 hold
bool KeyCode::IsHold() const
{
	return currentState < 0;
}

//如果用opengl的 keycallback存在一定的延迟
//所以自己写一个key event
void KeyInput::Update()
{
	for (auto&& key : keys)
	{
		key.second.lastState = key.second.currentState;
		key.second.currentState = GetKeyState(key.first);
	}
}

KeyCode KeyInput::Insert(short key)
{
	auto kc = KeyCode{key, -128, GetKeyState(key)};
	keys.insert(std::make_pair(key, kc));
	return kc;
}

bool KeyInput::IsPress(short key)
{
	const auto it = keys.find(key);
	if (it == keys.end())
	{
		return Insert(key).IsPress();
	}
	else
	{
		return (*it).second.IsPress();
	}
}

bool KeyInput::IsHold(short key)
{
	const auto it = keys.find(key);
	if (it == keys.end())
	{
		return Insert(key).IsHold();
	}
	else
	{
		return (*it).second.IsHold();
	}
}
