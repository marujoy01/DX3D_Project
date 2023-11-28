#include "..\Public\Graphic_Device.h"

CGraphic_Device::CGraphic_Device()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
{
}

HRESULT CGraphic_Device::Initialize(const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	
	_uint iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D_FEATURE_LEVEL		FeatureLV;

	//TODO �׷��� ��ġ �ʱ�ȭ, D11������ Device �İ�ü�� Context �İ�ü�� ���� �� �������ش�. �� ������ ���� �� ���� �߾���.
	
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &FeatureLV, &m_pContext)))
		return E_FAIL;

	//TODO ����ü�� ��ü ����(������ؽ�ó ����), ����� ��, �������ٽ� �� ����
	
	//! �� ���۸� �����ϱ� ���� Texture2D ����.
	if(FAILED(Ready_SwapChain(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iBackBufferSizeX, GraphicDesc.iBackBufferSizeY)))
		return E_FAIL;

	//! ����ü���� ��� �ִ� �ؽ�ó 2D�� �����ͼ� �̸� �������� ����� ����Ÿ�� �� ����. ����Ÿ�� : �׷��� ��� == �����
	if(FAILED(Ready_BackBufferRenderTargetView()))
		return E_FAIL;

	//! �������ٽ� �� ����
	if(FAILED(Ready_DepthStencilRenderTargetView(GraphicDesc.iBackBufferSizeX, GraphicDesc.iBackBufferSizeY)))
		return E_FAIL;

	//TODO ��ġ�� ���ε��س��� ����Ÿ�ٵ��, �������ٽǺ並 ����. ��ġ�� �ִ� 8���� ����Ÿ�ٱ��� ���ÿ� ��� ���� �� �ִ�.
	
	ID3D11RenderTargetView*	pRTVs[1] = //! RTV == RenderTargetView�� ����. ����� ����Ÿ���� �� ���ۻ��̶� �迭�� ����� 1�� ����
	{
		m_pBackBufferRTV,
	};

	m_pContext->OMSetRenderTargets(1, pRTVs, m_pDepthStencilView);

	//TODO �� ��Ʈ ����
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0; //! ȭ�鿡 ���ʻ�ܺ��� �׸��ڴ�.
	ViewPortDesc.TopLeftY = 0; 
	ViewPortDesc.Width = (_float)GraphicDesc.iBackBufferSizeX;
	ViewPortDesc.Height = (_float)GraphicDesc.iBackBufferSizeY;
	ViewPortDesc.MinDepth = 0.f; //! ���̹��۴� 0 ���� 1���� ���� �����ϴ�. �� ���� ����.
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	*ppDeviceOut = m_pDevice;
	*ppContextOut = m_pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

HRESULT CGraphic_Device::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pContext)
		return E_FAIL;

	//m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
	//	vClearColor, 1.f, 0)
	//

	/* ����۸� �ʱ�ȭ�Ѵ�.  */
	m_pContext->ClearRenderTargetView(m_pBackBufferRTV, (_float*)&vClearColor);
	
	return S_OK;
}

HRESULT CGraphic_Device::Clear_DepthStencil_View()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphic_Device::Resize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//
	ID3D11RenderTargetView* pNullRTV = nullptr;

	m_pContext->OMSetRenderTargets(1, &pNullRTV, nullptr);

	ID3D11ShaderResourceView* pNullSRV = nullptr;
	
	m_pContext->PSSetShaderResources(0,1, &pNullSRV);

	m_pContext->VSSetShaderResources(0,1, &pNullSRV);

	m_pBackBufferRTV->Release();
	m_pDepthStencilView->Release();
	

	RECT ClientRect{};
	GetWindowRect(hWnd, &ClientRect);

	_uint iWinCX = static_cast<UINT>(ClientRect.right - ClientRect.left);
	_uint iWinCY = static_cast<UINT>(ClientRect.bottom - ClientRect.top);
	
	DXGI_SWAP_CHAIN_DESC desc{};
	m_pSwapChain->GetDesc(&desc);

	m_pSwapChain->ResizeBuffers(desc.BufferCount,iWinCX,iWinCY, desc.BufferDesc.Format, desc.Flags);

	//TODO �� ��Ʈ ����
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0; //! ȭ�鿡 ���ʻ�ܺ��� �׸��ڴ�.
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)iWinCX;
	ViewPortDesc.Height = (_float)iWinCY;
	ViewPortDesc.MinDepth = 0.f; //! ���̹��۴� 0 ���� 1���� ���� �����ϴ�. �� ���� ����.
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);


	Ready_BackBufferRenderTargetView();
	Ready_DepthStencilRenderTargetView(iWinCX,iWinCY);

	

	return S_OK;
}

