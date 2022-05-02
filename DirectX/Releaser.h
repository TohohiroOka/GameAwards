#pragma once
#include "BaseEnemy.h"

class Releaser : public BaseEnemy
{
public:
	/// <summary>
	/// ���o�G����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="spawnPosition">�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>���o�G</returns>
	static Releaser* Create(Model* model, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>����</returns>
	bool Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �m�b�N�o�b�N�̏����Z�b�g
	/// </summary>
	/// <param name="angle">������Ԋp�x</param>
	/// <param name="power">�m�b�N�o�b�N�̋���</param>
	void SetKnockBack(float angle, int powerLevel) override;

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move() override;

	/// <summary>
	/// �ړ��������~���W�Ɍ����Z�b�g
	/// </summary>
	void SetStayPosAngle();

	/// <summary>
	/// ��~���W���Z�b�g
	/// </summary>
	void SetStayPos(XMFLOAT3 stayPosition);

	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	 void KnockBack() override;

	/// <summary>
	/// ���o���[�h�̏���
	/// </summary>
	void ReleaseMode();

	/// <summary>
	/// �G���o
	/// </summary>
	void Release();

private:
	//��~���W
	XMFLOAT3 stayPos = {};
	//���o�^�C�}�[
	int releaseTimer = 0;
	//���o������
	int releaseCount = 0;
	//���o���[�h��
	bool isReleaseMode = false;
};