#pragma once
#include"Sprite.h"

class Score
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
	static Score* Create(int textureNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Score();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int textureNum);

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
	/// �X�R�A�ɉ��Z����l���Z�b�g
	/// </summary>
	void SetAddScore(int addScore);

	//getter
	int GetScore() { return score; }

protected:
	/// <summary>
	/// �\���p�X�R�A��ύX
	/// </summary>
	void ChangeScore();

	/// <summary>
	/// �\���p�X�R�A�X�v���C�g�ύX
	/// </summary>
	void ChangeScoreSprite();

private:
	//�X�R�A�\���p�X�v���C�g
	static const int scoreDigits = 8;
	Sprite* scoreSprite[scoreDigits] = { nullptr };
	//�X�R�A
	int score = 0;
	//�\���p�X�R�A
	int displayScore = 0;
	//�ύX�O�̕\���p�X�R�A
	int changeDisplayScoreStart = 0;
	//�ύX��̕\���p�X�R�A
	int changeDisplayScoreEnd = 0;
	//�\���p�X�R�A�ύX�p�̃^�C�}�[
	int changeDisplayScoreTimer = 0;
	//�\���p�X�R�A�ύX����
	bool isChangeDisplayScore = false;
};