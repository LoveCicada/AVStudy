
#include "render/DYDX11.h"
#include <iterator>
#include <DirectXColors.h>

using namespace DirectX;

DYDX11::DYDX11(HWND hWnd)
	: m_pHWnd(hWnd)
{

}

DYDX11::~DYDX11() 
{
	Release();
}

bool DYDX11::Init()
{
	bool bRet = true;
	bRet = InitDevice();
	if (!bRet)
	{
		// to do
		return false;
	}
	return bRet;
}

void DYDX11::Release()
{
	CleanDevice();
}

bool DYDX11::InitDevice()
{
	bool bRet = true;

	bRet = CheckDevice();
	if (!bRet)
	{
		// to do
		return false;
	}

	bRet = CreateD3DDevice();
	if (!bRet)
	{
		// to do
		return false;
	}

	bRet = CreateRenderObj();
	if (!bRet)
	{
		// to do
		return false;
	}

	bRet = SetViewPort();
	if (!bRet)
	{
		// to do
		return false;
	}

	return bRet;
}

void DYDX11::CleanDevice()
{
	if (m_pImmediateContext)
	{
		m_pImmediateContext->ClearState();
	}

	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
	}

	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
	}

	if (m_pSwapChain1)
	{
		m_pSwapChain1->Release();
	}

	if (m_pImmediateContext1)
	{
		m_pImmediateContext1->Release();
	}

	if (m_pImmediateContext)
	{
		m_pImmediateContext->Release();
	}

	if (m_pD3DDevice1)
	{
		m_pD3DDevice1->Release();
	}

	if (m_pD3DDevice)
	{
		m_pD3DDevice->Release();
	}
}

bool DYDX11::CheckDevice()
{
	bool bRet = true;

	if (!m_pHWnd)
		return false;

	GetWndRtInfo();

#ifdef _DEBUG
	m_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG


	m_D3DdriverTypes[0] = D3D_DRIVER_TYPE_HARDWARE;
	m_D3DdriverTypes[1] = D3D_DRIVER_TYPE_WARP;
	m_D3DdriverTypes[2] = D3D_DRIVER_TYPE_REFERENCE;

	m_D3DFeatureLevels[0] = D3D_FEATURE_LEVEL_11_1;
	m_D3DFeatureLevels[1] = D3D_FEATURE_LEVEL_11_0;
	m_D3DFeatureLevels[2] = D3D_FEATURE_LEVEL_10_1;
	m_D3DFeatureLevels[3] = D3D_FEATURE_LEVEL_10_0;

	return bRet;
}

bool DYDX11::CreateD3DDevice()
{
	bool bRet = true;

	HRESULT hr;

	for (auto driverTypeItem : m_D3DdriverTypes)
	{
		m_D3DDriverType = driverTypeItem;
		hr = D3D11CreateDevice(nullptr, m_D3DDriverType, nullptr,
			m_createDeviceFlags, m_D3DFeatureLevels, std::size(m_D3DFeatureLevels),
			D3D11_SDK_VERSION, &m_pD3DDevice, &m_D3DFeatureLevel, &m_pImmediateContext);

		if (E_INVALIDARG == hr)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, m_D3DDriverType, nullptr,
				m_createDeviceFlags, &m_D3DFeatureLevels[1], std::size(m_D3DFeatureLevels) - 1,
				D3D11_SDK_VERSION, &m_pD3DDevice, &m_D3DFeatureLevel, &m_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
	{
		// 
		return false;
	}

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
	{
		// to do
		return false;
	}

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = m_pD3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pD3DDevice1));
		if (SUCCEEDED(hr))
		{
			hr = m_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pImmediateContext1));
			if (FAILED(hr))
			{
				// to do

				return false;
			}
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = m_width;
		sd.Height = m_height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(m_pD3DDevice, m_pHWnd, &sd,
			nullptr, nullptr, &m_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = m_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
			if (FAILED(hr))
			{
				// to do
				return false;
			}
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = m_width;
		sd.BufferDesc.Height = m_height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_pHWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(m_pD3DDevice, &sd, &m_pSwapChain);
		if (FAILED(hr))
		{
			// to do
			return false;
		}
	}
	
	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	hr = dxgiFactory->MakeWindowAssociation(m_pHWnd, DXGI_MWA_NO_ALT_ENTER);
	if (FAILED(hr))
	{
		// to do
		return false;
	}
	dxgiFactory->Release();

	return bRet;
}

bool DYDX11::CreateRenderObj()
{
	bool bRet = true;

	HRESULT hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
	{
		// to do
		return false;
	}

	hr = m_pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	if (FAILED(hr))
	{
		// to do
		return false;
	}
	pBackBuffer->Release();
	
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

	return bRet;
}

bool DYDX11::SetViewPort()
{
	bool bRet = true;

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_width;
	vp.Height = (FLOAT)m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	m_pImmediateContext->RSSetViewports(1, &vp);

	return bRet;
}

void DYDX11::Render()
{
	if (m_pImmediateContext && m_pSwapChain)
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::MistyRose);
		m_pSwapChain->Present(0, 0);
	}
}

void DYDX11::GetWndRtInfo()
{
	GetClientRect(m_pHWnd, &m_rt);
	m_width = m_rt.right - m_rt.left;
	m_height = m_rt.bottom - m_rt.top;
}