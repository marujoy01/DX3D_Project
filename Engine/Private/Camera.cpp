#include "Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice,pContext)
{
}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	if(nullptr == pArg)
		return E_FAIL;
	
	//TODO �θ�Ŭ������ ���ӿ�����Ʈ�� Initialize �Լ��� ȣ���ߴٸ� Transform ������Ʈ�� ������ٴ� �ǹ̿͵� ����.
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CAMERA_DESC*	pCameraDesc = (CAMERA_DESC*)pArg;

	//! ������ ���� Ʈ�������� �׵������ ���̴�. ���� �����ϰ��� �ϴ� ī�޶��� ���·� ����ȭ��������.
	//! ������ ó���� XMLoadFloat3�� �ַ��ߴ�. ������ �׷����ϸ� W���� 0���� ä�����⶧���� �ȵȴ� �ٽ� Float4 ���·� �ٲ����.
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pCameraDesc->vEye)); //! ī�޶��� ��ġ�ϱ� Transform�� POSITION�� ���ε�
	m_pTransformCom->Look_At(XMLoadFloat4(&pCameraDesc->vAt)); //! vAt�� ī�޶� �ٶ󺸴� �����̴� Transform�� Look_At �Լ� ȣ���ؼ� �ٶ󺸰� ������.
	
	m_fFovy = pCameraDesc->fFovY;
	m_fAspect = pCameraDesc->fAspect;
	m_fNear = pCameraDesc->fNear;
	m_fFar = pCameraDesc->fFar;
	
	//! 32,33�� ������ ���� vEye,vAt�� ��������� �����ʴ� ������ �� �� �ִ�. �ѹ��� �����ҰŴϱ� Ʈ�������� �ְ� Ʈ���������� �����ϱ� ���� ���ε� �� ���̴�.

	//TODO ������� ������ ���ǵ�
	//! ī�޶� ���� ����� ������� ī�޶��� �� ����̴�. ��� ��ü�� �� ���(ī�޶� ���)�� ���ľ� �ϴµ�, ������Ʈ�Ŵ������� ī�޶� ������Ʈ�� Ž�� �� 
	//! Ʈ�������� �����ͼ� Ʈ�������� ������ �ִ� ��������� ����� ������ �����ϴ°͵� ������ �ϰ�����, �� ��, ��ü���� �׷��������� �˻��� �ؾ��Ѵ�.
	//! �׷��� �ٸ���ü���� ���� ��� ��������� ���� �� �ִ� �۾��� �ʿ��ߴ�.
	//! dx9���� �׷��ȵ���̽��� GetTransform �Լ��� ȣ���ؼ� ��,������ ���� �� �־���.
	//! �� ������ ������� �����������̶�� Ŭ������ ���� ���̴�.
	return S_OK;
}

void CCamera::Priority_Tick(_float fTimeDelta)
{
}

void CCamera::Tick(_float fTimeDelta)
{
	//! ���������ο��� �ڱ��ڽ�(ī�޶�) ����� (���� �����), ������� ��������
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrixInverse());
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar));
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}


void CCamera::Free()
{
	__super::Free();
}
