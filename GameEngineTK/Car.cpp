#include "Car.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
Car::Car()
{
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
Car::~Car()
{
}

/// <summary>
/// 初期化
/// </summary>
void Car::Initialize()
{
	// 自機パーツの読み込み
	_obj.resize(CAR_PARTS_NUM);
	_obj[CAR_BODY].LoadModel(L"Resources\\body.cmo");
	_obj[CAR_TIRE].LoadModel(L"Resources\\tire.cmo");

	// パーツの親子関係をセット
	_obj[CAR_TIRE].SetParent(
		&_obj[CAR_BODY]);



	// 親からのオフセット
	// ボディの座標をいい感じの位置に
	_obj[CAR_BODY].SetTransform(Vector3(0, 0, 0));
	_obj[CAR_BODY].SetScale(Vector3(0.025f, 0.025f, 0.025f));

	//_obj[CAR_BODY].SetRotation(Vector3(0.0f, 180.0f, 0.0f));

	// 足をいい感じに
	_obj[CAR_TIRE].SetScale(Vector3(1.0f, 1.0f, 1.0f));

	// TODO
		//{// 弾丸用の当たり判定を設定
		//_collisionNodeBullet.Initialize();
		//		// 親パーツを指定
		//_collisionNodeBullet.SetParent(&_obj[PLAYER_PARTS_HAND]);
		//_collisionNodeBullet.SetTrans(Vector3(0, 0, 0));
		//_collisionNodeBullet.SetLocalRadius(0.5f);
		//}

	_carVelocity = Vector3::Zero;
}

/// <summary>
/// 更新処理
/// </summary>
void Car::Update()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	dxtk.UpdateInputState();

	// キ-ボードの状態
	Keyboard::State keyboardState = dxtk.m_keyboard->GetState();

	// ゲームパッドの状態
	GamePad::State gamepadState = dxtk.m_gamePad->GetState(0);

	// キー操作
	if (keyboardState.W)
	{
		// 移動量
		Vector3 moveV = Vector3(0.0f, 0.0f, -0.1f);

		float angle = _obj[CAR_BODY].GetRotation().y;

		Matrix rotmat = Matrix::CreateRotationY(angle);

		moveV = Vector3::TransformNormal(moveV, rotmat);

		// 自機移動
		Vector3 pos = _obj[CAR_BODY].GetTranslation();
		_obj[CAR_BODY].SetTransform(pos += moveV);
	}

	if (keyboardState.S)
	{
		// 移動量
		Vector3 moveV = Vector3(0.0f, 0.0f, 0.1f);

		float angle = _obj[CAR_BODY].GetRotation().y;

		Matrix rotmat = Matrix::CreateRotationY(angle);

		moveV = Vector3::TransformNormal(moveV, rotmat);

		// 自機移動
		Vector3 pos = _obj[CAR_BODY].GetTranslation();
		_obj[CAR_BODY].SetTransform(pos += moveV);
	}

	if (keyboardState.A)
	{
		// 回転量
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	if (keyboardState.D)
	{
		// 回転量
		//float rot = -0.03f;
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	/// <summary>
	/// ゲームパッド操作
	/// </summary>
	
	if (gamepadState.IsConnected())
	{
		// Aキー
		if (gamepadState.IsAPressed())
		{
			// 移動量
			Vector3 moveV = Vector3(0.0f, 0.0f, -0.1f);

			float angle = _obj[CAR_BODY].GetRotation().y;

			Matrix rotmat = Matrix::CreateRotationY(angle);

			moveV = Vector3::TransformNormal(moveV, rotmat);

			// 自機移動
			Vector3 pos = _obj[CAR_BODY].GetTranslation();
			_obj[CAR_BODY].SetTransform(pos += moveV);
		}

		// 左スティックの右入力
		float dirX = gamepadState.thumbSticks.leftX;

		// 回転量
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle - dirX *0.025f, 0));

	}
	
	// 当たり判定の更新
	_collisionNodeBullet.Update();


	if (keyboardState.Space)
	{
		FireBullet();

		isFire = true;
	}

	Calc();
}

void Car::Draw()
{
	for (std::vector<Obj3d>::iterator it = _obj.begin(); 
		it != _obj.end();
		it++)
	{
		it->Draw();
	}
	_collisionNodeBullet.Draw();

	_collisionNodeBody.Draw();
}

void Car::FireBullet()
{
	//// 発射するパーツのワールド行列を取得
	//Matrix worldm = _obj[PLAYER_PARTS_HAND].GetWorld();

	//Matrix worldm2 = _obj[PLAYER_PARTS_HAND2].GetWorld();


	// ワールド行列から各要素を抽出
	Vector3 scale;			// ワールドスケーリング
	Quaternion rotation;	// ワールド回転
	Vector3 translation;	// ワールド座標

	//worldm.Decompose(scale,rotation, translation);

	//// 発射やパーツを親から分離して独立
	//_obj[PLAYER_PARTS_HAND].SetParent(nullptr);
	//_obj[PLAYER_PARTS_HAND].SetScale(scale);
	//_obj[PLAYER_PARTS_HAND].SetRotationQ(rotation);
	//_obj[PLAYER_PARTS_HAND].SetTransform(translation);

	//worldm2.Decompose(scale, rotation, translation);

	//_obj[PLAYER_PARTS_HAND2].SetParent(nullptr);
	//_obj[PLAYER_PARTS_HAND2].SetScale(scale);
	//_obj[PLAYER_PARTS_HAND2].SetRotationQ(rotation);
	//_obj[PLAYER_PARTS_HAND2].SetTransform(translation);


	//// 弾丸の速度を設定
	//_bulletVel = Vector3(0, 0, -0.5f);
	//_bulletVel = Vector3::Transform(_bulletVel, rotation);
}

void Car::ResetBullet()
{
		//_obj[PLAYER_PARTS_HAND].SetParent(&_obj[PLAYER_PARTS_BODY]);
		//_obj[PLAYER_PARTS_HAND2].SetParent(&_obj[PLAYER_PARTS_BODY]);

		//_obj[PLAYER_PARTS_HAND].SetTransform(Vector3(-0.5, 0.6, -0.2));
		//_obj[PLAYER_PARTS_HAND].SetRotation(Vector3(0, 0, 0));
		//_obj[PLAYER_PARTS_HAND].SetScale(Vector3(0.25, 0.25, 0.75));


		//_obj[PLAYER_PARTS_HAND2].SetTransform(Vector3(0.5, 0.6, -0.2));
		//_obj[PLAYER_PARTS_HAND2].SetRotation(Vector3(0,0,0));
		//_obj[PLAYER_PARTS_HAND2].SetScale(Vector3(0.25, 0.25, 0.75));

}

const DirectX::SimpleMath::Vector3 & Car::GetPosition()
{
	// TODO: return ステートメントをここに挿入します
	return _obj[CAR_BODY].GetTranslation();
}

void Car::Calc()
{
	for (std::vector<Obj3d>::iterator it = _obj.begin();
		it != _obj.end();
		it++)
	{
		it->Update();
	}

	_collisionNodeBody.Update();

	_collisionNodeBullet.Update();
}

void Car::SetPosition(const DirectX::SimpleMath::Vector3 & trans)
{
	_obj[CAR_BODY].SetTransform(trans);
}

const DirectX::SimpleMath::Matrix & Car::GetLocalWorld()
{
	// TODO: return ステートメントをここに挿入します
	return _obj[CAR_BODY].GetWorld();

}

const DirectX::SimpleMath::Vector3 & Car::GetRot()
{
	// TODO: return ステートメントをここに挿入します
	return _obj[CAR_BODY].GetRotation();
}

