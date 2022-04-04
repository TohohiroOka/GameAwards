#pragma once
#include "DrawLine3D.h"

class Camera;

struct Grid
{
public:
	float mass;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 vel;
	DirectX::XMFLOAT3 force;

	Grid() {
		mass = 0.0f;
		pos = { 0.0f,0.0f,0.0f };
		vel = { 0.0f,0.0f,0.0f };
		force = { 0.0f,0.0f,0.0f };
	}
};

typedef std::vector<std::vector<Grid>> Grids;

class Cloth
{
private:// �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// ���ɂ傮�ɂ������
	/// </summary>
	/// <param name="startPoint"> �n�_
	/// <param name="endPoint"> �I�_
	/// <param name="color"> �F
	/// <param name="width"> ��
	/// <returns></returns>
	static Cloth* Create(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Cloth();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Cloth();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="startPoint"> �n�_
	/// <param name="endPoint"> �I�_
	/// <param name="color"> �F
	/// <param name="width"> ��
	/// <returns></returns>
	bool Init(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//setter
	void SetColor(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width);

private:
	/// <summary>
	/// �͂̍X�V
	/// </summary>
	void UpdateForce();

	/// <summary>
	/// �I�C���[�@
	/// </summary>
	void EularMethod();

	Grids grids;
	int counter;

	//��������
	static const int NumGrid = 5;// ����_
	static const int LineNum = NumGrid - 1;//���̖{��
	static const int sub = 4;
	const float gridmass = 0.1f;// ����_�̎���
	XMFLOAT3 gravity;// �d��
	XMFLOAT3 windforce;// ���̋���
	XMFLOAT3 damping;// �e����
	XMFLOAT3 k;// �悭�킩���
	XMFLOAT3 kd;// �悭�킩���
	XMFLOAT3 spring_length;// �悭�킩���
	XMFLOAT3 d_spring_length;// �悭�킩���
	XMFLOAT3 dt;// �悭�킩���
	//�����܂ł̕ϐ��̒��A�K���ɂ������Ă��炦����

	//���̑���
	float weight;
	//���̐F
	XMFLOAT4 color;

	//��
	DrawLine3D* line = nullptr;
};