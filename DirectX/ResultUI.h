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

private:
	enum DrawScene 
	{
		None,			//�����`�悵�Ȃ�
		ResultDraw,		//���U���g�`��
		FinalScoreDraw,	//�ŏI�X�R�A�`��
		Stay,			//�ꎞ��~
		PressButtonDraw,//pressButton�`��
	};

public:
	/// <summary>
	/// �^�C�g�����S����
	/// </summary>
	/// <returns>�^�C�g�����S</returns>
	static ResultUI* Create(int resultTexNum, int scoreTexNum, int numberTexNum, int pressButtonTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ResultUI();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int resultTexNum, int scoreTexNum, int numberTexNum, int pressButtonTexNum);

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
	/// �ŏI�X�R�A���m�肳����
	/// </summary>
	void SetFinalScore(int finalScore);

	//getter
	bool GetIsDrawAll() { return isDrawAll; }

private:
	/// <summary>
	/// ���Ԍv��
	/// </summary>
	void TimeCount();

	/// <summary>
	/// �\���p�X�R�A�X�v���C�g�̃J�E���g�𑝂₷���o
	/// </summary>
	void IncreaseDisplayScore();

	/// <summary>
	///  �\���p�X�R�A�X�v���C�g�ύX
	/// </summary>
	void IncreaseScoreSprite();

private:
	//���U���g�X�v���C�g
	Sprite* resultSprite = nullptr;
	//SCORE:�X�v���C�g
	Sprite* SCORESprite = nullptr;
	//�ŏI�X�R�A�X�v���C�g
	static const int scoreDigits = 8;
	Sprite* finalScoreSprite[scoreDigits] = { nullptr };
	//pressButton�X�v���C�g
	Sprite* pressButtonSprite = nullptr;
	//�ŏI�X�R�A
	int finalScore = 0;
	//�\���p�X�R�A
	int displayScore = 0;
	//���Ԍv���^�C�}�[
	int timer = 0;
	//���񂾂�`�悵�Ă���
	int drawScene = DrawScene::None;
	//�S�ĕ`�悵����
	bool isDrawAll = false;
};