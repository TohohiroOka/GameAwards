#pragma once
#include "BaseEnemy.h"

class Straighter : public BaseEnemy
{
public:
	/// <summary>
	/// ���i�G����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>���i�G</returns>
	static Straighter* Create(Model* model, XMFLOAT3 spawnPosition, float moveDegree, int knockBackPowerLevel = 0);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>����</returns>
	bool Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree) override;


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
	/// �e�̃m�b�N�o�b�N�̋����������p��
	/// </summary>
	/// <param name="knockBackPowerLevel">�m�b�N�o�b�N�̋���</param>
	void SetParentKnockBackPowerLevel(int knockBackPowerLevel);
};