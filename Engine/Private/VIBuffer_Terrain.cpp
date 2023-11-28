#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const wstring& strHeightMapFilePath)
{
	_ulong	dwByte = 0;
	HANDLE  hFile = CreateFile(strHeightMapFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(0 == hFile)
		return E_FAIL;

	//TODO ��Ʈ�� ������ 3�ܰ� ����
	//! 1. BITMAPFILEHEADER
	//! 2. BITMAPINFOHEADER
	//! 3. PIXEL
	
	//! �츰 ���̸�.bmp�� ���� �ȼ��� �����ðǵ� �׷����� 1,2�� ���� �о�� ���� �ȼ� �������ִ�.

	BITMAPFILEHEADER fh; //! ������ ��¥, ������ ��¥ ���� �����ִ� ��

	//! ��� �����ǵ�?, ������ ���� �����ðǵ�?, ������ ������ ������ ������� �󸸵�?, �о�� ����Ʈ�� ���?, �������� �ʿ����
	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);

	BITMAPINFOHEADER ih; //! ������ ����, �̹����� ���μ��λ�����(�ȼ��� ����) ���� �����ִ� ��

	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	_ulong*		pPixel = new _ulong[ih.biWidth * ih.biHeight]; //! �ȼ��� �翬�� ���λ����� * ���λ�����
	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);
	
	CloseHandle(hFile);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iNumVertexBuffers = 1; //! ���߿��� �������� ���۸� ����Ҽ� �־. ����� 1���� ���
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ; //! �ȼ��� ���� �����Ŷ��
	m_iStride = sizeof(VTXNORTEX); //! ������ �������Ҳ��� �׷��ϱ� ���� ������ NORMAL ���Խ���

	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3; //! �ε��� ���� ���?
	//! * 2�� �� �ϴ°ž�? : �簢�� �ϳ��׸��� �ﰢ�� �ΰ� �ʿ����ݾ�
	//! * 3��? : �ﰢ�� �ϳ��� �ε��� 3�� ���ݾ�.

	//TODO �ε��� ����� ���� ����
	//! ���� �ε����� ������� 2 ����Ʈ�� 4 ����Ʈ �� �߿� �ϳ��� ����� �����ϴ�.
	//! 2 ����Ʈ�� ��쿡�� unsigned short�� ������, 4 ����Ʈ�� ��쿡�� unsigned int�� ����� ���Ѵ�.
	//! ��, ������ ������ unsigned short�� ���� �� �ִ� �ִ� ���� 65535�� �Ѵ´ٸ� ����� 4�� ����ϰ�, �ƴ϶�� 2�� �شٴ� ���
	
	m_iIndexStride = m_iNumVertices >= 65535 ? 4 : 2; //! �������� 65535 ������ 4����Ʈ�� ��� �ƴϸ� 2�� ���ٴ� �������ǹ�

	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


#pragma region VERTEX_BUFFER 

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint	iIndex = i * m_iNumVerticesX + j; //! i�� 0�϶� 0,1,2,3,4,5 1�϶� ����X����, ����X����+1, ����X����+2, ����X����+3, ����X����+4

			//TODO 0x000000ff�� ����
			//! ���̸��� �׷��̽����Ϸ� �Ǿ��ִ�. ȸ���̶�� ���� RGB�� �����ϴٴ� ����̴�. ȸ������ ���̸� ǥ���ߴ�. y�� ���̰��� ä���ֱ����ؼ�
			//! �ȼ��� rgb�� �ƹ��ų� �Ѱ��� �������� �ƴµ�, ����Ʈ������ ���� �ʱ����ؼ� B�� �ش��ϴ� 0x000000ff�� ����ϴ� ��

			pVertices[iIndex].vPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.f, i); //! x �ϱ� j, y�� �ȼ��κ���, z�ϱ� i
			pVertices[iIndex].vNormal = _float3(0.0f, 0.0f, 0.f);
			
			//TODO UV��ǥ�� i�� j�� ����;���
			//! uv��ǥ�� 0 ~ 1�̴�. i�� j�� ������ 1025 X 1025��� ���� �� ���� 1025�̴�. �̰��� 0~1���� ������ �ٲ㼭 ����ߴ�.
			//! �׷��� j / (m_iNumVerticeX - 1.0f) ������ �������Ű� 1�̾ƴ϶� 1.0f ������ �� ������ 0~1���� ������ ����ϱ⶧���� �Ҽ��� ���ư��� �ȵƴ�.
			
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.0f), i / (m_iNumVerticesZ - 1.0f));
		}
	}

	Safe_Delete_Array(pPixel);

#pragma endregion

#pragma region INDEX_BUFFER
//TODO �ε��� ������ ���� ������ �־��ִ� ���� ��
	
	

	_uint* pIndices = new _uint[m_iNumIndices]; //! ���� ������ 65535 �����ϱ� 

	_uint  iNumIndices = 0;

	//TODO -1 �� ��?
	//! �� �ϸ� ���

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint	iIndex = i * m_iNumVerticesX + j; //! �Ʊ�� �Ȱ���~

			_uint iIndices[4] =
			{
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			//TODO �ﰢ�� ���� �����ϱ� �븻 �����ٰž�
			//! ���� ���� �ΰ� ���ؼ� �����Ҳ���. 0���� 1�ٶ󺸴� ���⺤�� �����, 1���� 2�ٶ󺸴� ���⺤�� ����°ž�
			//! dx11 �Լ����� ���͸� �����ϴ� �� ������

			vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Cross(vSourDir, vDestDir);

			//TODO ������ �븻 ���� �����ݾ�.
			//! ���� �븻 ���͸� ������ ������ �־������. �ϳ��� �븻���͸� �����Ҷ����� �븻������ ���ִ� �� Ű����Ʈ��
			//! �������͵��� ����� ���� ���� �븻������ ����� �����Ϸ��� �������͵��� ���̰� ��� ��������. ���� �������� �Ҷ� ���� �븻���͵��� ���̸� 1�� �������. ���̰� ����
			//! �������ڸ�, �׷��� �츮�� �븻������ ����� ����ؼ� ��Ȯ�� �������� ��ġ�� ������ ��� �븻���͸� ���� �� �ִ� �ž�.
			//! ����, ���̰� Ʋ�ȴٸ� ���� ������ �۾��� �߾�� �����ž�.
			
			vNormal = XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[0]].vNormal)));

			vNormal = XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[1]].vNormal)));

			vNormal = XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[2]].vNormal)));

			//TODO �簢���� �ﰢ�� �ΰ��� ���������ݾ� �ι�° �ﰢ���� �������
			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Cross(vSourDir, vDestDir);

			vNormal = XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[0]].vNormal)));

			vNormal = XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[2]].vNormal)));

			vNormal = XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[3]].vNormal)));
		}
	}
#pragma endregion

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc); //! �ʱ�ȭ

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices; //! �Ҵ��� ũ�⸦ ���Ѵ�.
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/; //! ���� ���۸� ����Ѵ�
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //! �������� ���� ä�� �Ŵ�
	m_BufferDesc.CPUAccessFlags = 0; //! ���� ������ ��쿡�� ��ȿ�� ���̴�. �������۴� 0���� ä���
	m_BufferDesc.MiscFlags = 0; //! ���� ����
	m_BufferDesc.StructureByteStride = m_iStride; //! ���� ũ�� ������ �����Ŵ�.

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; //! �ε��� ���� ���� ä�ﲨ��
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);



	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype(strHeightMapFilePath)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
