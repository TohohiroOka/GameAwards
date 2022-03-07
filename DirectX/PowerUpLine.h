#pragma once
#include "DrawLine3D.h"
#include "Camera.h"

class PowerUpLine
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
	/// �p���[�A�b�v������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>�p���[�A�b�v��</returns>
	static PowerUpLine *Create(XMFLOAT3 startPoint, XMFLOAT3 endPoint);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PowerUpLine();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>����</returns>
	bool Initialize(XMFLOAT3 startPoint, XMFLOAT3 endPoint);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Camera *camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void SetColor(XMFLOAT4 color);
	//getter
	XMFLOAT3 GetStartPoint() { return startPoint; }
	XMFLOAT3 GetEndPoint() { return endPoint; }

private:
	//��
	//DrawFunction *line = nullptr;
	//DrawLine* line = nullptr;
	DrawLine3D* line = nullptr;
	//���̎n�_
	XMFLOAT3 startPoint = { 0, 0, 0 };
	//���̏I�_
	XMFLOAT3 endPoint = { 0, 0, 0 };
};