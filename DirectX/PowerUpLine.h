#pragma once
//#include "DrawLine3D.h"
#include "Cloth.h"
#include "ConnectCircle.h"
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
	/// <param name="startPoint">�n�_�Ɏg�p����R�l�N�g�T�[�N��</param>
	/// <param name="endPoint">�I�_�Ɏg�p����R�l�N�g�T�[�N��</param>
	/// <returns>�p���[�A�b�v��</returns>
	static PowerUpLine *Create(ConnectCircle* startPoint, ConnectCircle* endPoint);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PowerUpLine();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="startPoint">�n�_�Ɏg�p����R�l�N�g�T�[�N��</param>
	/// <param name="endPoint">�I�_�Ɏg�p����R�l�N�g�T�[�N��</param>
	/// <returns>����</returns>
	bool Initialize(ConnectCircle* startPoint, ConnectCircle* endPoint);

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
	bool IsAlreadyCreateLine(ConnectCircle* startPoint, ConnectCircle* endPoint);

	/// <summary>
	/// �n�_���I�_�Ɉ����̃R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
	/// </summary>
	/// <param name="point">�R�l�N�g�T�[�N��</param>
	/// <returns>�n�_���I�_�Ɉ����̃R�l�N�g�T�[�N�����g�p���Ă��邩</returns>
	bool CheckUsePoints(ConnectCircle* point);

	/// <summary>
	/// �폜
	/// </summary>
	void SetDelete();


	void SetColor(XMFLOAT4 color);
	//getter
	XMFLOAT3 GetStartPoint() { return startPoint->GetPosition(); }
	XMFLOAT3 GetEndPoint() { return endPoint->GetPosition(); }
	bool GetIsDelete() { return isDelete; }

private:
	/*//��
	DrawLine3D* line = nullptr;
	//���̎n�_�Ŏg�p����R�l�N�g�T�[�N��
	ConnectCircle* startPoint = nullptr;
	//���̏I�_�Ŏg�p����R�l�N�g�T�[�N��
	ConnectCircle* endPoint = nullptr;
	//���̑���
	float weight = 0.5f;
	//�폜���邩
	bool isDelete = false;*/

	//���ɂ傮�ɂ��
	Cloth* cloth = nullptr;
	//���̎n�_�Ŏg�p����R�l�N�g�T�[�N��
	ConnectCircle* startPoint = nullptr;
	//���̏I�_�Ŏg�p����R�l�N�g�T�[�N��
	ConnectCircle* endPoint = nullptr;
	//���̑���
	float weight = 0.5f;
	//�폜���邩
	bool isDelete = false;
};