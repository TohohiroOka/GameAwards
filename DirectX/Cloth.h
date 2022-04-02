#pragma once
#include "DrawLine3D.h"
#include "Camera.h"
#include <vector>

using namespace std;

struct Grid
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
	float mass;
	XMFLOAT3 pos;
	XMFLOAT3 vel;
	XMFLOAT3 force;

	Grid() {
		pos = { 0.0f,0.0f,0.0f };
		vel = { 0.0f,0.0f,0.0f };
		force = { 0.0f,0.0f,0.0f };
	}
};

typedef vector<vector<Grid>> Grids;

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
	static const int sub = 4;
	const float gridmass = 0.1f;// ����_�̎���
	XMFLOAT3 gravity = { 4.9f ,4.9f ,0.0f };// �d��
	XMFLOAT3 windforce = { 3.0f ,3.0f ,0.0f };// ���̋���
	XMFLOAT3 damping = { 0.05f ,0.05f ,0.0f };// �e����
	XMFLOAT3 k = { 5.0f ,5.0f ,0.0f };// �悭�킩���
	XMFLOAT3 kd = { 2.0f ,2.0f ,0.0f };// �悭�킩���
	XMFLOAT3 spring_length = { 2.0f ,2.0f ,0.0f };// �悭�킩���
	XMFLOAT3 d_spring_length = { spring_length.x * sqrtf(2.0f),spring_length.y * sqrtf(2.0f),0.0f };// �悭�킩���
	XMFLOAT3 dt = { 0.01f ,0.01f ,0.0f };// �悭�킩���
	//�����܂ł̕ϐ��̒��A�K���ɂ������Ă��炦����

	//���̑���
	float weight = 0.5f;

	XMFLOAT4 color = { 0.4f, 1, 0.2f, 1 };

	//��
	DrawLine3D* line[NumGrid][NumGrid] = { nullptr };
};