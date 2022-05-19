#pragma once
#include"Sprite.h"

class PauseUI
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
	static PauseUI* Create(int plainTexNum, int pauseTexNum, int backGameTexNum, int backTitleTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PauseUI();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int plainTexNum, int pauseTexNum, int backGameTexNum, int backTitleTexNum);

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
	/// �m�肳����
	/// </summary>
	void SetSelect() { isSelect = true; };

	//getter
	bool GetIsBackGame() { return isBackGame; }
	bool GetIsSelect() { return isSelect; }

private:
	/// <summary>
	/// �Q�[���ɖ߂邩�^�C�g���V�[���ɖ߂邩
	/// </summary>
	void SelectBack();

private:
	//�w�i�p�X�v���C�g
	Sprite* backSprite = nullptr;
	//�|�[�Y�X�v���C�g
	Sprite* pauseSprite = nullptr;
	//�Q�[���ɖ߂�I���X�v���C�g
	Sprite* backGameSprite = nullptr;
	//�^�C�g���ɖ߂�I���X�v���C�g
	Sprite* backTitleSprite = nullptr;
	//�Q�[���ɖ߂邩
	bool isBackGame = true;
	//�I��������
	bool isSelect = false;
	//�T�E���h�̍Đ��p
	int sound[1];
	//0:�J�[�\���ړ�
};