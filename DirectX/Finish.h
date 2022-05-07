#pragma once
#include"Sprite.h"

class Finish
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// Finish�\������
	/// </summary>
	/// <returns>Finish�\��</returns>
	static Finish* Create(int finishTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Finish();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int finishTexNum);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// Finish�X�v���C�g�𓮂�����ԂɃZ�b�g
	/// </summary>
	void SetFinishSpriteMove();

	//getter
	bool GetIsFinishSpriteMoveEnd() { return isFinishSpriteMoveEnd; }

private:

	/// <summary>
	/// Finish�X�v���C�g�𓮂���
	/// </summary>
	void FinishSpriteMove();

private:
	//Finish�p�X�v���C�g
	Sprite* finishSprite = nullptr;
	//Finish�X�v���C�g�𓮂�����
	bool isFinishSpriteMove = false;
	//Finish���I�������
	bool isFinishSpriteMoveEnd = false;
	//Finish�X�v���C�g�𓮂������ԃ^�C�}�[
	int finishSpriteMoveTimer = 0;
};