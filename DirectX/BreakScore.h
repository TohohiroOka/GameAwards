#pragma once
#include"Sprite.h"

class BreakScore
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
	/// �X�v���C�g�s��
	/// </summary>
	enum MoveSpriteScene
	{
		None,	//�������Ȃ�
		Fall,	//�~��
		Stay,	//��~
		Rize,	//�㏸
	};

public:
	/// <summary>
	/// �ǔj��X�R�A����
	/// </summary>
	/// <returns>�ǔj��X�R�A</returns>
	static BreakScore* Create(int breakTexNum, int numberTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BreakScore();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int breakTexNum, int numberTexNum);

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
	/// �ǔj��X�R�A�𑝂₷
	/// </summary>
	void AddScore();

	//getter
	int GetBreakScore() { return breakScore; }

private:
	/// <summary>
	/// �ǔj�󐔃X�v���C�g�̐����ύX
	/// </summary>
	void ChangeBreakNumSprite();

	/// <summary>
	/// �X�v���C�g�𓮂�����Ԃɂ���
	/// </summary>
	void SetMoveSprite();

	/// <summary>
	/// �X�v���C�g���~��������
	/// </summary>
	void SpriteFall();

	/// <summary>
	/// �X�v���C�g���~������
	/// </summary>
	void SpriteStay();

	/// <summary>
	/// �X�v���C�g���㏸������
	/// </summary>
	void SpriteRize();

private:
	//BREAK�X�v���C�g
	Sprite* breakSprite = nullptr;
	//�ǔj�󐔃X�v���C�g
	static const int breakDigits = 3;
	Sprite* breakNumSprite[breakDigits] = { nullptr };

	//�ǔj�󖇐�
	int breakScore = 0;

	//�X�v���C�g�𓮂�����
	bool isMoveSprite = false;
	//�X�v���C�g�̍s��
	int moveSpriteScene = MoveSpriteScene::None;
	//�X�v���C�g�~���^�C�}�[
	int spriteFallTimer = 0;
	//�X�v���C�g��~�^�C�}�[
	int spriteStayTimer = 0;
	//�X�v���C�g�㏸�^�C�}�[
	int spriteRizeTimer = 0;
};