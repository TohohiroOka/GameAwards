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
	/// <param name="chaserModel">���f��</param>
	static void SetModel(Model* chaserModel);

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
	/// �^�[�Q�b�g�̌����ɐi�s�p�x���C��
	/// </summary>
	void SetAngleForTarget(XMFLOAT3 targetPosition);

	/// <summary>
	/// �ړ����x�ύX
	/// </summary>
	void ChangeMoveSpeed();

private:
	//���f��
	static Model* chaserModel;

private:
	//�ړ����x�̃^�C�}�[
	int moveSpeedTimer = 0;
};