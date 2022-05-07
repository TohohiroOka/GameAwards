#pragma once
#include"Sprite.h"

class ReadyGo
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
	/// ReadyGo�\������
	/// </summary>
	/// <returns>ReadyGo�\��</returns>
	static ReadyGo* Create(int readyTexNum, int goTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ReadyGo();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int readyTexNum, int goTexNum);

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
	/// Ready�X�v���C�g�𓮂�����ԂɃZ�b�g
	/// </summary>
	void SetReadySpriteMove();

	/// <summary>
	/// Go�X�v���C�g�𓮂�����ԂɃZ�b�g
	/// </summary>
	void SetGoSpriteMove();

	//getter
	bool GetIsReadyGoEnd() { return isReadyGoEnd; }

private:

	/// <summary>
	/// Ready�X�v���C�g�𓮂���
	/// </summary>
	void ReadySpriteMove();

	/// <summary>
	/// Go�X�v���C�g�𓮂���
	/// </summary>
	void GoSpriteMove();

private:
	//Ready�p�X�v���C�g
	Sprite* readySprite =  nullptr;
	//Go�p�X�v���C�g
	Sprite* goSprite = nullptr;
	//Ready�X�v���C�g�𓮂�����
	bool isReadySpriteMove = false;
	//Go�X�v���C�g�𓮂�����
	bool isGoSpriteMove = false;
	//Ready�X�v���C�g�𓮂������ԃ^�C�}�[
	int readySpriteMoveTimer = 0;
	//Go�X�v���C�g�𓮂������ԃ^�C�}�[
	int goSpriteMoveTimer = 0;
	//ReadyGo���I�������
	bool isReadyGoEnd = false;
};