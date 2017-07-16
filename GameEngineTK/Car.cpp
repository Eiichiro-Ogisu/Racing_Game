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
	_obj[CAR_BODY].SetScale(Vector3(0.01f, 0.01f, 0.01f));

	//_obj[CAR_BODY].SetRotation(Vector3(0.0f, 180.0f, 0.0f));

	// 足をいい感じに
	_obj[CAR_TIRE].SetScale(Vector3(1.0f, 1.0f, 1.0f));

	_carSpeed = 0.0f;

	_isForwordMove = false;

	_isBackMove = false;

	// DXTKを管理するインスタンスを取得
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	dxtk.m_buttons.Reset();
}

/// <summary>
/// 更新処理
/// </summary>
void Car::Update()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	// キーボードの状態(多分こっち使う)
	auto state = dxtk.m_gamePad->GetState(0);

	dxtk.m_buttons.Update(state);

	//dxtk.UpdateInputState();

	// キ-ボードの状態
	Keyboard::State keyboardState = dxtk.m_keyboard->GetState();

	//// ゲームパッドの状態
	//GamePad::State gamepadState = dxtk.m_gamePad->GetState(0);

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
	
	if (state.IsConnected())
	{
		// Aボタンが押されていないかつxが押されていなければ)
		if (dxtk.m_buttons.a && !dxtk.m_buttons.x)
		{
			// 加速
			Acceleration();
		}

		// 移動しているなら減速をかける
		if (_isForwordMove && !dxtk.m_buttons.a/* == !GamePad::ButtonStateTracker::PRESSED*/)
		{
			// 車の速度を徐々に落としていく
			Deceleration();
		}

		/// <summary>
		/// 移動中に行われる処理
		/// </summary>
		// 前進移動しているなら
		if (_isForwordMove)
		{
			//if(!dxtk.m_buttons.b)
			AddSpeed();

			// 前進移動を呼び出す
			SteeringOperation(true);
		}

		/// <summary>
		/// バック処理
		/// </summary>
		// bボタンでバック
		if (!_isForwordMove && !dxtk.m_buttons.x && dxtk.m_buttons.b)
		{
			_isBackMove = true;
			BackMove();
		}

		// 後退移動フラグが立っているなら後退移動を呼び出す
		if (_isBackMove)
		{
			SteeringOperation(false);
		}

		// bボタンが離されたらバック移動フラグを下ろす
		if (!_isForwordMove && dxtk.m_buttons.b == GamePad::ButtonStateTracker::RELEASED)
		{
			_isBackMove = false;
		}

		/// <summary>
		/// ブレーキ処理
		/// </summary>
		// xボタンでブレーキ
		if (_isForwordMove && dxtk.m_buttons.x/* == GamePad::ButtonStateTracker::PRESSED*/)
		{
			Breaking();
		}

		/// <summary>
		/// 車のスピードが一定以下になったら停止させる
		/// </summary>
		if (dxtk.m_buttons.a == !GamePad::ButtonStateTracker::PRESSED &&
			_carSpeed < 0.025f && _isForwordMove)
		{
			_isForwordMove = false;
		}
	}
	

	// 当たり判定の更新
	//_collisionNodeBullet.Update();


	//if (keyboardState.Space)
	//{
	//	FireBullet();

	//	isFire = true;
	//}

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
}

void Car::ResetBullet()
{
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

/// <summary>
/// 移動開始
/// </summary>
void Car::Acceleration()
{
	_carSpeed += MOVE_SPEED_FIRST;

	// 移動フラグオンに
	_isForwordMove = true;
}

/// <summary>
/// 移動終了
/// </summary>
void Car::Breaking()
{
	// TODO: ブレーキの実装
	_carSpeed *= 0.98f;
}

/// <summary>
/// 減速処理
/// </summary>
void Car::Deceleration()
{
	_carSpeed *= DECELERATION_VALUE;
}

/// <summary>
/// 加速処理
/// </summary>
void Car::AddSpeed()
{
	// 進行方向ベクトルの設定
	Vector3 moveVec = Vector3(0.0f, 0.0f, -1.0f);

	// 回転取得
	float angle = _obj[CAR_BODY].GetRotation().y;

	// ワールド座標系に変換
	Matrix rotmat = Matrix::CreateRotationY(angle);

	moveVec = Vector3::TransformNormal(moveVec, rotmat);

	Vector3 carVelocity = moveVec * _carSpeed;

	// 自機移動
	Vector3 pos = _obj[CAR_BODY].GetTranslation();

	_obj[CAR_BODY].SetTransform(pos += carVelocity);
}

/// <summary>
/// 車のハンドル制御
/// </summary>
/// <param name="moveState">true:前進移動,false:後退移動</param>
void Car::SteeringOperation(bool moveState)
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	// キーボードの状態(多分こっち使う)
	auto state = dxtk.m_gamePad->GetState(0);

	// 左側スティックの左右入力
	float dirX = state.thumbSticks.leftX;

	if (moveState)
	{
		// 回転量
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle - dirX *0.01f, 0));
	}
	else if (!moveState)
	{
		// 回転量
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle - dirX *0.005f, 0));
	}
}

/// <summary>
/// バック処理
/// </summary>
void Car::BackMove()
{
	// 進行方向ベクトルの設定
	Vector3 moveVec = Vector3(0.0f, 0.0f, 1.0f);

	// 回転取得
	float angle = _obj[CAR_BODY].GetRotation().y;

	// ワールド座標系に変換
	Matrix rotmat = Matrix::CreateRotationY(angle);

	moveVec = Vector3::TransformNormal(moveVec, rotmat);

	Vector3 carVelocity = moveVec * BACK_SPEED;

	// 自機移動
	Vector3 pos = _obj[CAR_BODY].GetTranslation();

	_obj[CAR_BODY].SetTransform(pos += carVelocity);
}
