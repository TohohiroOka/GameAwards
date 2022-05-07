#pragma once
#include "BaseEnemy.h"

class TitleLogo : public BaseEnemy
{
public:
	/// <summary>
	/// �^�C�g�����S����
	/// </summary>
	/// <param name="position">�������W</param>
	/// <returns>�^�C�g�����S</returns>
	static TitleLogo* Create(XMFLOAT3 spawnPosition);

	/// <summary>
	/// �^�C�g�����S�̃��f�����Z�b�g
	/// </summary>
	/// <param name="titleLogoModel">���f��</param>
	static void SetModel(Model* titleLogoModel);

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

private:
	//���f��
	static Model* titleLogoModel;
};