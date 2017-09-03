#include "Car.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Car::Car()
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Car::~Car()
{
}

/// <summary>
/// ������
/// </summary>
void Car::Initialize()
{
	// ���@�p�[�c�̓ǂݍ���
	_obj.resize(CAR_PARTS_NUM);
	_obj[CAR_BODY].LoadModel(L"Resources\\body.cmo");
	_obj[CAR_TIRE].LoadModel(L"Resources\\tire.cmo");

	// �p�[�c�̐e�q�֌W���Z�b�g
	_obj[CAR_TIRE].SetParent(
		&_obj[CAR_BODY]);



	// �e����̃I�t�Z�b�g
	// �{�f�B�̍��W�����������̈ʒu��
	_obj[CAR_BODY].SetTransform(Vector3(0, 0, 3));
	_obj[CAR_BODY].SetScale(Vector3(0.01,0.01,0.01));

	//_obj[CAR_BODY].SetRotation(Vector3(0.0f, 180.0f, 0.0f));

	// ��������������
	_obj[CAR_TIRE].SetScale(Vector3(1.0f, 1.0f, 1.0f));

	_carSpeed = 0.0f;

	_isForwordMove = false;

	_isBackMove = false;

	// �S�g�̓����蔻���ݒ�
	_collisionNodeBody.Initialize();
	_collisionNodeBody.SetParent(&_obj[CAR_BODY]);
	_collisionNodeBody.SetTrans(Vector3(0, 0, 3));
	_collisionNodeBody.SetLocalRadius(1.0f);
	


	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	dxtk.m_buttons.Reset();
}

/// <summary>
/// �X�V����
/// </summary>
void Car::Update()
{
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	// �L�[�{�[�h�̏��(�����������g��)
	auto state = dxtk.m_gamePad->GetState(0);

	dxtk.m_buttons.Update(state);

	//dxtk.UpdateInputState();

	// �L-�{�[�h�̏��
	Keyboard::State keyboardState = dxtk.m_keyboard->GetState();

	//// �Q�[���p�b�h�̏��
	//GamePad::State gamepadState = dxtk.m_gamePad->GetState(0);

	// �L�[����
	if (keyboardState.W)
	{
		// �ړ���
		Vector3 moveV = Vector3(0.0f, 0.0f, -0.1f);

		float angle = _obj[CAR_BODY].GetRotation().y;

		Matrix rotmat = Matrix::CreateRotationY(angle);

		moveV = Vector3::TransformNormal(moveV, rotmat);

		// ���@�ړ�
		Vector3 pos = _obj[CAR_BODY].GetTranslation();
		_obj[CAR_BODY].SetTransform(pos += moveV);
	}

	if (keyboardState.S)
	{
		// �ړ���
		Vector3 moveV = Vector3(0.0f, 0.0f, 0.1f);

		float angle = _obj[CAR_BODY].GetRotation().y;

		Matrix rotmat = Matrix::CreateRotationY(angle);

		moveV = Vector3::TransformNormal(moveV, rotmat);

		// ���@�ړ�
		Vector3 pos = _obj[CAR_BODY].GetTranslation();
		_obj[CAR_BODY].SetTransform(pos += moveV);
	}

	if (keyboardState.A)
	{
		// ��]��
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	if (keyboardState.D)
	{
		// ��]��
		//float rot = -0.03f;
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	/// <summary>
	/// �Q�[���p�b�h����
	/// </summary>
	
	if (state.IsConnected())
	{
		// A�{�^����������Ă��Ȃ�����x��������Ă��Ȃ����)
		if (dxtk.m_buttons.a && !dxtk.m_buttons.x)
		{
			// ����
			Acceleration();
		}

		// �ړ����Ă���Ȃ猸����������
		if (_isForwordMove && !dxtk.m_buttons.a/* == !GamePad::ButtonStateTracker::PRESSED*/)
		{
			// �Ԃ̑��x�����X�ɗ��Ƃ��Ă���
			Deceleration();
		}

		/// <summary>
		/// �ړ����ɍs���鏈��
		/// </summary>
		// �O�i�ړ����Ă���Ȃ�
		if (_isForwordMove)
		{
			//if(!dxtk.m_buttons.b)
			AddSpeed();

			// �O�i�ړ����Ăяo��
			SteeringOperation(true);
		}

		/// <summary>
		/// �o�b�N����
		/// </summary>
		// b�{�^���Ńo�b�N
		if (!_isForwordMove && !dxtk.m_buttons.x && dxtk.m_buttons.b)
		{
			_isBackMove = true;
			BackMove();
		}

		// ��ވړ��t���O�������Ă���Ȃ��ވړ����Ăяo��
		if (_isBackMove)
		{
			SteeringOperation(false);
		}

		// b�{�^���������ꂽ��o�b�N�ړ��t���O�����낷
		if (!_isForwordMove && dxtk.m_buttons.b == GamePad::ButtonStateTracker::RELEASED)
		{
			_isBackMove = false;
		}

		/// <summary>
		/// �u���[�L����
		/// </summary>
		// x�{�^���Ńu���[�L
		if (_isForwordMove && dxtk.m_buttons.x/* == GamePad::ButtonStateTracker::PRESSED*/)
		{
			Breaking();
		}

		/// <summary>
		/// �Ԃ̃X�s�[�h�����ȉ��ɂȂ������~������
		/// </summary>
		if (dxtk.m_buttons.a == !GamePad::ButtonStateTracker::PRESSED &&
			_carSpeed < 0.025f && _isForwordMove)
		{
			_isForwordMove = false;
		}
	}

	// �����蔻��̍X�V
	//_collisionNodeBody.Update();

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

	_collisionNodeBody.Draw();
}

const DirectX::SimpleMath::Vector3 & Car::GetPosition()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
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
}

void Car::SetPosition(const DirectX::SimpleMath::Vector3 & trans)
{
	_obj[CAR_BODY].SetTransform(trans);
}

const DirectX::SimpleMath::Matrix & Car::GetLocalWorld()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return _obj[CAR_BODY].GetWorld();

}

