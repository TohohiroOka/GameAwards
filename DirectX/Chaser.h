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
	static Chaser* Create(XMFLOAT3 spawnPosition, float moveDegree = 0, bool isParent = false);

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
	/// �e���琶�܂ꂽ�ꍇ�̐ݒ�
	/// </summary>
	/// <param name="moveDegree"></param>
	void SetSpawnParent(float moveDegree);

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
	/// ��ʓ��ɂ��邩�`�F�b�N
	/// </summary>
	bool CheckInScreen();

	/// <summary>
	/// ���i�̓���
	/// </summary>
	void MoveStraight();

	/// <summary>
	/// ���i�̓��������鎞�Ԃ��v��
	/// </summary>
	void CountMoveStraightTime();

	/// <summary>
	/// �Ǐ]�̓���
	/// </summary>
	void MoveChase();

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
	//�e���琶�܂ꂽ��̓��������邩
	bool isParentMove = false;
	//���i���ԃ^�C�}�[
	int moveStraightTimer = 0;
};