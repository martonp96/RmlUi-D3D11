#include "main.h"
#include <varargs.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_SYSCOMMAND && wParam == SC_CLOSE)
		PostQuitMessage(0);

	CAppManager::Get()->WndProc(msg, wParam, lParam);
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

#define KEYMAP_SIZE 256
static Rml::Input::KeyIdentifier KeyIdentifierMap[KEYMAP_SIZE];

void InitialiseKeymap()
{
	// Initialise the key map with default values.
	memset(KeyIdentifierMap, 0, sizeof(KeyIdentifierMap));

	// Assign individual values.
	KeyIdentifierMap['A'] = Rml::Input::KI_A;
	KeyIdentifierMap['B'] = Rml::Input::KI_B;
	KeyIdentifierMap['C'] = Rml::Input::KI_C;
	KeyIdentifierMap['D'] = Rml::Input::KI_D;
	KeyIdentifierMap['E'] = Rml::Input::KI_E;
	KeyIdentifierMap['F'] = Rml::Input::KI_F;
	KeyIdentifierMap['G'] = Rml::Input::KI_G;
	KeyIdentifierMap['H'] = Rml::Input::KI_H;
	KeyIdentifierMap['I'] = Rml::Input::KI_I;
	KeyIdentifierMap['J'] = Rml::Input::KI_J;
	KeyIdentifierMap['K'] = Rml::Input::KI_K;
	KeyIdentifierMap['L'] = Rml::Input::KI_L;
	KeyIdentifierMap['M'] = Rml::Input::KI_M;
	KeyIdentifierMap['N'] = Rml::Input::KI_N;
	KeyIdentifierMap['O'] = Rml::Input::KI_O;
	KeyIdentifierMap['P'] = Rml::Input::KI_P;
	KeyIdentifierMap['Q'] = Rml::Input::KI_Q;
	KeyIdentifierMap['R'] = Rml::Input::KI_R;
	KeyIdentifierMap['S'] = Rml::Input::KI_S;
	KeyIdentifierMap['T'] = Rml::Input::KI_T;
	KeyIdentifierMap['U'] = Rml::Input::KI_U;
	KeyIdentifierMap['V'] = Rml::Input::KI_V;
	KeyIdentifierMap['W'] = Rml::Input::KI_W;
	KeyIdentifierMap['X'] = Rml::Input::KI_X;
	KeyIdentifierMap['Y'] = Rml::Input::KI_Y;
	KeyIdentifierMap['Z'] = Rml::Input::KI_Z;

	KeyIdentifierMap['0'] = Rml::Input::KI_0;
	KeyIdentifierMap['1'] = Rml::Input::KI_1;
	KeyIdentifierMap['2'] = Rml::Input::KI_2;
	KeyIdentifierMap['3'] = Rml::Input::KI_3;
	KeyIdentifierMap['4'] = Rml::Input::KI_4;
	KeyIdentifierMap['5'] = Rml::Input::KI_5;
	KeyIdentifierMap['6'] = Rml::Input::KI_6;
	KeyIdentifierMap['7'] = Rml::Input::KI_7;
	KeyIdentifierMap['8'] = Rml::Input::KI_8;
	KeyIdentifierMap['9'] = Rml::Input::KI_9;

	KeyIdentifierMap[VK_BACK] = Rml::Input::KI_BACK;
	KeyIdentifierMap[VK_TAB] = Rml::Input::KI_TAB;

	KeyIdentifierMap[VK_CLEAR] = Rml::Input::KI_CLEAR;
	KeyIdentifierMap[VK_RETURN] = Rml::Input::KI_RETURN;

	KeyIdentifierMap[VK_PAUSE] = Rml::Input::KI_PAUSE;
	KeyIdentifierMap[VK_CAPITAL] = Rml::Input::KI_CAPITAL;

	KeyIdentifierMap[VK_KANA] = Rml::Input::KI_KANA;
	KeyIdentifierMap[VK_HANGUL] = Rml::Input::KI_HANGUL;
	KeyIdentifierMap[VK_JUNJA] = Rml::Input::KI_JUNJA;
	KeyIdentifierMap[VK_FINAL] = Rml::Input::KI_FINAL;
	KeyIdentifierMap[VK_HANJA] = Rml::Input::KI_HANJA;
	KeyIdentifierMap[VK_KANJI] = Rml::Input::KI_KANJI;

	KeyIdentifierMap[VK_ESCAPE] = Rml::Input::KI_ESCAPE;

	KeyIdentifierMap[VK_CONVERT] = Rml::Input::KI_CONVERT;
	KeyIdentifierMap[VK_NONCONVERT] = Rml::Input::KI_NONCONVERT;
	KeyIdentifierMap[VK_ACCEPT] = Rml::Input::KI_ACCEPT;
	KeyIdentifierMap[VK_MODECHANGE] = Rml::Input::KI_MODECHANGE;

	KeyIdentifierMap[VK_SPACE] = Rml::Input::KI_SPACE;
	KeyIdentifierMap[VK_PRIOR] = Rml::Input::KI_PRIOR;
	KeyIdentifierMap[VK_NEXT] = Rml::Input::KI_NEXT;
	KeyIdentifierMap[VK_END] = Rml::Input::KI_END;
	KeyIdentifierMap[VK_HOME] = Rml::Input::KI_HOME;
	KeyIdentifierMap[VK_LEFT] = Rml::Input::KI_LEFT;
	KeyIdentifierMap[VK_UP] = Rml::Input::KI_UP;
	KeyIdentifierMap[VK_RIGHT] = Rml::Input::KI_RIGHT;
	KeyIdentifierMap[VK_DOWN] = Rml::Input::KI_DOWN;
	KeyIdentifierMap[VK_SELECT] = Rml::Input::KI_SELECT;
	KeyIdentifierMap[VK_PRINT] = Rml::Input::KI_PRINT;
	KeyIdentifierMap[VK_EXECUTE] = Rml::Input::KI_EXECUTE;
	KeyIdentifierMap[VK_SNAPSHOT] = Rml::Input::KI_SNAPSHOT;
	KeyIdentifierMap[VK_INSERT] = Rml::Input::KI_INSERT;
	KeyIdentifierMap[VK_DELETE] = Rml::Input::KI_DELETE;
	KeyIdentifierMap[VK_HELP] = Rml::Input::KI_HELP;

	KeyIdentifierMap[VK_LWIN] = Rml::Input::KI_LWIN;
	KeyIdentifierMap[VK_RWIN] = Rml::Input::KI_RWIN;
	KeyIdentifierMap[VK_APPS] = Rml::Input::KI_APPS;

	KeyIdentifierMap[VK_SLEEP] = Rml::Input::KI_SLEEP;

	KeyIdentifierMap[VK_NUMPAD0] = Rml::Input::KI_NUMPAD0;
	KeyIdentifierMap[VK_NUMPAD1] = Rml::Input::KI_NUMPAD1;
	KeyIdentifierMap[VK_NUMPAD2] = Rml::Input::KI_NUMPAD2;
	KeyIdentifierMap[VK_NUMPAD3] = Rml::Input::KI_NUMPAD3;
	KeyIdentifierMap[VK_NUMPAD4] = Rml::Input::KI_NUMPAD4;
	KeyIdentifierMap[VK_NUMPAD5] = Rml::Input::KI_NUMPAD5;
	KeyIdentifierMap[VK_NUMPAD6] = Rml::Input::KI_NUMPAD6;
	KeyIdentifierMap[VK_NUMPAD7] = Rml::Input::KI_NUMPAD7;
	KeyIdentifierMap[VK_NUMPAD8] = Rml::Input::KI_NUMPAD8;
	KeyIdentifierMap[VK_NUMPAD9] = Rml::Input::KI_NUMPAD9;
	KeyIdentifierMap[VK_MULTIPLY] = Rml::Input::KI_MULTIPLY;
	KeyIdentifierMap[VK_ADD] = Rml::Input::KI_ADD;
	KeyIdentifierMap[VK_SEPARATOR] = Rml::Input::KI_SEPARATOR;
	KeyIdentifierMap[VK_SUBTRACT] = Rml::Input::KI_SUBTRACT;
	KeyIdentifierMap[VK_DECIMAL] = Rml::Input::KI_DECIMAL;
	KeyIdentifierMap[VK_DIVIDE] = Rml::Input::KI_DIVIDE;
	KeyIdentifierMap[VK_F1] = Rml::Input::KI_F1;
	KeyIdentifierMap[VK_F2] = Rml::Input::KI_F2;
	KeyIdentifierMap[VK_F3] = Rml::Input::KI_F3;
	KeyIdentifierMap[VK_F4] = Rml::Input::KI_F4;
	KeyIdentifierMap[VK_F5] = Rml::Input::KI_F5;
	KeyIdentifierMap[VK_F6] = Rml::Input::KI_F6;
	KeyIdentifierMap[VK_F7] = Rml::Input::KI_F7;
	KeyIdentifierMap[VK_F8] = Rml::Input::KI_F8;
	KeyIdentifierMap[VK_F9] = Rml::Input::KI_F9;
	KeyIdentifierMap[VK_F10] = Rml::Input::KI_F10;
	KeyIdentifierMap[VK_F11] = Rml::Input::KI_F11;
	KeyIdentifierMap[VK_F12] = Rml::Input::KI_F12;
	KeyIdentifierMap[VK_F13] = Rml::Input::KI_F13;
	KeyIdentifierMap[VK_F14] = Rml::Input::KI_F14;
	KeyIdentifierMap[VK_F15] = Rml::Input::KI_F15;
	KeyIdentifierMap[VK_F16] = Rml::Input::KI_F16;
	KeyIdentifierMap[VK_F17] = Rml::Input::KI_F17;
	KeyIdentifierMap[VK_F18] = Rml::Input::KI_F18;
	KeyIdentifierMap[VK_F19] = Rml::Input::KI_F19;
	KeyIdentifierMap[VK_F20] = Rml::Input::KI_F20;
	KeyIdentifierMap[VK_F21] = Rml::Input::KI_F21;
	KeyIdentifierMap[VK_F22] = Rml::Input::KI_F22;
	KeyIdentifierMap[VK_F23] = Rml::Input::KI_F23;
	KeyIdentifierMap[VK_F24] = Rml::Input::KI_F24;

	KeyIdentifierMap[VK_NUMLOCK] = Rml::Input::KI_NUMLOCK;
	KeyIdentifierMap[VK_SCROLL] = Rml::Input::KI_SCROLL;

	KeyIdentifierMap[VK_OEM_NEC_EQUAL] = Rml::Input::KI_OEM_NEC_EQUAL;

	KeyIdentifierMap[VK_OEM_FJ_JISHO] = Rml::Input::KI_OEM_FJ_JISHO;
	KeyIdentifierMap[VK_OEM_FJ_MASSHOU] = Rml::Input::KI_OEM_FJ_MASSHOU;
	KeyIdentifierMap[VK_OEM_FJ_TOUROKU] = Rml::Input::KI_OEM_FJ_TOUROKU;
	KeyIdentifierMap[VK_OEM_FJ_LOYA] = Rml::Input::KI_OEM_FJ_LOYA;
	KeyIdentifierMap[VK_OEM_FJ_ROYA] = Rml::Input::KI_OEM_FJ_ROYA;

	KeyIdentifierMap[VK_SHIFT] = Rml::Input::KI_LSHIFT;
	KeyIdentifierMap[VK_CONTROL] = Rml::Input::KI_LCONTROL;
	KeyIdentifierMap[VK_MENU] = Rml::Input::KI_LMENU;

	KeyIdentifierMap[VK_BROWSER_BACK] = Rml::Input::KI_BROWSER_BACK;
	KeyIdentifierMap[VK_BROWSER_FORWARD] = Rml::Input::KI_BROWSER_FORWARD;
	KeyIdentifierMap[VK_BROWSER_REFRESH] = Rml::Input::KI_BROWSER_REFRESH;
	KeyIdentifierMap[VK_BROWSER_STOP] = Rml::Input::KI_BROWSER_STOP;
	KeyIdentifierMap[VK_BROWSER_SEARCH] = Rml::Input::KI_BROWSER_SEARCH;
	KeyIdentifierMap[VK_BROWSER_FAVORITES] = Rml::Input::KI_BROWSER_FAVORITES;
	KeyIdentifierMap[VK_BROWSER_HOME] = Rml::Input::KI_BROWSER_HOME;

	KeyIdentifierMap[VK_VOLUME_MUTE] = Rml::Input::KI_VOLUME_MUTE;
	KeyIdentifierMap[VK_VOLUME_DOWN] = Rml::Input::KI_VOLUME_DOWN;
	KeyIdentifierMap[VK_VOLUME_UP] = Rml::Input::KI_VOLUME_UP;
	KeyIdentifierMap[VK_MEDIA_NEXT_TRACK] = Rml::Input::KI_MEDIA_NEXT_TRACK;
	KeyIdentifierMap[VK_MEDIA_PREV_TRACK] = Rml::Input::KI_MEDIA_PREV_TRACK;
	KeyIdentifierMap[VK_MEDIA_STOP] = Rml::Input::KI_MEDIA_STOP;
	KeyIdentifierMap[VK_MEDIA_PLAY_PAUSE] = Rml::Input::KI_MEDIA_PLAY_PAUSE;
	KeyIdentifierMap[VK_LAUNCH_MAIL] = Rml::Input::KI_LAUNCH_MAIL;
	KeyIdentifierMap[VK_LAUNCH_MEDIA_SELECT] = Rml::Input::KI_LAUNCH_MEDIA_SELECT;
	KeyIdentifierMap[VK_LAUNCH_APP1] = Rml::Input::KI_LAUNCH_APP1;
	KeyIdentifierMap[VK_LAUNCH_APP2] = Rml::Input::KI_LAUNCH_APP2;

	KeyIdentifierMap[VK_OEM_1] = Rml::Input::KI_OEM_1;
	KeyIdentifierMap[VK_OEM_PLUS] = Rml::Input::KI_OEM_PLUS;
	KeyIdentifierMap[VK_OEM_COMMA] = Rml::Input::KI_OEM_COMMA;
	KeyIdentifierMap[VK_OEM_MINUS] = Rml::Input::KI_OEM_MINUS;
	KeyIdentifierMap[VK_OEM_PERIOD] = Rml::Input::KI_OEM_PERIOD;
	KeyIdentifierMap[VK_OEM_2] = Rml::Input::KI_OEM_2;
	KeyIdentifierMap[VK_OEM_3] = Rml::Input::KI_OEM_3;

	KeyIdentifierMap[VK_OEM_4] = Rml::Input::KI_OEM_4;
	KeyIdentifierMap[VK_OEM_5] = Rml::Input::KI_OEM_5;
	KeyIdentifierMap[VK_OEM_6] = Rml::Input::KI_OEM_6;
	KeyIdentifierMap[VK_OEM_7] = Rml::Input::KI_OEM_7;
	KeyIdentifierMap[VK_OEM_8] = Rml::Input::KI_OEM_8;

	KeyIdentifierMap[VK_OEM_AX] = Rml::Input::KI_OEM_AX;
	KeyIdentifierMap[VK_OEM_102] = Rml::Input::KI_OEM_102;
	KeyIdentifierMap[VK_ICO_HELP] = Rml::Input::KI_ICO_HELP;
	KeyIdentifierMap[VK_ICO_00] = Rml::Input::KI_ICO_00;

	KeyIdentifierMap[VK_PROCESSKEY] = Rml::Input::KI_PROCESSKEY;

	KeyIdentifierMap[VK_ICO_CLEAR] = Rml::Input::KI_ICO_CLEAR;

	KeyIdentifierMap[VK_ATTN] = Rml::Input::KI_ATTN;
	KeyIdentifierMap[VK_CRSEL] = Rml::Input::KI_CRSEL;
	KeyIdentifierMap[VK_EXSEL] = Rml::Input::KI_EXSEL;
	KeyIdentifierMap[VK_EREOF] = Rml::Input::KI_EREOF;
	KeyIdentifierMap[VK_PLAY] = Rml::Input::KI_PLAY;
	KeyIdentifierMap[VK_ZOOM] = Rml::Input::KI_ZOOM;
	KeyIdentifierMap[VK_PA1] = Rml::Input::KI_PA1;
	KeyIdentifierMap[VK_OEM_CLEAR] = Rml::Input::KI_OEM_CLEAR;
}

