
#include <Windows.h>

class DYWnd
{
public:
	DYWnd(HINSTANCE hInstance = nullptr, int nCmdShow = 0);
	~DYWnd();

	HRESULT InitWnd();
	void ReleaseWnd();

	HWND GetWnd();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE m_pHInstance = nullptr;
	int m_nCmdShow = 0;
	HWND m_pHWnd = nullptr;

};