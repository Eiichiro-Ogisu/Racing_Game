#include "FollowCamera.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

// 静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	_targetPos = Vector3::Zero;

	_targetAngle = 0.0;
}

void FollowCamera::Update()
{
	// 視点,参照点
	Vector3 eyepos, refpos;

	// 自機の上空2mを参照点とする
	refpos = _targetPos + Vector3(0, 2, 0);

	// 参照点と視点の差分のベクトル
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);

	// 自機の後ろに回り込むための回転行列を生成
	Matrix rotmat = Matrix::CreateRotationY(_targetAngle);

	// 差分ベクトルを回転
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	// カメラ座標を計算
	eyepos = refpos + cameraV;

	SetEyePos(eyepos);

	SetRefPos(refpos);

	// 基底クラスの更新
	Camera::Update();
}

void FollowCamera::SetTargetPos(DirectX::SimpleMath::Vector3 targetPos)
{
	_targetPos = targetPos;
}

void FollowCamera::SetTargetAngle(float targetAngles)
{
	_targetAngle = targetAngles;
}
