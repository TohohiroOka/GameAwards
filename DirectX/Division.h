#pragma once
#include "BaseEnemy.h"

class Division : public BaseEnemy
{
public:
	/// <summary>
	/// �����G����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>�����G</returns>
	static Division* Create(Model* model, XMFLOAT3 spawnPosition, float moveDegree);

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
	//�����������ԃ^�C�}�[
	int aliveTimer = 0;
};