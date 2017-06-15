/// <summary>
/// 衝突判定ライブラリ
/// </summary>

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

// 球
class Sphere
{
public:
	DirectX::SimpleMath::Vector3 _center;	// 中心座標

	float _radius;	// 半径

	// コンストラクタ
	Sphere()
	{
		_radius = 1.0f;
	}
};

// 線分
class Segment
{
public:
	DirectX::SimpleMath::Vector3 Start;	// 始点座標
	DirectX::SimpleMath::Vector3 End;	// 終点座標
};

// カプセル
class Capsule
{
public:
	Segment _segment;	// 芯線
	float _radius;		// 半径

	// コンストラクタ
	Capsule()
	{
		_segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
		_segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
		_radius = 1.0f;
	}
};

bool CheckSphere2Sphere(const Sphere & _sphereA, const Sphere & _sphereB);
