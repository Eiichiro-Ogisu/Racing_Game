/// <summary>
/// �Փ˔��胉�C�u����
/// </summary>

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

// ��
class Sphere
{
public:
	DirectX::SimpleMath::Vector3 _center;	// ���S���W

	float _radius;	// ���a

	// �R���X�g���N�^
	Sphere()
	{
		_radius = 1.0f;
	}
};

// ����
class Segment
{
public:
	DirectX::SimpleMath::Vector3 Start;	// �n�_���W
	DirectX::SimpleMath::Vector3 End;	// �I�_���W
};

// �O�p�`
class Triangle
{
public:
	DirectX::SimpleMath::Vector3 P0;
	DirectX::SimpleMath::Vector3 P1;
	DirectX::SimpleMath::Vector3 P2;
	DirectX::SimpleMath::Vector3 Normal;	// �@���x�N�g��
};

// �J�v�Z��
class Capsule
{
public:
	Segment _segment;	// �c��
	float _radius;		// ���a

	// �R���X�g���N�^
	Capsule()
	{
		_segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
		_segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
		_radius = 1.0f;
	}
};

class OBB
{
	//// ���S���W
	//DirectX::SimpleMath::Vector3 m_center;

	//// ���a(x,y,z)
	//float m_radius[3];

	//OBB()
	//{
	//	for (int i = 0; i < 3; i++)
	//	{
	//		m_radius[i] = 1.0f;
	//	}
	//}
};

bool CheckSphere2Sphere(const Sphere & _sphereA, const Sphere & _sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3 & _p0, const DirectX::SimpleMath::Vector3 & _p1, const DirectX::SimpleMath::Vector3 & _p2, Triangle * _triangle);

bool CheckSphere2Triangle(const Sphere & _sphere, const Triangle & _tri, DirectX::SimpleMath::Vector3 * _inter);

bool CheckSegment2Triangle(const Segment & _segment, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);
