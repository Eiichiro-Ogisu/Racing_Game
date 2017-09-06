#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Singleton.h"
#include "DXTKResouces.h"
#include "obj3d.h"
#include "CollisionNode.h"

class Car
{
public:

	// 移動初速
	const float MOVE_SPEED_FIRST = 0.001f;

	// 移動初速<m/frame>
	const float MOVE_SPEED_MAX = 0.03f;
	// 減速値
	const float DECELERATION_VALUE = 0.995f;

	// バックの速度
	const float BACK_SPEED = 0.02f;

	// 自機パーツ
	enum CAR_PARTS
	{
		CAR_BODY,
		CAR_TIRE,

		CAR_PARTS_NUM
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Car();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Car();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 全身の当たり判定取得
	const SphereNode& GetCollisionNodeBody() { return _collisionNodeBody; }



	/// <summary>
	/// 座標を取得
	/// </summary>
	const DirectX::SimpleMath::Vector3& GetPosition();

	void Calc();

	/// <summary>
	/// 座標を設定
	/// </summary>
	void SetPosition(const DirectX::SimpleMath::Vector3& trans);

	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	const DirectX::SimpleMath::Matrix& GetLocalWorld();


	/// <summary>
	/// 角度の取得
	/// </summary>
	const DirectX::SimpleMath::Vector3& GetRot();

	void Acceleration();

	void Breaking();

	void Deceleration();

	void AddSpeed();

	void SteeringOperation(bool moveState);

	void BackMove();

protected:
	// パーツ
	std::vector<Obj3d> _obj;

	// 自機の座標
	DirectX::SimpleMath::Vector3 _position;

	DirectX::SimpleMath::Vector3 _angle;

	// プレイヤーの速度ベクトル
	float _carSpeed;

	// _carSpeedに加速情報が入っているかどうか
	bool _isForwordMove;

	// バックしているかどうか
	bool _isBackMove;

	int cnt = 0;

	bool isFire = false;

	// 全身用の当たり判定
	SphereNode _collisionNodeBody;


};