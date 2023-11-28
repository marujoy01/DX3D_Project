#pragma once

#include "Renderer.h"
#include "PipeLine.h"
#include "Input_Device.h"
#include "Component_Manager.h"

/* Ŭ���̾�Ʈ���� ������ ����� ����ϱ����� �ݵ�� ���ľ��ϴ� ��ü. */

/* DX11�� ���� �ʿ��� ��ü�� �����ϰ� �������ϴ� �۾��� �����Ѵ�. */
/* ���� ������ ����(��)�� �����ϰ� �ʿ信���� ������Ʈ ����. */
/* ���� ������ ��ü���� �����ϰ� ������Ʈ�ϰ� �����Ѵ�. */
/* ���� ������ ���� ������Ʈ���� ������ �����ϰ� �����Ͽ� �����´�. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.Engine */
	/* �������̺귯���� ����ϱ����� �غ� ��� ��ģ��. */
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Tick_Engine(_float fTimeDelta);
	HRESULT  Render_Engine();
	void Clear(_uint iLevelIndex);

public: /* For.Graphic_Device */		
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();
	HRESULT UseFullScreen(_bool bMode);
	HRESULT Resize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
public: /* For.Timer_Manager */
	HRESULT	Add_Timer(const wstring& strTimeTag);
	_float Compute_TimeDelta(const wstring& strTimeTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iCurrentLevelIndex, class CLevel* pNewLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring & strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring & strPrototypeTag, void* pArg);

public: /* For.Renderer */
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eGroupID, class CGameObject* pGameObject);
	
public: /* For.PipeLine */
	void		Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	void		Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix);

	_matrix		Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4	Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState);

	_matrix		Get_TransformMatrixInverse(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4	Get_TransformFloat4x4Inverse(CPipeLine::D3DTRANSFORMSTATE eState);

	_float4		Get_CamPosition();

public: /* For.Input_Device */
	void	Set_hWnd_hInst(HINSTANCE hInst, HWND hWnd);
	_byte   Get_DIKeyState(_ubyte byKeyID);
	_byte   Get_DIMouseState(CInput_Device::MOUSEKEYSTATE eMouse);
	_long   Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE eMouseState);
	_bool	Key_Pressing(_ubyte byKeyID);
	_bool	Key_Down(_ubyte byKeyID);
	_bool	Key_Up(_ubyte byKeyID);
	_bool	Mouse_Pressing(CInput_Device::MOUSEKEYSTATE eMouse);
	_bool	Mouse_Down(CInput_Device::MOUSEKEYSTATE eMouse);
	_bool	Mouse_Up(CInput_Device::MOUSEKEYSTATE eMouse);



private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };

private:
	//#�����ν��Ͻ��ڵ�
	HWND	  m_hWnd = { nullptr };
	HINSTANCE m_hInst = { nullptr };


public:
	void Release_Manager();
	static void Release_Engine();
};

END