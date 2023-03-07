
#include <d3d11.h>
#include <d3d11_1.h>
#include <array>

class DYDX11
{
public:
	DYDX11(HWND hWnd = nullptr);
	~DYDX11();

	bool Init();
	void Release();

	/*
	* @brief 
	* 1. ����Ҫ�����豸���ͺ��������� 
	* 2. ����d3d�豸, ��Ⱦ�����ͽ�����
	* 3. ������Ⱦ����
	* 4. �����ӿڹ۲���(viewport)
	*/
	bool InitDevice();
	void CleanDevice();

	bool CheckDevice();
	bool CreateD3DDevice();
	bool CreateRenderObj();
	bool SetViewPort();

	void Render();

protected:
	void GetWndRtInfo();

private:
	HWND m_pHWnd = nullptr;
	RECT m_rt{0};
	UINT m_width = 0;
	UINT m_height = 0;

	UINT					m_createDeviceFlags = 0;
	D3D_DRIVER_TYPE			m_D3DdriverTypes[3] = { D3D_DRIVER_TYPE_UNKNOWN };
	D3D_FEATURE_LEVEL		m_D3DFeatureLevels[4] = { D3D_FEATURE_LEVEL_1_0_CORE };

	D3D_DRIVER_TYPE         m_D3DDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_D3DFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	// �Կ�, ���ڴ�����Դ, ����: ��Դ��(ID3D11Resource, ��������ͻ�����) ��ͼ�� ��ɫ��
	ID3D11Device*			m_pD3DDevice = nullptr;
	ID3D11Device1*			m_pD3DDevice1 = nullptr;
	// ��Ⱦ����, ������Ⱦ�ͼ��㹤��, ��Ҫ����������������D3D�豸�������ĸ�����Դ����ͼ����ɫ��������ת;
	// ���������Դ��ֱ�Ӷ�д����
	ID3D11DeviceContext*	m_pImmediateContext = nullptr;
	ID3D11DeviceContext1*	m_pImmediateContext1 = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;
	IDXGISwapChain1*		m_pSwapChain1 = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
};