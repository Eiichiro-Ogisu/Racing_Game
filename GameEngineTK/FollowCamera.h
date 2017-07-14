/// <summary>
/// 自機に追従するカメラクラス
/// </summary>
#pragma once

#include "Camera.h"
#include <Keyboard.h>
#include "DXTKResouces.h"

class FollowCamera :public Camera
{
public:
	// 自機とカメラの距離
	static const float CAMERA_DISTANCE;

	// コンストラクタ
	FollowCamera(int width, int height);

	// 毎フレーム更新
	void Update()override;

	// TPSカメラの初期化
	void InitializeTPS();

	// 追従対象の座標をセット
	void SetTargetPos(const DirectX::SimpleMath::Vector3 & targetPos);

	// 追従対象の角度をセット
	void SetTargetAngle(float targetAngles);

protected:
	// 自機の座標
	DirectX::SimpleMath::Vector3 _targetPos;

	// 追従対象の回転角
	float _targetAngle;

	bool isFPS = false;
};