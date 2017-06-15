/// <summary>
/// 衝突判定ライブラリ
/// </summary>
#include "Collision.h"

using namespace DirectX::SimpleMath;

/// <summary>
/// 球と球の当たり判定
/// </summary>
/// <param name="_sphereA">球A</param>
/// <param name="_sphereB">球B</param>
/// <returns>true:ヒット	false:ヒットせず</returns>
bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB)
{
	// 球AとBの中心座標の差を計算する
	Vector3 sub = _sphereB._center - _sphereA._center;

	float distanceSquare;

	// 三平方の定理
	distanceSquare = sqrt(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);

	float radiusSquare;

	radiusSquare = _sphereA._radius + _sphereB._radius;

	radiusSquare = radiusSquare * radiusSquare;

	// 距離が半径の和より大きければ当たっていない
	if (distanceSquare > radiusSquare)
	{
		return false;
	}
	return true;
}