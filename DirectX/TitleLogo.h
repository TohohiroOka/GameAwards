#pragma once
#include"Sprite.h"

class TitleLogo
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
	static TitleLogo* Create(int textureNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TitleLogo();

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
	/// �����J�n
	/// </summary>
	/// <param name="fallStartPosition">�����J�n���W</param>
	/// <param name="fallEndPosition">�����n�_</param>
	void FallStart(XMFLOAT2 fallStartPosition, XMFLOAT2 fallEndPosition);

	//getter
	bool GetIsFall() { return isFall; }

private:
	/// <summary>
	/// ����
	/// </summary>
	void Fall();

private:
	//�^�C�g�����S�X�v���C�g
	Sprite* titleSprite = nullptr;
	//�����J�n���W
	XMFLOAT2 fallStartPosition = {};
	//�����n�_
	XMFLOAT2 fallEndPosition = {};
	//��������
	bool isFall = false;
	//�����^�C�}�[
	int fallTimer = 0;
};