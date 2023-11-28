#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iNumVertexBuffers = 1; //! ���߿��� �������� ���۸� ����Ҽ� �־. ����� 1���� ���
	m_iNumVertices = 4; //! �簢���̴ϱ� ������ ������ 4��
	m_iStride = sizeof(VTXPOSTEX); //! ���� �Ѱ��� ũ��� �� ������ ������ �־���� ������ ������(����ü)

	m_iNumIndices = 6; //! �簢���� �ﰢ������ �ٲܲ���. [0,1,2], [0,2,3] �� 6����

	//TODO �ε��� ����� ���� ����
	//! ���� �ε����� ������� 2 ����Ʈ�� 4 ����Ʈ �� �߿� �ϳ��� ����� �����ϴ�.
	//! 2 ����Ʈ�� ��쿡�� unsigned short�� ������, 4 ����Ʈ�� ��쿡�� unsigned int�� ����� ���Ѵ�.
	//! ��, ������ ������ unsigned short�� ���� �� �ִ� �ִ� ���� 65535�� �Ѵ´ٸ� ����� 4�� ����ϰ�, �ƴ϶�� 2�� �شٴ� ���
	
	m_iIndexStride = 2; //! ���⼭�� ������ ������ 65535�� ���� ���� ���̴� 2�� �� ���̴�.

	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


#pragma region VERTEX_BUFFER 
//TODO ���� ������ ���� ������ �־��ִ� ����

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc); //! �ʱ�ȭ

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices; //! �Ҵ��� ũ�⸦ ���Ѵ�.
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/; //! ���� ���۸� ����Ѵ�
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //! �������� ���� ä�� �Ŵ�
	m_BufferDesc.CPUAccessFlags = 0; //! ���� ������ ��쿡�� ��ȿ�� ���̴�. �������۴� 0���� ä���
	m_BufferDesc.MiscFlags = 0; //! ���� ����

	//todo StructureByteStride : ����� �Ҵ�� ������ �����͸� ������ � ������ ���� ���̳�
	m_BufferDesc.StructureByteStride = m_iStride; //! ���� ũ�� ������ �����Ŵ�.

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);


	//TODO ������ ���� �̾߱�
	//! ������ �ٲ۴ٴ� ���� ���� ������ �迭���� �����ؼ� ���� �������� ������ �ٲ��� ���� ���Ѵ�.
	//! �츮�� ������ ������������ �ϴ� ������, ���� ������ �����͸� �����ؼ� �װ� ��ȯ���Ѽ� ����ϴ� ����.
	//! ��, ������ ������������ ������ �ٲٴ� ������ �ƴ϶�, ��ȯ�ؼ� ����ϴ� ����


	//TODO ���ع���
	//! �ٷ� �Ʒ����� VTXPOSTEX ����ü�� �Ҵ��ϴ� ������ m_SubResourceData.pSysMem�� ���� ä����� �ϴ� ����
	//! ���� ���۸� ������ٴ� ������ �ƴ϶�� ��.
	//! �׷��� ���� ä���ְ� 71�� ���ο��� ��¥�� ���۸� ���� �ѵ� �ʿ���������� �����ϴ� ���̴�.

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];

	//TODO �������� ������ 1 �������� �����ϴ� ����
	//! ���� ������ �߾��� 0,0 �̴�. 
	//! ���Ͱ��� 1�������� ��������� ���߿� ������ ���� �ٶ� ����� ������ �ʿ���� ���ϴ� ���� �� �� �ִ�
	//! ex) m_pTransform->Set_Scale(15.f,15.f,15.f), ���� 1������ �ƴ϶� 2�����̾��ٸ� �������� 30�� �Ǿ��� ��. 
	//! 1�����̶� �������� 15�� �Ǵ� ���̴�.
	//! ��, -0.5, 0, +0.5. ���� �������� �� ���� �� ���̸� 1�� ������̴�.
	

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f); //! ���� ��
	pVertices[0].vTexcoord = _float2(0.0f, 0.f); //! ���� ��

	//TODO vTexcoord = UV��ǥ�� ���� �� = 0,0 ������ �Ʒ� = 1, 1
	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.f); //! ������ ��

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.0f); //! ������ �Ʒ�

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.0f);//! ���� �Ʒ�

	m_SubResourceData.pSysMem = pVertices;

	//! pVertices�� �Ҵ��Ͽ� ä������ �������� ������ ID3D11Buffer�� �Ҵ��� ������ �����Ͽ� ä���ִ´�.
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

//TODO ���� ������ ���� ������ �־��ִ� ���� ��
#pragma endregion

#pragma region INDEX_BUFFER
//TODO �ε��� ������ ���� ������ �־��ִ� ���� ��
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; //! �ε��� ���� ���� ä�ﲨ��
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	_ushort* pIndices = new _ushort[m_iNumIndices]; //! ���� ������ 65535 �ȳ����ϱ� _ushort

	pIndices[0] = 0; //! �簢�� ���� ������ �ð���� �������� ù��° �ﰢ�� 
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0; //! �簢�� ���� ������ �ð���� �������� �ι�° �ﰢ�� 
	pIndices[4] = 2;
	pIndices[5] = 3;

	//! �簢�� �ϼ�

	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

//TODO �ε��� ������ ���� ������ �־��ִ� ���� ��
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}
