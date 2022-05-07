#pragma once
#include "BaseEnemy.h"

class Chaser : public BaseEnemy
{
public:
	/// <summary>
	/// �ǐՓG����
	/// </summary>
	/// <param name="spawnPosition">�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>�ǐՓG</returns>
	static Chaser* Create(XMFLOAT3 spawnPosition);

	/// <summary>
	/// �ǐՓG�̃��f�����Z�b�g
	/// </summary>
	/// <param name="chaserModel1">�������f��</param>
	/// <param name="chaserModel2">������шЗ�1�̃��f��</param>
	/// <param name="chaserModel3">������шЗ�2�̃��f��</param>
	/// <param name="chaserModel4">������шЗ�3�̃��f��</param>
	static void SetModel(Model* chaserModel1, Model* chaserModel2, Model* chaserModel3, Model* chaserModel4);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>����</returns>
	bool Initialize(XMFLOAT3 spawnPosition, float moveDegree) override;

	/// <summary>
	/// �m�b�N�o�b�N�̏����Z�b�g
	/// </summary>
	/// <param name="angle">������Ԋp�x</param>
	/// <param name="power">�m�b�N�o�b�N�̋���</param>
	void SetKnockBack(float angle, int powerLevel, int shockWaveGroup) override;

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
	/// �^�[�Q�b�g�̌����ɐi�s�p�x���C��
	/// </summary>
	void SetAngleForTarget(XMFLOAT3 targetPosition);

	/// <summary>
	/// �ړ����x�ύX
	/// </summary>
	void ChangeMoveSpeed();

private:
	//���f��
	static const int modelNum = 4;
	static Model* chaserModel[modelNum];

private:
	//�ړ����x�̃^�C�}�[
	int moveSpeedTimer = 0;
};