#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;	//! �����ν��Ͻ� Ŭ������ Engine ������Ʈ�� �ִ� Ŭ����. Ŭ���̾�Ʈ���� ��������� �����ν��Ͻ��� �ּҰ��� �������� ���� ���漱��.
END

BEGIN(Client)

class CMainApp final  : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta); //! ���� �����ӿ�ũ���� ����ϴ� Update �Լ��� �����̴�. ���� �����ӿ�ũ������ Tick���� ����ؼ� ���.
	HRESULT Render();

private:
	//! Direct9 �������� ����ߴ� IDirect3DDevice9 == LPDIRECT3DDEVICE9 �İ�ü�� ����� Direct11���������� ID3D11Device, ID3D11DeviceContext �ΰ��� �ɰ���.
	
	//! ID3D11Device : ����� ��ŭ ������ �Ҵ� ���ش�  (ex. �ؽ�ó��������, ũ������Ʈ���ؽ����� )
	//! ��, ������ ��� ������ �Ҵ��Ѵٴ� ������ ID3D11Device ��ü�� �����Ѵ�.
	
	
	//! ID3D11DeviceContext : ID3D11Device ��ü�� �����ϴ� ���� �Ҵ��� ������ ��Ÿ ������� �κе��� �����Ѵ�.
	//! ��ġ�� �̿��ؼ� ������ �׸����� �� ��, � ���� �׸����� �ϴ����� ��ġ�� ���ε��ϴ� ���� ����
	
	//! �İ�ü�� �ΰ��� ������ ����ϴ� ���� ���� : ��� ��ü�� ID3D11Device�� �������ٸ� �� ��ü�� �Ҵ��� ���ִ±���, ID3D11DeviceContext�� �������ٸ� �� ��ü�� ���ε��ϰ� �׸����� �ϴ±���.��� ����.
	
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };
	CGameInstance*					m_pGameInstance = { nullptr };

private:
	HRESULT Open_Level(LEVEL eStartLevelID);
	HRESULT Ready_Prototype_Component_ForStaticLevel();

public:
	static CMainApp* Create(); //! Create �Լ��� ���� �ڽ� ��ü���� �����ϰ� �ִ� �Լ��̴�. �ֻ��� �θ��Լ�(Abstract)�� ���� ��üȭ �� �� ���⿡ ������ ���� �ʴ�.
	virtual void Free();
};

END