const DirectX::SimpleMath::Vector3 & Car::GetRot()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return _obj[CAR_BODY].GetRotation();
}

/// <summary>
/// �ړ��J�n
/// </summary>
void Car::Acceleration()
{
	_carSpeed += MOVE_SPEED_FIRST;

	// �ړ��t���O�I����
	_isForwordMove = true;
}

/// <summary>
/// �ړ��I��
/// </summary>
void Car::Breaking()
{
	// TODO: �u���[�L�̎���
	_carSpeed *= 0.98f;
}

/// <summary>
/// ��������
/// </summary>
void Car::Deceleration()
{
	_carSpeed *= DECELERATION_VALUE;
}

/// <summary>
/// ��������
/// </summary>
void Car::AddSpeed()
{
	// �i�s�����x�N�g���̐ݒ�
	Vector3 moveVec = Vector3(0.0f, 0.0f, -1.0f);

	// ��]�擾
	float angle = _obj[CAR_BODY].GetRotation().y;

	// ���[���h���W�n�ɕϊ�
	Matrix rotmat = Matrix::CreateRotationY(angle);

	moveVec = Vector3::TransformNormal(moveVec, rotmat);

	Vector3 carVelocity = moveVec * _carSpeed;

	// ���@�ړ�
	Vector3 pos = _obj[CAR_BODY].GetTranslation();

	_obj[CAR_BODY].SetTransform(pos += carVelocity);
}

/// <summary>
/// �Ԃ̃n���h������
/// </summary>
/// <param name="moveState">true:�O�i�ړ�,false:��ވړ�</param>
void Car::SteeringOperation(bool moveState)
{
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	// �L�[�{�[�h�̏��(�����������g��)
	auto state = dxtk.m_gamePad->GetState(0);

	// �����X�e�B�b�N�̍��E����
	float dirX = state.thumbSticks.leftX;

	if (moveState)
	{
		// ��]��
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle - dirX *0.01f, 0));
	}
	else if (!moveState)
	{
		// ��]��
		float angle = _obj[CAR_BODY].GetRotation().y;
		_obj[CAR_BODY].SetRotation(Vector3(0, angle - dirX *0.005f, 0));
	}
}

/// <summary>
/// �o�b�N����
/// </summary>
void Car::BackMove()
{
	// �i�s�����x�N�g���̐ݒ�
	Vector3 moveVec = Vector3(0.0f, 0.0f, 1.0f);

	// ��]�擾
	float angle = _obj[CAR_BODY].GetRotation().y;

	// ���[���h���W�n�ɕϊ�
	Matrix rotmat = Matrix::CreateRotationY(angle);

	moveVec = Vector3::TransformNormal(moveVec, rotmat);

	Vector3 carVelocity = moveVec * BACK_SPEED;

	// ���@�ړ�
	Vector3 pos = _obj[CAR_BODY].GetTranslation();

	_obj[CAR_BODY].SetTransform(pos += carVelocity);
}
