#include "Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice,pContext)
{
}

CTexture::CTexture(CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_SRVs(rhs.m_SRVs)
{
	for(auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	//TODO �ؽ����� ������ �������ִ�. �׷��� vector �����̳ʸ� ����Ѱ��̰�. �׷� reserve�� ����༭ ������� �޸� ������ �Ҵ����� �ʰ�������
	m_SRVs.reserve(m_iNumTextures);

	for (size_t i = 0; i < m_iNumTextures; i++)
	{
		/* ..\������\139\Framework\Client\Bin\Resources\Textures\Default.jpg */
		_tchar		szFullPath[MAX_PATH] = TEXT("");


		//TODO ���� Default%d ���ٸ� �ڿ� i�� �ٰ� �ƴҰ�쿡�� i�� ���� �ʴ´�. �׷��� Default.jpg�� �൵ �ƹ��� �ϵ� ���������ʾƼ� �״�� ��밡���� ��.
		wsprintf(szFullPath, strTextureFilePath.c_str(), i);

		/* D:\ */
		//TODO ����� ����̺갡 �������� �ޱ����� ���ڿ� �޸� ������ ���ø޸𸮿� �Ҵ��ߴ�. ������ ���� ����� ����̺갡 �������ʰ� ../ ����ΰ� ���ýÿ��� ���� �ȴ�.
		_tchar		szDrive[MAX_PATH] = TEXT("");

		/* ������\139\Framework\Client\Bin\Resources\Textures\ */
		//TODO ����̺긦 ������ ���丮 ��θ� �߶� �������ش�.
		_tchar		szDirectory[MAX_PATH] = TEXT("");

		/* Default0 */
		_tchar		szFileName[MAX_PATH] = TEXT("");

		/* .jpg */
		_tchar		szExt[MAX_PATH] = TEXT("");

		//TODO �� ����� szFullPath�� �Է°����� �ְ� �������� ��°��� �ޱ����� ������ �ָ�ȴ�. ������ Ȯ���ڸ� �˸� �Ǳ⿡ 0 �Ǵ� nullptr�� ä��� ��θ� ������ �ִ� ������ �־���.
		_wsplitpath_s(szFullPath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);


		//! CreateDDSTextureFromFile �Լ��� ���ؼ� ��ο��ִ� �ؽ�ó������ ���ҽ���� �ε��Ų��.
		ID3D11ShaderResourceView* pSRV = { nullptr };

		//! �������� ID3D11Texture2D �ؽ�ó ������ü�� ���� �� m_pDevice->CreateShaderResourceView �ϴ� ������ ���ҽ��並 ���������ߴ�.
		//! ������ CreateDDSTextureFromFile �Լ��� ���ο��� �˾Ƽ� ������ü�� ���ҽ��� ���� ������༭ ������ ���� �Լ���.
		 

		HRESULT			hr = 0;

		//! DDS Ȯ������ ��� �б�
		if (!lstrcmp(szExt, TEXT(".dds")))
		{
			hr = CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);
		}
		//! �츮�� �־��� TK(ToolKit)���̺귯���� tga �ؽ�ó�� �ε����� ���Ѵ�. ����ڿ��� �˸��� ���� �б�
		else if (!lstrcmp(szExt, TEXT(".tga")))
		{
			MSG_BOX("tga Ȯ���ڴ� �ε��� �� �����ϴ�.");
		}
		//! ������ ������ �⺻ Ȯ���ڵ� (png, jpg, bmp ���)�� ���� WIC�� �ش��Ѵ�.
		else
		{
			hr = CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);
			
		}

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	return pShader->Bind_SRV(pConstantName, m_SRVs[iTextureIndex]);
}

HRESULT CTexture::Bind_ShaderResourceArray(CShader* pShader, const _char* pConstantName)
{
	//! �迭�� ù��° �ּҴ� �ش� �迭�� �ּ��� �ش��ϱ⿡ ���������� ���ڷ� front�� �Ѱ��� ��.
	return pShader->Bind_SRVs(pConstantName, &m_SRVs.front(), m_iNumTextures);
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype(strTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for(auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();
}
