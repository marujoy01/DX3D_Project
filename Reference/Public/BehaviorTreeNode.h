#pragma once
#include "Engine_Defines.h"

//TODO ���ϴ� ���ε�?
//! ���ѻ��¸ӽ�(FSM)�� �Ѱ踦 �غ��ϰ��� ���ȵ� ���

//TODO ��� ���µ�?

//! Ʈ���� ������Ҵ� ������ ������
//! ��Ʈ���, ��Ʈ���÷ο� ���(��Ʈ��, �������ƴϴ�), �ͽ�ť�ǳ��(���� ���. �½�Ʈ����� �Ѵ�)
//! ���� �켱 Ž�� ( DFS )
//! ��Ʈ�� �÷ο���� �ڱ� �ڽ� ��� �Ʒ��� �ִ� ����Ʈ��(�귣ġ����� ��)�� ���� ǥ���Ǵ� �ൿ(���� �½�ũ)�� �����ϴ� ��.
//! Selector ���� Sequence ��尡 �̿� �ش�ȴ� 



//! ������ Action(leaf Node), Selector, Sequance 3���� ��带 �⺻������ ������ �ִ�.
//! �� ������ �ڽ��� ���¸� ��ȯ�ؾ��Ѵ�.
//! ���´� ��� �����ϳĿ� ���� �ٸ���. ��κ� �Ʒ� 3������ �����ȴ�.
//! Fail(����)
//! Running(���� ��) ���� ���� Running�� ��ȯ�� ��尡 �ٽ� ȣ��ȴ�.
//! Success(����)

//TODO Action ���
//! �ൿ�� ������ ����̴�. �׷��⿡ Ʈ���� leaf node�� �־���Ѵ�.

//TODO Selector ���
//! or �����ڿ� ������ ����� �ϴ� ����̴�.
//! �ڽ� ������ ���ʿ��� ������ ������ �����Ѵ�.
//! �켱������ ���� �ڽ� ��� �� ���� ���ʿ� ��ġ�Ͽ��� �Ѵ�.
//! �ڽ� ���� �� ������ ��尡 �ִٸ� �� ��带 �����ϰ� �����Ѵ�.
//! ���⼭ �����̶� Success �� Running�� ���Ѵ�.
//! ���� �ൿ �� �ϳ��� �����ؾ� �� ���� �����ϸ� �ȴ�.
//! ���÷� �� �ֱ�, ����, �ɱ� ���� �ִ�.

//TODO Sequence ���
//! and �����ڿ� ������ ����� �ϴ� ����̴�.
//! �ڽ� ���� �� ������ ��尡 ���� �� ���� �����Ѵ�.
//! ���� �ൿ�� ������� �����ؾ� �� ���� �����ϸ� �ȴ�.
//! ���÷� �� Ž�� -> ������ �̵� -> ���� ���� �ִ�.

//TODO ����� Ʈ�������̴�
//!������ �Ʒ���, ���ʿ��� ������ ������ Ž���Ѵ�.

BEGIN(Engine)

class CBTNode
{
public:
	virtual void Execute(class CBlackBoard& blackBoard) = 0;
};

END