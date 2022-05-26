#pragma once
#include "BaseEnemy.h"

class Releaser : public BaseEnemy
{
public:
	/// <summary>
	/// ���o�G����
	/// </summary>
	/// <param name="spawnPosition">�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>���o�G</returns>
	static Releaser* Create(XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

	/// <summary>
	/// ���o�G�̃��f�����Z�b�g
	/// </summary>
	/// <param name="releaserModel">���f��</param>
	static void SetModel(Model* releaserModel);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>����</returns>
	bool Initialize(XMFLOAT3 spawnPosition, float moveDegree) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �m�b�N�o�b�N�̏����Z�b�g
	/// </summary>
	/// <param name="angle">������Ԋp�x</param>
	/// <param name="powerLevel">�m�b�N�o�b�N�̋���</param>
	/// <param name="powerMagnification">�m�b�N�o�b�N�̋����̔{��</param>
	void SetKnockBack(float angle, int powerLevel, float powerMagnification) override;

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move() override;

	/// <summary>
	/// ���U���g�V�[���p�̈ړ�
	/// </summary>
	void ResultMove() override;

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
	//���f��
	static Model* releaserModel;

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