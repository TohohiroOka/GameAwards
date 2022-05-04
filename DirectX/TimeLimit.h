#pragma once
#include"Sprite.h"

class TimeLimit
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
	/// �������Ԑ���
	/// </summary>
	/// <returns>��������</returns>
	static TimeLimit* Create(int textureNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TimeLimit();

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
	/// �������ԉ�
	/// </summary>
	/// <param name="second"></param>
	void Recovery(int second);

	//getter
	int GetTime() { return timer; }

private:
	/// <summary>
	/// �J�E���g�_�E��
	/// </summary>
	void CountDown();

	/// <summary>
	/// �\���p�������ԃX�v���C�g�ύX
	/// </summary>
	void ChangeTimeSprite();

private:
	//�������ԃX�v���C�g
	static const int timeDigits = 2;
	Sprite* timeSprite[timeDigits] = { nullptr };
	//�������ԍő�
	const int timeLimitMax = 6000;
	//���Ԍv���^�C�}�[
	int timer = timeLimitMax;
};