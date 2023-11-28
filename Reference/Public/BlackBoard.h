#pragma once

#include "Component.h"

//TODO �ൿƮ��(Behavior Tree)���� ���Ǵ� �����̴�
//! Ű, ��� ���·� �����ȴ�.
//! �� Ű�� Ư�� ������ ������ ��Ÿ����
//! ���� Ű, ����� ������ ������ ������ ������Ʈ�� ����Ǿ� ���� ��ü(NPC,AI ĳ����)�� ���� ���¸� ��Ÿ����.
//! �������� ������ �ൿƮ�� ��忡�� ���Ǿ� ���� ������ �����ϰų� ������ ������ �� Ȱ��ȴ�.
//! ���� ���ڸ�, �� ĳ���Ͱ� �÷��̾ �����ϵ��� �ϴ� �ൿ Ʈ���� �����Ѵٰ� ��������.
//! �ൿƮ���� ���� �����忡�� �� ������ �о�ͼ� �÷��̾ �����ϴ� ���� �ൿ�� ������ �� �ִ�.

//TODO �׷��� �����尡 ���İ�
//! �ΰ������� �Ǵܿ� ����ϴ� �����͵��� ������ �ǹ��Ѵ�
//! NPC�� �ǻ� ������ ������ ������ ������� ���� �ȴٴ� ��.

//TODO �����غ��� � �ΰ������� �Ǵܿ� ���Ǵ� �����͵��� ���� ������
//! �������� ���� ���ڸ� ����(����)�� ��ġ�� ���°��� �ʿ��ϴ�
//! ���°��� ��? �׾�����, ��Ҵ���, ������������, ���Ż������� �˾ƾ� �� ���� ���ݾ�
//! 

//TODO Ű����� ������� �����?


BEGIN(Engine)

class CBlackBoard final : public CComponent
{
private:
	CBlackBoard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlackBoard(const CBlackBoard& rhs);
	virtual ~CBlackBoard() = default;

public:
	HRESULT		Initialize();
	
public:
	static  CBlackBoard* Create();
	virtual CComponent*  Clone(void* pArg) override;
	virtual void Free() override;

private:
	class CBTRoot*			m_pRootTree;
};

END
