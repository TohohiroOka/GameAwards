#pragma once
#include"Sprite.h"

class ResultUI
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
	/// �^�C�g�����S����
	/// </summary>
	/// <returns>�^�C�g�����S</returns>
	static ResultUI* Create(int plainTexNum, int resultTexNum, int breakTexNum, int numberTexNum, int retryTexNum, int backTitleTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ResultUI();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int plainTexNum, int resultTexNum, int breakTexNum, int numberTexNum, int retryTexNum, int backTitleTexNum);

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
	/// �ǔj�󐔂��m�肳����
	/// </summary>
	void SetBreakWallNum(int breakWallNum);

	/// <summary>
	/// �Ó]��ԂɃZ�b�g
	/// </summary>
	void SetBlackOut();

	/// <summary>
	/// ���U���g�X�v���C�g�𓮂�����ԂɃZ�b�g
	/// </summary>
	void SetMoveResultSprite();

	/// <summary>
	/// �ǔj�󐔃X�v���C�g�𓮂�����ԂɃZ�b�g
	/// </summary>
	void SetMoveBreakSprite();

	/// <summary>
	/// ���g���C�X�v���C�g�𓮂�����ԂɃZ�b�g
	/// </summary>
	void SetMoveRetrySprite();

	/// <summary>
	/// ���̃V�[���I�����m�肳����
	/// </summary>
	void SetSelect() { isSelect = true; };

	//getter
	bool GetIsDrawAll() { return isDrawAll; }
	bool GetIsRetry() { return isRetry; }
	bool GetIsSelect() { return isSelect; }

private:
	/// <summary>
	/// �ǔj�󐔃X�v���C�g�̐����ύX
	/// </summary>
	void ChangeBreakNumSprite();

	/// <summary>
	/// �Ó]
	/// </summary>
	void BlackOut();

	/// <summary>
	/// ���U���g�X�v���C�g�𓮂���
	/// </summary>
	void MoveResultSprite();

	/// <summary>
	/// �ǔj�󐔃X�v���C�g�𓮂���
	/// </summary>
	void MoveBreakSprite();

	/// <summary>
	/// ���g���C�X�v���C�g�𓮂���
	/// </summary>
	void MoveRetrySprite();

	/// <summary>
	/// ���g���C���邩�^�C�g���V�[���ɖ߂邩
	/// </summary>
	void SelectRetry();

private:
	//�Ó]�w�i�p�X�v���C�g
	Sprite* blackoutSprite = nullptr;
	//���U���g�X�v���C�g
	Sprite* resultSprite = nullptr;
	//BREAK�X�v���C�g
	Sprite* breakSprite = nullptr;
	//�󂵂����X�v���C�g
	static const int breakDigits = 4;
	Sprite* breakNumSprite[breakDigits] = { nullptr };
	//���g���C�X�v���C�g
	Sprite* retrySprite = nullptr;
	//�^�C�g���ɖ߂�X�v���C�g
	Sprite* backTitleSprite = nullptr;

	//�ǔj�󖇐�
	int breakWallNum = 0;

	//�w�i�Ó]����
	bool isBlackout = false;
	//�w�i�Ó]���鎞�ԃ^�C�}�[
	int blackoutTimer = 0;
	//���U���g�X�v���C�g�𓮂�����
	bool isMoveResultSprite = false;
	//���U���g�X�v���C�g�𓮂������ԃ^�C�}�[
	int moveResultSpriteTimer = 0;
	//�ǔj�󐔃X�v���C�g�𓮂�����
	bool isMoveBreakSprite = false;
	//�ǔj�󐔃X�v���C�g�𓮂������ԃ^�C�}�[
	int moveBreakSpriteTimer = 0;
	//���g���C�X�v���C�g�𓮂�����
	bool isMoveRetrySprite = false;
	//���g���C�X�v���C�g�𓮂������ԃ^�C�}�[
	int moveRetrySpriteTimer = 0;

	//�S�ĕ`�悵����
	bool isDrawAll = false;

	//���g���C��Ԃ�
	bool isRetry = true;
	//�m�肵����
	bool isSelect = false;
	//�T�E���h�̍Đ��p
	int sound[1];
	//0:�J�[�\���ړ�
};