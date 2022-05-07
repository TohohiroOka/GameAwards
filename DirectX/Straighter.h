#pragma once
#include "BaseEnemy.h"

class Straighter : public BaseEnemy
{
public:
	/// <summary>
	/// ���i�G����
	/// </summary>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>���i�G</returns>
	static Straighter* Create(XMFLOAT3 spawnPosition, float moveDegree, int knockBackPowerLevel = 0);

	/// <summary>
	/// ���i�G�̃��f�����Z�b�g
	/// </summary>
	/// <param name="straighterModel1">�������f��</param>
	/// <param name="straighterModel2">������шЗ�1�̃��f��</param>
	/// <param name="straighterModel3">������шЗ�2�̃��f��</param>
	/// <param name="straighterModel4">������шЗ�3�̃��f��</param>
	static void SetModel(Model* straighterModel1, Model* straighterModel2, Model* straighterModel3, Model* straighterModel4);

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
	/// �e�̃m�b�N�o�b�N�̋����������p��
	/// </summary>
	/// <param name="knockBackPowerLevel">�m�b�N�o�b�N�̋���</param>
	void SetParentKnockBackPowerLevel(int knockBackPowerLevel);

private:
	//���f��
	static const int modelNum = 4;
	static Model* straighterModel[modelNum];
};