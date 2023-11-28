#pragma once
#include "Component.h"
//TODO ������

//! OR ���ְų� �ɾ��ְų� �����ְ�
 
//TODO ������
//! AND Ž�� -> �߰� -> ����


BEGIN(Engine)

class CBehaviorTree final : public CComponent
{
private:
	CBehaviorTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBehaviorTree(const CBehaviorTree& rhs);
	virtual ~CBehaviorTree() = default;

public:
	virtual HRESULT Initialize_Prototype(); //! ������Ÿ�� ������ ����Ѵٴ� ���� �� �� �ִ�
	virtual HRESULT Initialize(_uint iNumArg, void* pArgArray);
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;

private:
	BT::Builder*	 m_pBuilder = { nullptr };
	shared_ptr<Node> m_pTree;

public:
	static	CBehaviorTree*	Create();
	virtual CComponent*		Clone(_uint iNumArg, void* pArg);
	virtual void			Free() override;
	
};

END

