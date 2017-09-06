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

	// �ړ�����
	const float MOVE_SPEED_FIRST = 0.001f;

	// �ړ�����<m/frame>
	const float MOVE_SPEED_MAX = 0.03f;
	// �����l
	const float DECELERATION_VALUE = 0.995f;

	// �o�b�N�̑��x
	const float BACK_SPEED = 0.02f;

	// ���@�p�[�c
	enum CAR_PARTS
	{
		CAR_BODY,
		CAR_TIRE,

		CAR_PARTS_NUM
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Car();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Car();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	// �S�g�̓����蔻��擾
	const SphereNode& GetCollisionNodeBody() { return _collisionNodeBody; }



	/// <summary>
	/// ���W���擾
	/// </summary>
	const DirectX::SimpleMath::Vector3& GetPosition();

	void Calc();

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	void SetPosition(const DirectX::SimpleMath::Vector3& trans);

	/// <summary>
	/// ���[���h�s����擾
	/// </summary>
	const DirectX::SimpleMath::Matrix& GetLocalWorld();


	/// <summary>
	/// �p�x�̎擾
	/// </summary>
	const DirectX::SimpleMath::Vector3& GetRot();

	void Acceleration();

	void Breaking();

	void Deceleration();

	void AddSpeed();

	void SteeringOperation(bool moveState);

	void BackMove();

protected:
	// �p�[�c
	std::vector<Obj3d> _obj;

	// ���@�̍��W
	DirectX::SimpleMath::Vector3 _position;

	DirectX::SimpleMath::Vector3 _angle;

	// �v���C���[�̑��x�x�N�g��
	float _carSpeed;

	// _carSpeed�ɉ�����񂪓����Ă��邩�ǂ���
	bool _isForwordMove;

	// �o�b�N���Ă��邩�ǂ���
	bool _isBackMove;

	int cnt = 0;

	bool isFire = false;

	// �S�g�p�̓����蔻��
	SphereNode _collisionNodeBody;


};