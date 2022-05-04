#pragma once
#include "BaseEnemy.h"

class Chaser : public BaseEnemy
{
public:
	/// <summary>
	/// �ǐՓG����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="spawnPosition">�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>�ǐՓG</returns>
	static Chaser* Create(Model* model, XMFLOAT3 spawnPosition);

	/// <summary>
	/// �^�[�Q�b�g���W���Z�b�g
	/// </summary>
	/// <param name="targetPos"></param>
	static void SetTargetPos(XMFLOAT3 targetPos) { Chaser::targetPos = targetPos; };

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

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move() override;

	/// <summary>
	/// �^�[�Q�b�g�̌����ɐi�s�p�x���C��
	/// </summary>
	void SetAngleForTarget(XMFLOAT3 targetPosition);

	/// <summary>
	/// �ړ����x�ύX
	/// </summary>
	void ChangeMoveSpeed();

private:
	//�^�[�Q�b�g���W
	static XMFLOAT3 targetPos;

private:
	//�ړ����x�̃^�C�}�[
	int moveSpeedTimer = 0;
};