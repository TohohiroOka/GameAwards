#pragma once
#include "Object3d.h"

class BuckGround
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private://�ÓI�����o�ϐ�

	static const int widthX = 13;
	static const int widthY = 8;
	static const int arreyNum = widthX * widthY;

private://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(Model* model);

public://�����o�֐�

	BuckGround() {};
	~BuckGround();

	/// <summary>
	/// �������
	/// </summary>
	void AllDelete();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	BuckGround* Create(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private://�����o�ϐ�

	static std::vector<Object3d*> buckGround;
};
