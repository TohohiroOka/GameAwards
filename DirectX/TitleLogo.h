#pragma once
#include "BaseEnemy.h"

class TitleLogo : public BaseEnemy
{
public:
	/// <summary>
	/// �^�C�g�����S����
	/// </summary>
	/// <returns>�^�C�g�����S</returns>
	static TitleLogo* Create();

	/// <summary>
	/// �^�C�g�����S�̃��f�����Z�b�g
	/// </summary>
	/// <param name="titleLogoModel">���f��</param>
	static void SetModel(Model* titleLogoModel);

	/// <summary>
	/// �X�|�[���I�������u�Ԃ�
	/// </summary>
	/// <returns></returns>
	static bool GetTriggerSpawnEnd();

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
	/// <param name="powerLevel">�m�b�N�o�b�N�̋���</param>
	/// <param name="powerMagnification">�m�b�N�o�b�N�̋����̔{��</param>
	void SetKnockBack(float angle, int powerLevel, float powerMagnification) override;

	/// <summary>
	///	�ǂɂԂ�����������
	/// </summary>
	/// <returns>�ǂɂԂ�������</returns>
	bool IsCollisionWall() override;

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
	/// X������
	/// </summary>
	void ReflectionX();

	/// <summary>
	/// Y������
	/// </summary>
	void ReflectionY();


private:
	//���f��
	static Model* titleLogoModel;
	//�X�|�[���I��������
	static bool isSpawnEnd;
	//�X�|�[�����W
	static const XMFLOAT3 spawnPos;
	//��~���W
	static const XMFLOAT3 stayPos;

private:
	//�X�|�[������
	bool isSpawn = true;
	//�X�|�[�����鎞�ԃ^�C�}�[
	int spawnTimer = 0;
	//��x�ǂɓ���������
	bool isCollisionWallFirst = false;
};