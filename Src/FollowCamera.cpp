#include "FollowCamera.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

// �ÓI�����o�ϐ��̏�����
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
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	dxtk.UpdateInputState();

	Vector3 eyePos, refPos;

	// �L-�{�[�h�̏��
	Keyboard::State keyboardState = dxtk.m_keyboard->GetState();

	dxtk.m_keyTracker->Update(keyboardState);

	// C�L�[���������Ƃɐ؂�ւ�
	if (dxtk.m_keyTracker->IsKeyPressed(Keyboard::Keys::C))
	{
		// �t���O��؂�ւ�
		isFPS = !isFPS;

		if (!isFPS)
		{
			InitializeTPS();
		}
	}


	// ���_(���@�̖ڂ̈ʒu)
	Vector3 eyepos, refpos;
	if (isFPS)
	{
		Vector3 position;

		position = _targetPos + Vector3(0, 0.2f, 0);

		// �Q�Ɠ_���王�_�ւ̍���
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);

		// ���@�̌��ɉ�荞�ނ��߂̉�]
		Matrix rotmat = Matrix::CreateRotationY(_targetAngle);

		// �����x�N�g������]������
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// ���_
		eyepos = position + cameraV * 0.1f;

		// �Q�Ɠ_
		refpos = position + cameraV;

	}

	if (isFPS != true)
	{
		// ���@�̏��1m���Q�Ɠ_�Ƃ���
		refpos = _targetPos + Vector3(0, 1, 0);

		// �Q�Ɠ_�Ǝ��_�̍����̃x�N�g��
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);

		// ���@�̌��ɉ�荞�ނ��߂̉�]�s��𐶐�
		Matrix rotmat = Matrix::CreateRotationY(_targetAngle);

		// �����x�N�g������]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// �J�������W���v�Z
		eyepos = refpos + cameraV;

		// �S���R�J����
		eyepos = m_eyePos + (eyepos - m_eyePos)/* * 0.9f*/;

		refpos = m_refPos + (refpos - m_refPos) /** 0.8f*/;
	}
	SetEyePos(eyepos);

	SetRefPos(refpos);

	// ���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::InitializeTPS()
{
	Vector3 eyePos, refPos;

	// ���@�̏��2m�̈ʒu�𑨂���
	refPos = _targetPos + Vector3(0, 2, 0);

	// �Q�Ɠ_���王�_�ւ̍���
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);

	// ���@�̌��ɉ�荞�ނ��߂̉�]
	Matrix rotmat = Matrix::CreateRotationY(_targetAngle);

	// �����x�N�g������]������
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	// �n�_���W���v�Z
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
