
#include <Windows.h>
#include <stdlib.h>
#include <stdint.h>
#include "ui/DYWnd.h"
#include "render/DYDX11.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT ret = E_FAIL;
	bool bRet = false;

	DYWnd dyWnd(hInstance, nCmdShow);
	ret = dyWnd.InitWnd();
	if (FAILED(ret))
	{
		return 0;
	}

	DYDX11 dyDX11(dyWnd.GetWnd());
	bRet = dyDX11.Init();
	if (!bRet)
	{
		// to do
		return -1;
	}

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// to do
			dyDX11.Render();
		}
	}



	return (int)msg.wParam;
}