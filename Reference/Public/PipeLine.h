#pragma once
//TODO ���������� ��ü�� ����
//! �ΰ��� ���� ��ü���� �׸� �� �������� ���Ǵ� ��, ������ĵ��� �����Ѵ�.

#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	//TODO �����, ����뿡 ���� �����ε�
	void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	void Set_Transform(D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix);

public:
	_matrix Get_TransformMatrix(D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(D3DTRANSFORMSTATE eState);

	_matrix Get_TransformMatrixInverse(D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4Inverse(D3DTRANSFORMSTATE eState);

	_float4 Get_CamPosition();

public:
	HRESULT Initialize();
	//TODO ƽ�Լ�?
	//! �����ν��Ͻ��� ƽ���� �Լ����� ȣ���� ���̴�. �� ƽ���� ī�޶� �������ִ� ��, ��������� Set���� ��.
	void	Tick();

private:
	//TODO ��������� �迭�� ����� ����
	//! �迭�� �ȵξ��ٸ� ������̳� ��������̳Ŀ� ���� ���Ǻб⸦ �߾���ߴ�. �װ� �Ⱦ��� �ۿ��� enum���� �����ָ� �����´� ����� Get,Set �ϰ�;���.
	_float4x4		m_Transform[D3DTS_END];

	//! 4X4Ÿ���� ������� ���ϴ� �۾��� �������� ���̴�. �׷��� �ʿ��Ҷ����� Get_TransformFloat4x4Inverse �Լ��� ȣ���ϱ� ���ٴ� 
	//! ���ʿ� Tick �Լ����� ������� ���ؼ� ��������� �����صδ� �������� ���°� ���Ҵ�.
	_float4x4		m_Transform_Inverse[D3DTS_END];

	//! ���߿� �� ������ �ϰԵǸ� ī�޶��� ���� ��ġ�� �䱸�ϴ� ��Ȳ�� ���� �����. �׶� ī�޶��� �����(ī�޶��� ����)�� 4��(������ġ)�� �Ѱ��ֱ� ���� ���������� �������ش�.
	_float4			m_vCamPosition;

public:
	static CPipeLine*	Create();
	virtual void		Free() override;
};

END

