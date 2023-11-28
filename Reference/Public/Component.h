#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(); //! ������Ÿ�� ������ ����Ѵٴ� ���� �� �� �ִ�
	virtual HRESULT Initialize(void* pArg);
	virtual void	Priority_Tick(_float fTimeDelta) {};
	virtual void	Tick(_float fTimeDelta) {};
	virtual void	Late_Tick(_float fTimeDelta) {};

protected:
	//TODO ������Ʈ�� ��ġ�� ����־���ϴ� ����
	//! ������ �Ҵ��ϰų�(ex. Buffer) �ؽ�ó�� �ҷ��´�(ex. Texture) ���� �ؾ��ϱ� ������ ������ ������ �����ϱ�

	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	//! �Ʒ��� ������Ʈ�� ����ϴ� ������ ���캸�� Ŭ���� �����ϴ� ������ �� �� �ִ�
	virtual CComponent* Clone(void* pArg) = 0; 
	virtual void Free() override;

};

END

//TODO ������Ʈ�� ����ϴ� ����

//! ����, �÷��̾�� ���Ǵ� Knight, Archer Ŭ������ �ְ�, ���ͷ� ���Ǵ� OrcWarrior, OrcArcher Ŭ������ ���� ��
//! �÷��̾�� ���Ǵ� Ŭ�������� Ű�Է°� ���õ� �Լ��� �ʿ��ϰ�, ���ͷ� ���Ǵ� Ŭ�������� �ΰ������� �ۼ������ �ϴ� �Լ��� �ʿ��ϴ�.
//! �θ�Ŭ������ ����ؼ� is a ��Ӱ��踦 ������ָ� ������, Knight Ŭ������ OrcWarrior Ŭ������ �������� �ٰŸ� Ÿ���̰�, Archer, OrcArcher Ŭ������ ���Ÿ� Ÿ���� ����
//! �� �ѹ� �б�Ǵ� ���� �� �� �ִ�.
//! is a ��Ӱ��踦 ����� �ֱ⿡�� ����� ������ ���� ��찡 �ִ�.
//! ����Ʈ���� ���߿��� ����ϴ� ��ӱ����� �������ڴ� ���� �ʿ����� ��ü�� ����� �� �� �����Ͽ� ��� ����� ������� ó���ϱ⿡�� ������ ���� ���� ���δ�.
//! �׷��� ������ ���� ������ ��ӱ����� ����������� ������ ������ �������� �� ���� �������ߴ�.
//! is a ���谡 �ƴ� has a ���踦 ����ϴ� ��. ( �Ļ� ��Ű�� ���� �ƴ� �����ϴ� ��ӱ����� �����ڴ� �� )
//! Knight Ŭ������ ��� Ű�Է°� ���õ� �Լ�, �ٰŸ�Ÿ���� ���õ� �Լ��� �ʿ��ߴ�.
//! Knight Ŭ������ ��������� �ٰŸ�Ÿ�� ��ü�� ����� �Ҵ�, Ű�Է¿� ���� ��ü�� ����� �Ҵ��ؼ� ���Խ�Ű�� �͵� �����ٴ� �ǰ�.
//! ��, ��ǰ�� ���� ���õ� ����� ���� Ŭ������ ���Խ��Ѽ� ������ڴ� �Ϳ��� ���õ� ������ �����̴�. ( Ư�� �ϼ���ǰ�� ����� ���� ��ɸ� ���� �� Ŭ���� )
