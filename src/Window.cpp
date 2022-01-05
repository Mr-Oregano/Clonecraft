
#include "Window.h"

#include <iostream>

#include "glad/glad.h"
#include "wglext.h"

#define GL_CLASSNAME "GL-WINDOW"

bool Window::s_Initialized = false;

LRESULT Window::Procedure(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window(const WindowProps &props)
	: m_Props(props)
{
		// TODO: Save global hInstance
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	if (!s_Initialized)
	{
		WNDCLASSEX wc = {};

		wc.cbSize = sizeof(wc);
		wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wc.hInstance = hInstance;
		wc.lpfnWndProc = &Window::Procedure;
		wc.lpszClassName = GL_CLASSNAME;
		wc.style = CS_OWNDC;

		RegisterClassEx(&wc); // TODO: Check for register issues

		s_Initialized = true;
	}

	m_Handle = CreateWindowEx(
		0, GL_CLASSNAME, props.title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, props.width, props.height,
		nullptr, nullptr, hInstance, nullptr);

	m_DeviceContext = GetDC(m_Handle);

	CreateGLContext();

	glViewport(0, 0, props.width, props.height);
}

Window::~Window()
{
	DestroyWindow(m_Handle);
	DestroyGLContext();
}

void Window::CreateGLContext()
{
	PIXELFORMATDESCRIPTOR pfd = {};

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pfn = ChoosePixelFormat(m_DeviceContext, &pfd);

	SetPixelFormat(m_DeviceContext, pfn, &pfd);

	HGLRC context = wglCreateContext(m_DeviceContext);
	wglMakeCurrent(m_DeviceContext, context);

	auto wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	wglMakeCurrent(m_DeviceContext, nullptr);
	wglDeleteContext(context);

	const int attribList[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0 // End
	};

	int pixelFormat;
	UINT numFormats;

	wglChoosePixelFormatARB(m_DeviceContext, attribList, nullptr, 1, &pixelFormat, &numFormats);
	SetPixelFormat(m_DeviceContext, pixelFormat, &pfd);

	int gl_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0 // End
	};

	context = wglCreateContextAttribsARB(m_DeviceContext, nullptr, gl_attribs);
	wglMakeCurrent(m_DeviceContext, context);

	if (!gladLoadGL())
		std::cerr << "Failed to load required OpenGL functions" << std::endl;
}

void Window::DestroyGLContext()
{
	wglMakeCurrent(m_DeviceContext, NULL);
	wglDeleteContext(wglGetCurrentContext());
}

void Window::Show()
{
	ShowWindow(m_Handle, SW_NORMAL);
}

bool Window::PollEvents()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void Window::SwapBuffers()
{
	::SwapBuffers(m_DeviceContext);
}