void CAppManager::Create(int _width, int _height)
{
	width = _width;
	height = _height;

	WNDCLASS wc = { };

	wc.lpfnWndProc = ::WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"app_window";

	RegisterClass(&wc);

	window = CreateWindowEx(0, wc.lpszClassName, L"RmlUi DirectX11", WS_OVERLAPPEDWINDOW, 100, 100, width, height, NULL, NULL, wc.hInstance, NULL);
	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

	isRunning = true;

	InitialiseKeymap();
}

static int GetKeyModifierState()
{
	int key_modifier_state = 0;

	// Query the state of all modifier keys
	if (GetKeyState(VK_CAPITAL) & 1)
	{
		key_modifier_state |= Rml::Input::KM_CAPSLOCK;
	}

	if (HIWORD(GetKeyState(VK_SHIFT)) & 1)
	{
		key_modifier_state |= Rml::Input::KM_SHIFT;
	}

	if (GetKeyState(VK_NUMLOCK) & 1)
	{
		key_modifier_state |= Rml::Input::KM_NUMLOCK;
	}

	if (HIWORD(GetKeyState(VK_CONTROL)) & 1)
	{
		key_modifier_state |= Rml::Input::KM_CTRL;
	}

	if (HIWORD(GetKeyState(VK_MENU)) & 1)
	{
		key_modifier_state |= Rml::Input::KM_ALT;
	}

	return key_modifier_state;
}

