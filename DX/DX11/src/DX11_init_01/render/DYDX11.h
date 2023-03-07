
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
	* 1. 定义要检查的设备类型和特征级别 
	* 2. 创建d3d设备, 渲染环境和交换链
	* 3. 创建渲染对象
	* 4. 设置视口观察区(viewport)
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
	// 显卡, 用于创建资源, 例如: 资源类(ID3D11Resource, 包含纹理和缓冲区) 视图类 着色器
	ID3D11Device*			m_pD3DDevice = nullptr;
	ID3D11Device1*			m_pD3DDevice1 = nullptr;
	// 渲染管线, 负责渲染和计算工作, 需要绑定来自与它关联的D3D设备所创建的各种资源、视图和着色器才能运转;
	// 还负责对资源的直接读写操作
	ID3D11DeviceContext*	m_pImmediateContext = nullptr;
	ID3D11DeviceContext1*	m_pImmediateContext1 = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;
	IDXGISwapChain1*		m_pSwapChain1 = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
};