HRESULT CGraphic_Device::UseFullScreen(_bool bMode)
{
	if (bMode)
 	{
 		DXGI_MODE_DESC modeDesc = {};
 		modeDesc.Width = 2560; // ���ο� �ʺ�
 		modeDesc.Height = 1440; // ���ο� ����
		modeDesc.RefreshRate.Numerator = 60; // �ֻ���
		modeDesc.RefreshRate.Denominator = 1;
 		modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �ȼ� ����
 		modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
 		modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
 
 		m_pSwapChain->ResizeTarget(&modeDesc);
 	}
 	else
 	{
 		DXGI_MODE_DESC modeDesc = {};
 		modeDesc.Width = 1280; // ���ο� �ʺ�
 		modeDesc.Height = 720; // ���ο� ����
 		modeDesc.RefreshRate.Numerator = 60; // �ֻ���
 		modeDesc.RefreshRate.Denominator = 1;
 		modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �ȼ� ����
 		modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
 		modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
 
 		m_pSwapChain->ResizeTarget(&modeDesc);
 	}
	
	return S_OK;
}

HRESULT CGraphic_Device::Present()
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;



	/* ���� ���ۿ� �ĸ���۸� ��ü�Ͽ� �ĸ���۸� �������� �����ִ� ������ �Ѵ�. */
	/* �ĸ���۸� ����ȭ�鿡 �����ٲ�. */
	return m_pSwapChain->Present(0, 0);
}

HRESULT CGraphic_Device::Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY)
{
	IDXGIDevice* pDevice = nullptr;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);

	IDXGIAdapter* pAdapter = nullptr;
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	IDXGIFactory* pFactory = nullptr;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	//TODO ����ü���� �����Ѵ�. = �ؽ��ĸ� �����ϴ� ���� + �����ϴ� ����

	DXGI_SWAP_CHAIN_DESC		SwapChain;
	ZeroMemory(&SwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	//! �ؽ���(�����)�� �����ϴ� ����
	SwapChain.BufferDesc.Width = iWinCX;
	SwapChain.BufferDesc.Height = iWinCY;

	SwapChain.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChain.BufferCount = 1;

	//! �����ϴ� ����
	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;
	SwapChain.SampleDesc.Quality = 0;
	SwapChain.SampleDesc.Count = 1;

	SwapChain.OutputWindow = hWnd;
	SwapChain.Windowed = eWinMode;
	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 

	//! ����۶�� �ؽ��ĸ� �����ߴ�.
	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &SwapChain, &m_pSwapChain)))
		return E_FAIL;



	Safe_Release(pFactory);
	Safe_Release(pAdapter);
	Safe_Release(pDevice);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_BackBufferRenderTargetView()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	/* ���� ������ ����ϱ����� �뵵�� �ؽ��ĸ� �����ϱ����� ���̽� �����͸� ������ �ִ� ��ü�̴�. */
	/* ���� ������ ����ϱ����� �뵵�� �ؽ��� : ID3D11RenderTargetView, ID3D11ShaderResoureView, ID3D11DepthStencilView */
	ID3D11Texture2D* pBackBufferTexture = nullptr;

	/* ����ü���� ����ִ� �ؽ�ó�� �����ͺ�. */
	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &m_pBackBufferRTV)))
		return E_FAIL;

	Safe_Release(pBackBufferTexture);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWinCX;
	TextureDesc.Height = iWinCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr,
		&pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr,
		&m_pDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

CGraphic_Device* CGraphic_Device::Create(const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	CGraphic_Device* pInstance = new CGraphic_Device();

	if (FAILED(pInstance->Initialize(GraphicDesc, ppDevice, ppContext)))
	{
		MSG_BOX("Failed to Created : CGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGraphic_Device::Free()
{
	Safe_Release(m_pSwapChain);
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pContext);

	//TODO �޸� ���� �߻��� �� ��ü�� � �İ�ü���� �˱����� MSDN���� ������ �ڵ�
#if defined(DEBUG) || defined(_DEBUG)
ID3D11Debug* d3dDebug;
HRESULT hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
if (SUCCEEDED(hr))
{
	OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
	OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");

	hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
	OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
}
if (d3dDebug != nullptr)            d3dDebug->Release();
#endif

	Safe_Release(m_pDevice);
}