void CAppManager::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto context = CContextManager::Get()->GetContext();

	if (msg == WM_MOUSEMOVE)
	{
		const int& x = LOWORD(lParam);
		const int& y = HIWORD(lParam);

		auto m_IsMouseOver = (x >= 0 && x < 1280) && (y >= 0 && y <= 768);
		if (m_IsMouseOver)
		{
			context->ProcessMouseMove((x - 0) * 1.f, (y - 0) * 1.f, GetKeyModifierState());
		}
	}

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		context->ProcessMouseButtonDown(0, GetKeyModifierState());
		break;

	case WM_LBUTTONUP:
		context->ProcessMouseButtonUp(0, GetKeyModifierState());
		break;

	case WM_RBUTTONDOWN:
		context->ProcessMouseButtonDown(1, GetKeyModifierState());
		break;

	case WM_RBUTTONUP:
		context->ProcessMouseButtonUp(1, GetKeyModifierState());
		break;

	case WM_MBUTTONDOWN:
		context->ProcessMouseButtonDown(2, GetKeyModifierState());
		break;

	case WM_MBUTTONUP:
		context->ProcessMouseButtonUp(2, GetKeyModifierState());
		break;

	case WM_MOUSEWHEEL:
		context->ProcessMouseWheel(static_cast<float>((short)HIWORD(wParam)) / static_cast<float>(-WHEEL_DELTA), GetKeyModifierState());
		break;

	case WM_KEYDOWN:
		context->ProcessKeyDown(KeyIdentifierMap[wParam], GetKeyModifierState());
		break;

	case WM_KEYUP:
		context->ProcessKeyUp(KeyIdentifierMap[wParam], GetKeyModifierState());
		break;

	case WM_CHAR:
		static char16_t firstU16CodeUnit = 0;

		char16_t c = (char16_t)wParam;
		Rml::Character character = (Rml::Character)c;

		// Windows sends two-wide characters as two messages.
		if (c >= 0xD800 && c < 0xDC00)
		{
			// First 16-bit code unit of a two-wide character.
			firstU16CodeUnit = c;
		}
		else
		{
			if (c >= 0xDC00 && c < 0xE000 && firstU16CodeUnit != 0)
			{
				// Second 16-bit code unit of a two-wide character.
					// Encode a single code point as a UTF-8 string.
				//RMLUICORE_API String ToUTF8(Character character);

				// Encode an array of code points as a UTF-8 string.
				//RMLUICORE_API String ToUTF8(const Character * characters, int num_characters);

				Rml::String utf8 = Rml::StringUtilities::ToUTF8(character);
				character = Rml::StringUtilities::ToCharacter(utf8.data());
			}
			else if (c == '\r')
			{
				// Windows sends new-lines as carriage returns, convert to endlines.
				character = (Rml::Character)'\n';
			}

			firstU16CodeUnit = 0;

			// Only send through printable characters.
			if (((char32_t)character >= 32 || character == (Rml::Character)'\n') && character != (Rml::Character)127)
				context->ProcessTextInput(character);
		}
		break;
	}
}

bool CAppManager::IsRunning()
{
	return isRunning;
}

void CAppManager::HandleMessages()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			isRunning = false;
	}
}

void CAppManager::Msg(const char* message, ...)
{
	char buf[256];

	va_list va;
	va_start(va, message);

	vsprintf(buf, message, va);
	vprintf(message, va);
	printf("\n");
	va_end(va);

	MessageBoxA(window, buf, "", 0);
}