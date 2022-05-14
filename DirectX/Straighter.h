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
	static Straighter* Create(XMFLOAT3 spawnPosition, float moveDegree);

	/// <summary>
	/// ���i�G�̃��f�����Z�b�g
	/// </summary>
	/// <param name="straighterModel">���f��</param>
	static void SetModel(Model* straighterModel);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>����</returns>
	bool Initialize(XMFLOAT3 spawnPosition, float moveDegree) override;

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

private:
	//���f��
	static Model* straighterModel;
};