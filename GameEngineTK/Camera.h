#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
public:
	// メンバ関数
	Camera(int width, int height);

	virtual ~Camera();

	// 更新
	virtual void Update();

	// ビュー行列の取得
	const DirectX::SimpleMath::Matrix& GetViewMatrix();

	// 射影行列を取得
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix();

	DirectX::SimpleMath::Vector3 GetUpVec();

	// 視点をセット
	void SetEyePos(DirectX::SimpleMath::Vector3 eyePos);

	void SetRefPos(DirectX::SimpleMath::Vector3 ref);

	void SetUpVec(const DirectX::SimpleMath::Vector3& upVec);

	void SetFovY(float y);

	void SetAspect(float aspect);

	void SetNearclip(float nearclip);

	void SetFarclip(float farclip);
protected:
	// 材料はここに

	// ビュー行列
	DirectX::SimpleMath::Matrix m_cameraView;
	// 視点
	DirectX::SimpleMath::Vector3 m_eyePos;

	// 注視点
	DirectX::SimpleMath::Vector3 m_refPos;

	// どちらが画面上方向か
	DirectX::SimpleMath::Vector3 m_upVec;

	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_cameraProj;

	// 垂直方向視野角
	float m_fovY;

	// アスペクト比
	float m_aspect;

	// 手前の表示限界
	float m_nearclip;

	// 奥の表示限界
	float m_farclip;
};
