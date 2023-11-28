#include "stdafx.h"
#include "Camera_Dynamic.h"
#include "GameInstance.h"

CCamera_Dynamic::CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice,pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic& rhs)
	:CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype(LEVEL eLevel)
{
	m_eCurrentLevelID = eLevel;
	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void* pArg)
{
	if(nullptr == pArg)
		return E_FAIL;
	
	DYNAMIC_CAMERA_DESC* pDesc = (DYNAMIC_CAMERA_DESC*)pArg;

	m_fMouseSensitivity = pDesc->fMouseSensitivity;

	if(FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Dynamic::Priority_Tick(_float fTimeDelta)
{
}

void CCamera_Dynamic::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (m_pGameInstance->Key_Up(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (m_pGameInstance->Key_Down(DIK_S))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	


	//TODO �θ��� Tick�Լ��� ȣ������� ����������� ���������� ��ü���� �����ش�.
	__super::Tick(fTimeDelta);
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
}

CCamera_Dynamic* CCamera_Dynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevel)
{
	CCamera_Dynamic* pInstance = new CCamera_Dynamic(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX("Failed to Created : CCamera_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic* pInstance = new CCamera_Dynamic(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();
}
