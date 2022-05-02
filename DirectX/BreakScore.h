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
	/// �X�R�A�\������
	/// </summary>
	/// <returns>�X�R�A�\��</returns>
	static BreakScore* Create(int numberTexNum, int breakTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BreakScore();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int numberTexNum, int breakTexNum);

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
	/// �X�R�A�����Z����
	/// </summary>
	void AddScore();

	//getter
	int GetScore() { return score; }

protected:
	/// <summary>
	/// �\���p�X�R�A�X�v���C�g�ύX
	/// </summary>
	void ChangeScoreSprite();

private:
	//�X�R�A�\���p�X�v���C�g
	static const int scoreDigits = 4;
	Sprite* scoreSprite[scoreDigits] = { nullptr };
	//BREAK�X�v���C�g
	Sprite* breakSprite = nullptr;
	//�X�R�A
	int score = 0;
};