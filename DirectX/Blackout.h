#pragma once
#include"Sprite.h"

class Blackout
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
	/// �V�[���J�ڗp�Ó]����
	/// </summary>
	/// <returns>�V�[���J�ڗp�Ó]</returns>
	static Blackout* Create(int plainTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Blackout();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int plainTexNum);

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
	/// �Ó]��ԂɃZ�b�g
	/// </summary>
	void SetBlackout();

	/// <summary>
	/// �Ó]������ԂɃZ�b�g
	/// </summary>
	void SetBlackoutReturn();

	//getter
	bool GetIsAllBlack() { return isAllBlack; }
	bool GetIsBlackout() { return isBlackout; }
	bool GetIsBlackReturn() { return isReturn; }

private:
	/// <summary>
	/// �Ó]
	/// </summary>
	void BlackoutUpdate();

	/// <summary>
	/// �Ó]��߂�
	/// </summary>
	void BlackoutReturn();

private:
	//�Ó]�p�X�v���C�g
	Sprite* blackoutSprite = nullptr;
	//�^���Â�
	bool isAllBlack = false;
	//�Ó]����
	bool isBlackout = false;
	//�Ó]���鎞�ԃ^�C�}�[
	int blackoutTimer = 0;
	//�Ó]��߂���
	bool isReturn = false;
	//�Ó]�������鎞�ԃ^�C�}�[
	int returnTimer = 0;
};