#include "FollowCamera.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

// 静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 3.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	_targetPos = Vector3::Zero;

	_targetAngle = 0.0;
	
	isFPS = false;

	InitializeTPS();
}

void FollowCamera::Update()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	dxtk.UpdateInputState();

	Vector3 eyePos, refPos;

	// キ-ボードの状態
	Keyboard::State keyboardState = dxtk.m_keyboard->GetState();

	dxtk.m_keyTracker->Update(keyboardState);

	// Cキーを押すごとに切り替え
	if (dxtk.m_keyTracker->IsKeyPressed(Keyboard::Keys::C))
	{
		// フラグを切り替え
		isFPS = !isFPS;

		if (!isFPS)
		{
			InitializeTPS();
		}
	}


	// 視点(自機の目の位置)
	Vector3 eyepos, refpos;
	if (isFPS)
	{
		Vector3 position;

		position = _targetPos + Vector3(0, 0.2f, 0);

		// 参照点から視点への差分
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);

		// 自機の後ろに回り込むための回転
		Matrix rotmat = Matrix::CreateRotationY(_targetAngle);

		// 差分ベクトルを回転させる
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// 視点
		eyepos = position + cameraV * 0.1f;

		// 参照点
		refpos = position + cameraV;

	}

	if (isFPS != true)
	{
		// 自機の上空1mを参照点とする
		refpos = _targetPos + Vector3(0, 1, 0);

		// 参照点と視点の差分のベクトル
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);

		// 自機の後ろに回り込むための回転行列を生成
		Matrix rotmat = Matrix::CreateRotationY(_targetAngle);

		// 差分ベクトルを回転
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// カメラ座標を計算
		eyepos = refpos + cameraV;

		// ゴム紐カメラ
		eyepos = m_eyePos + (eyepos - m_eyePos)/* * 0.9f*/;

		refpos = m_refPos + (refpos - m_refPos) /** 0.8f*/;
	}
	SetEyePos(eyepos);

	SetRefPos(refpos);

	// 基底クラスの更新
	Camera::Update();
}

void FollowCamera::InitializeTPS()
{
	Vector3 eyePos, refPos;

	// 自機の情報2mの位置を捉える
	refPos = _targetPos + Vector3(0, 2, 0);

	// 参照点から視点への差分
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);

	// 自機の後ろに回り込むための回転
	Matrix rotmat = Matrix::CreateRotationY(_targetAngle);

	// 差分ベクトルを回転させる
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	// 始点座標を計算
	eyePos = refPos + cameraV;

	SetEyePos(eyePos);

	SetRefPos(refPos);
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3  & targetPos)
{
	_targetPos = targetPos;
}

void FollowCamera::SetTargetAngle(float targetAngles)
{
	_targetAngle = targetAngles;
}
//void FollowCamera::SetKeyboard(Keyboard* keyboard)
//{
//	_keyboard = keyboard;
//}
