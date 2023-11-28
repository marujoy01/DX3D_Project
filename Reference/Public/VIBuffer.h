#pragma once

#include "Component.h"

//TODO VIBuffer�� �ٽ� �θ�Ŭ������ ����� ����
//! ������ Ư�� ���� Ŭ�������� ������ �θ�Ŭ������ ����Ұǵ�
//! Ư�� ������ ���� ���ڸ� Rect, Cube, Terrain, Model ���� �ִ�
//! VI <-- Vertex �� Index�� ���ڸ� �аǵ�, ��� Ư�� ���� Ŭ�������� �翬�� ������ �ʿ��ϴ�.
//! ��ó�� �������ۿ� �ε������۸� ����Ŷ�� �θ�Ŭ�������� ���������� ������ �� �����ϱ �����̴�.

BEGIN(Engine)

//TODO VIBuffer = Vertex + Index + Buffer(�޸� ����)
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

public:
	//TODO ����, �ε���, �ν��Ͻ� ���۵��� �����Ѵ�.
	HRESULT Bind_VIBuffers();
	HRESULT Create_Buffer(_Inout_ ID3D11Buffer * *ppBuffer);
	//! �Ʒ� �ּ����� �����ϵ� ����Ÿ���� �����ϱ⿡ Create_Buffer �Լ� �ϳ��� �ѹ��� ó���� ����������.

protected:
	//TODO DX9�� �ٸ��� �������ۿ� �ε������۰� ����ϴ� ����Ÿ���� ID3D11Buffer�� ������ ���� �� �� �ִ�
	ID3D11Buffer* m_pVB = { nullptr }; //! ���ؽ� ���� ( ���� ����ü���� ���� )
	ID3D11Buffer* m_pIB = { nullptr }; //! �ε��� ���� ( �׸��� ������ ���� ������ �ε��� ���� == ���� ����

	//TODO ���� ���� ������� �ϴ� ������ �Ӽ��� �����Ѵ�.
	//! D3D11_BUFFER_DESC : ������������ �ε����������� , ���� �������� ���� �������� ���� ����
	//! �ڽ� ��ü���� ä�� �� �� �θ��� Create_Buffer() �Լ��� ȣ���ϴ� ������ �� ��.
	D3D11_BUFFER_DESC			m_BufferDesc; 

	//! D3D11_SUBRESOURCE_DATA : �Ʒ��� void* pSystem ���� �ʱⰪ�� ���ϴ� ���� �༭ ����� ��
	//! ���������� �ڽİ�ü���� ä�� ��
	
	//! typedef struct D3D11_SUBRESOURCE_DATA
	//! {
	//! 	const void* pSysMem;
	//! 	UINT SysMemPitch;
	//! 	UINT SysMemSlicePitch;
	//! } 	D3D11_SUBRESOURCE_DATA;
	
	
	D3D11_SUBRESOURCE_DATA		m_SubResourceData;

protected:
	_uint						m_iNumVertices = { 0 }; //! ���� ����
	_uint						m_iStride = { 0 }; //! ���� �ϳ��� ũ��
	_uint						m_iNumVertexBuffers = { 0 };

	_uint						m_iNumIndices = { 0 }; //! �ε��� ����
	_uint						m_iIndexStride = { 0 }; //! �ε��� �ϳ��� ũ��
	DXGI_FORMAT					m_eIndexFormat = {  };
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology = { };

	

public:
	virtual CComponent* Clone(void* pArg) = 0; //! ���縦 �����Ѵ�
	virtual void Free() override;
};

END

