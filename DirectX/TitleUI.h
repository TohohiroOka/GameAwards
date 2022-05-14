#pragma once
#include "Object3d.h"

class TitleUI
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
	/// �^�C�g���V�[����UI����
	/// </summary>
	/// <param name="RBModel">RB�{�^���I�u�W�F�N�g�Ɏg�p���郂�f��</param>
	/// <returns>�^�C�g���V�[����UI</returns>
	static TitleUI* Create(Model* RBModel);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TitleUI();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Model* RBModel);

	/// <summary>
	/// ���t���[������
	/// </summary>
	/// <param name="playerPos">�Ǐ]����v���C���[���W</param>
	void Update(XMFLOAT3 playerPos);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//setter
	void SetIsDraw(bool isDraw) { this->isDraw = isDraw; };

private:
	//RB�{�^���X�I�u�W�F�N�g
	Object3d* RBObject = nullptr;
	//�`�悷�邩
	bool isDraw = false;
};