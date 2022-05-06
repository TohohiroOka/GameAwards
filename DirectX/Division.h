#pragma once
#include "BaseEnemy.h"

class Division : public BaseEnemy
{
public:
	/// <summary>
	/// �����G����
	/// </summary>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>�����G</returns>
	static Division* Create(XMFLOAT3 spawnPosition, float moveDegree);

	/// <summary>
	/// �����G�̃��f�����Z�b�g
	/// </summary>
	/// <param name="divisionModel1">�������f��</param>
	/// <param name="divisionModel2">������шЗ�1�̃��f��</param>
	/// <param name="divisionModel3">������шЗ�2�̃��f��</param>
	/// <param name="divisionModel4">������шЗ�3�̃��f��</param>
	static void SetModel(Model* divisionModel1, Model* divisionModel2, Model* divisionModel3, Model* divisionModel4);

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
	/// <param name="power">�m�b�N�o�b�N�̋���</param>
	void SetKnockBack(float angle, int powerLevel, int shockWaveGroup) override;

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move() override;

	/// <summary>
	/// ��ʓ��ɂ��邩�`�F�b�N
	/// </summary>
	bool CheckInScreen();

	/// <summary>
	/// ���E����
	/// </summary>
	void ReflectionX();

	/// <summary>
	/// �㉺����
	/// </summary>
	void ReflectionY();

	/// <summary>
	/// �������ԍX�V
	/// </summary>
	void AliveTimeUpdate();

private:
	//���f��
	static const int modelNum = 4;
	static Model* divisionModel[modelNum];

private:
	//�����������ԃ^�C�}�[
	int aliveTimer = 0;
};