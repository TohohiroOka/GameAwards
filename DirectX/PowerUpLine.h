#pragma once
#include "DrawLine3D.h"
#include "DeadEnemyPoint.h"
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
	static PowerUpLine *Create(DeadEnemyPoint* startPoint, DeadEnemyPoint* endPoint);

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
	bool Initialize(DeadEnemyPoint* startPoint, DeadEnemyPoint* endPoint);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Camera *camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���ɐ�������Ă��邩���`�F�b�N
	/// </summary>
	bool IsAlreadyCreateLine(DeadEnemyPoint* startPoint, DeadEnemyPoint* endPoint);

	/// <summary>
	/// �n�_���I�_�Ɉ����̎��S�~���g�p���Ă��邩�m�F
	/// </summary>
	/// <param name="point">���S�~</param>
	void CheckUsePoints(DeadEnemyPoint* point);


	void SetColor(XMFLOAT4 color);
	//getter
	XMFLOAT3 GetStartPoint() { return startPoint->GetPosition(); }
	XMFLOAT3 GetEndPoint() { return endPoint->GetPosition(); }
	bool GetIsDelete() { return isDelete; }

private:
	//��
	DrawLine3D* line = nullptr;
	//���̎n�_
	DeadEnemyPoint* startPoint = nullptr;
	//���̏I�_
	DeadEnemyPoint* endPoint = nullptr;
	//���̑���
	float weight = 0.5f;
	//�폜���邩
	bool isDelete = false;
};