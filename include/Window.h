#pragma once

#ifdef UNICODE
	#undef UNICODE
#endif

#include <Windows.h>

struct WindowProps
{
	int width;
	int height;
	const char *title;
};

class Window
{
public:
	Window(const WindowProps &props);
	~Window();

	void Show();
	void SwapBuffers();

	// Returns true if events should continue to be polled.
	// if a WM_QUIT message has been posted, returns false.
	//
	bool PollEvents();
private:
	WindowProps m_Props;
	HDC m_DeviceContext;
	HWND m_Handle;

private:
	void CreateGLContext(); // TODO: Delegate to graphics device class
	void DestroyGLContext(); // 

private:
	static bool s_Initialized;

	static LRESULT Procedure(
		_In_ HWND hWnd,
		_In_ UINT uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam);
};