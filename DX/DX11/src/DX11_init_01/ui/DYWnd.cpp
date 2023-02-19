
#include "ui/DYWnd.h"
#include "ui/resource.h"
#include <functional>
using namespace std::placeholders;

DYWnd::DYWnd(HINSTANCE hInstance, int nCmdShow)
	: m_pHInstance(hInstance),
	m_nCmdShow(nCmdShow)
{
}

DYWnd::~DYWnd()
{
	ReleaseWnd();
}

HRESULT DYWnd::InitWnd()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &DYWnd::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_pHInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "DYWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));
	if (!RegisterClassEx(&wcex))
	{
		return E_FAIL;
	}

	// Create window
	RECT rt = { 0,0,800,600 };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
	m_pHWnd = CreateWindow("DYWindowClass", "Direct3D 11 Tutorial 1: Direct3D 11 Basics",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rt.right - rt.left, rt.bottom - rt.top,
		nullptr, nullptr, m_pHInstance, nullptr);
	if (!m_pHWnd)
	{
		return E_FAIL;
	}

	ShowWindow(m_pHWnd, m_nCmdShow);

	return S_OK;
}

void DYWnd::ReleaseWnd()
{
	DestroyWindow(m_pHWnd);
	m_pHWnd = nullptr;
}

HWND DYWnd::GetWnd()
{
	return m_pHWnd;
}

LRESULT DYWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// Note that this tutorial does not handle resizing (WM_SIZE) requests,
		// so we created the window without the resize border.

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
