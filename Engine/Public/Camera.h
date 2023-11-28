#pragma once

#include "GameObject.h"


//TODO ī�޶� Ŭ������ ����
//! Ŭ���̾�Ʈ �����ڰ� ������� �ϴ� ī�޶���� �������� �θ� �Ǳ����� Ŭ����

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public tagGameObjectDesc
	{
		_float4 vEye, vAt;
		_float	fFovY, fAspect, fNear, fFar;
	}CAMERA_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;

protected:
	//TODO ��������� ����ִ� ����, eye�� at�� ��������
	//! ���� ��������� ����ִ� ������ �׻� ���� ������ �ƴ϶� �̾߱�. ���� ���濡���� ó�� �Ѵٿ� ����ȴ�.
	//! eye�� at���� ��쿡�� �ѹ��� �ְ� ���߿� Transform ������Ʈ�� ������ ���̱⶧���� ��������� ���� �ʴ´�.

	//! ��������� ��������� �����͸� ���������� ������ ��. ���������鼭 ��������� ������̴�. ���� �ٲ�� �翬�� �ٲ����.
	_float			m_fFovy = { 0.0f };
	_float			m_fAspect = { 0.0f };
	_float			m_fNear = { 0.0f };
	_float			m_fFar = { 0.0f };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void		 Free() override;

};

END