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
	static ResultUI* Create(int plainTexNum, int resultTexNum, int breakTexNum, int numberTexNum, int maxComboTexNum, int retryTexNum, int pressATexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ResultUI();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int plainTexNum, int resultTexNum, int breakTexNum, int numberTexNum, int maxComboTexNum, int retryTexNum, int pressATexNum);

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
	/// �ő�R���{���m�肳����
	/// </summary>
	void SetMaxCombo(int maxCombo);

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
	/// �ő�R���{���X�v���C�g�𓮂�����ԂɃZ�b�g
	/// </summary>
	void SetMoveMaxComboSprite();

	/// <summary>
	/// ���g���C�X�v���C�g�𓮂�����ԂɃZ�b�g
	/// </summary>
	void SetMoveRetrySprite();

	//getter
	bool GetIsDrawAll() { return isDrawAll; }

private:
	/// <summary>
	/// �ǔj�󐔃X�v���C�g�̐����ύX
	/// </summary>
	void ChangeBreakNumSprite();

	/// <summary>
	/// �ő�R���{�X�v���C�g�̐����ύX
	/// </summary>
	void ChangeMaxComboSprite();

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
	/// �ő�R���{���X�v���C�g�𓮂���
	/// </summary>
	void MoveMaxComboSprite();

	/// <summary>
	/// ���g���C�X�v���C�g�𓮂���
	/// </summary>
	void MoveRetrySprite();

private:
	//�Ó]�p�X�v���C�g
	Sprite* blackoutSprite = nullptr;
	//���U���g�X�v���C�g
	Sprite* resultSprite = nullptr;
	//BREAK�X�v���C�g
	Sprite* breakSprite = nullptr;
	//�󂵂����X�v���C�g
	static const int breakDigits = 4;
	Sprite* breakNumSprite[breakDigits] = { nullptr };
	//MAXCOMBO�X�v���C�g
	Sprite* maxComboSprite = nullptr;
	//�ő�R���{���X�v���C�g
	static const int maxComboDigits = 4;
	Sprite* maxComboNumSprite[maxComboDigits] = { nullptr };
	//���g���C�X�v���C�g
	Sprite* retrySprite = nullptr;
	//PRESS A�X�v���C�g
	Sprite* pressASprite = nullptr;

	//�ǔj�󖇐�
	int breakWallNum = 0;
	//�ő�R���{�X�R�A
	int maxCombo = 0;

	//�Ó]����
	bool isBlackout = false;
	//�Ó]���鎞�ԃ^�C�}�[
	int blackoutTimer = 0;
	//���U���g�X�v���C�g�𓮂�����
	bool isMoveResultSprite = false;
	//���U���g�X�v���C�g�𓮂������ԃ^�C�}�[
	int moveResultSpriteTimer = 0;
	//�ǔj�󐔃X�v���C�g�𓮂�����
	bool isMoveBreakSprite = false;
	//�ǔj�󐔃X�v���C�g�𓮂������ԃ^�C�}�[
	int moveBreakSpriteTimer = 0;
	//�ő�R���{���X�v���C�g�𓮂�����
	bool isMoveMaxComboSprite = false;
	//�ő�R���{���X�v���C�g�𓮂������ԃ^�C�}�[
	int moveMaxComboSpriteTimer = 0;
	//���g���C�X�v���C�g�𓮂�����
	bool isMoveRetrySprite = false;
	//���g���C�X�v���C�g�𓮂������ԃ^�C�}�[
	int moveRetrySpriteTimer = 0;

	//�S�ĕ`�悵����
	bool isDrawAll = false;
};