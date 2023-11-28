#pragma once
#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT Initialize(const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext); //! �׷��� ����̽� �ʱ�ȭ
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);//! ����۸� ����� ���� �Լ� _float4�� ���Ӱ� typedef �� �ڷ������� XMFLOAT4 �ڷ����� �ǹ��Ѵ�.
	HRESULT Clear_DepthStencil_View();//! ���̹��ۿ� ���ٽ� ���۸� ����� ���� �Լ�
	HRESULT Resize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT UseFullScreen(_bool bMode);
	HRESULT Present(); //! �ĸ� ���۸� ������۷� ��ü�ϴ� �Լ�( �̸� �׷����� �� ���۸� ȭ�鿡 ���� �����ش�. ) 
	

private:
	ID3D11Device*			m_pDevice = { nullptr }; //! �޸� �Ҵ� ( ��������, �ε�������, �ؽ��ķε� ) �İ�ü�� ������ ���õ� ����
	ID3D11DeviceContext*	m_pContext = { nullptr }; //! ��ɽ��� ( ���ε� �۾�, �������۸� SetStreamSource(), SetIndices(), SetTransform(), SetTexture()
	
	IDXGISwapChain*			m_pSwapChain = { nullptr }; //! �ĸ���ۿ� ������۸� ��ü�ذ��鼭 ȭ�鿡 �����ִ� ����

	ID3D11RenderTargetView*		m_pBackBufferRTV = { nullptr }; //! ����Ÿ��(�׷��� ���)������ ���� �� �ִ�  �ؽ���Ÿ��..
	ID3D11DepthStencilView*		m_pDepthStencilView = { nullptr }; //! ���̽��ٽǹ���(���Ƿ� ������ �ȼ����� ������?)�μ� ���� �� �ִ� Ÿ��.

private:
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);


public:
	static CGraphic_Device* Create(const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	virtual void Free() override;
};

END


//TODO IDirect3DTexture9 -> { ID3D11Texture2D, ID3D11ShaderResourceView, ID3D11RenderTargetView, ID3D11DepthStencilView }
//!ID3D11Texture2D = �ؽ��ĸ� ǥ���ϴ� ������ü Ÿ���̴�. ������ü�� ����� ���� -> �뵵�� �´� ���������� ����ϱ����� �ؽ��İ�ü�� ����������.
//!ID3D11ShaderResourceView = ���̴��� ���޵� �� �ִ� �ؽ���Ÿ��.
//!ID3D11RenderTargetView = ����Ÿ�ٿ����� ���� �� �ִ�  �ؽ���Ÿ��.
//!ID3D11DepthStencilView = ���̽��ٽǹ��۷μ� ���� �� �ִ� Ÿ��.


//TODO ����ü�ο��� �ʼ������� �ʿ��� �����ʹ� ����۰� �ʿ��Ͽ�. �Ͽ� ����۸� �����ϱ����� ������ �����ش�.
//! ����ü���� ������� == �����(�ؽ���)�� �����ȴ�.
//! ���� Direct9 �����ӿ�ũ������ Begin_Scene, End_Scene�� �������, ���� �ѹ��� Ŭ���� �����ʰ�, Ŭ�����ϴ� �κ��� ������ �ϰ��ִ�.