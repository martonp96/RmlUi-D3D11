#pragma once
#include "utils/CSingleton.h"

class CAppManager : public CSingleton<CAppManager>
{
	bool isRunning;
	HWND window;
	int width;
	int height;

public:
	void Create(int _width, int _height);
	void WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool IsRunning();
	void HandleMessages();

	int GetWidth()
	{
		return width;
	}

	int GetHeight()
	{
		return height;
	}

	HWND GetWindow()
	{
		return window;
	}

	void Msg(const char* message, ...);
};