#pragma once
#include"Sprite.h"

class Rush
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
	///	���b�V���pUI����
	/// </summary>
	/// <returns>�^�C�g����ʗpUI</returns>
	static Rush* Create(int rushTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Rush();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int rushTexNum);

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
	/// ��ʓ��Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveInScreen();

	/// <summary>
	/// ��ʊO�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveOutScreen();

	//getter
	bool GetIsInScreen() { return isInScreen; }


private:
	/// <summary>
	/// ��ʓ��Ɉړ�
	/// </summary>
	void MoveInScreen();

	/// <summary>
	/// ��ʊO�Ɉړ�
	/// </summary>
	void MoveOutScreen();

	/// <summary>
	/// �F�ύX
	/// </summary>
	void ChangeColor();

private:
	//���b�V���X�v���C�g
	Sprite* rushSprite = nullptr;
	//��ʓ��Ɉړ�����
	bool isMoveInScreen = false;
	//��ʓ��Ɉړ����鎞�ԃ^�C�}�[
	int moveInScreenTimer = 0;
	//��ʊO�Ɉړ�����
	bool isMoveOutScreen = false;
	//��ʊO�Ɉړ����鎞�ԃ^�C�}�[
	int moveOutScreenTimer = 0;
	//��ʓ��ɂ��邩
	bool isInScreen = false;
	//�F��ς��邩
	bool isChangeColor = false;
	//�F��ς��鎞��
	int changeColorTimer = 0;
	//�������邩
	bool isAlphaDown = true;
};