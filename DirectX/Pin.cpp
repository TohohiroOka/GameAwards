#include "Pin.h"
#include "SafeDelete.h"

Pin* Pin::Create(Model* model, XMFLOAT3 position)
{
	//インスタンスを生成
	Pin* instance = new Pin();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Pin::~Pin()
{
	safe_delete(pinObject);
}

bool Pin::Initialize(Model* model, XMFLOAT3 position)
{
	//オブジェクト生成
	pinObject = Object3d::Create();
	if (pinObject == nullptr) {
		return false;
	}

	//情報をセット
	pinObject->SetPosition(position);
	pinObject->SetScale({ 2, 2, 1 });

	//モデルをセット
	if (model) {
		pinObject->SetModel(model);
	}

	return true;
}

void Pin::Update()
{
	//ダメージフラグがtrueなら
	if (isDamage)
	{
		//タイマーを更新
		damageTimer++;

		//タイマーが一定時間経過したら
		const int damageTime = 100;
		if (damageTimer >= damageTime)
		{
			//ダメージタイマーを初期化
			damageTimer = 0;

			//ダメージフラグをfalseにする
			isDamage = false;

			pinObject->SetColor({ 1,1,1,1 });
		}
	}

	//オブジェクト更新
	pinObject->Update();
}

void Pin::Draw()
{
	//オブジェクト描画
	pinObject->Draw();
}

void Pin::Damage(int damagePower)
{
	//引数の分ダメージを喰らう
	HP -= damagePower;
	//HPが0以下になったら削除する
	if (HP <= 0) 
	{
		SetDelete();
	}

	//色を変更する
	pinObject->SetColor({ 1,0,1,1 });

	//ダメージ状態にする
	isDamage = true;
}

void Pin::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void Pin::SetDelete()
{
	//削除する
	isDelete = true;
}
