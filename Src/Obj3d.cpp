#include "Obj3d.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

// �ÓI�����o�ϐ��̎���

// �f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;

// �f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;

// �J����
Camera* Obj3d::m_camera;

// �ėp�`��X�e�[�g
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;

// �G�t�F�N�g�t�@�N�g��
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera * camera)
{
	m_d3dDevice = device;
	m_d3dContext = context;
	m_camera = camera;

	// �ėp�`��X�e�[�g����
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	// �G�t�F�N�g�t�@�N�g������
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	m_factory->SetDirectory(L"Resources");
}

Obj3d::Obj3d()
{
	m_scale = Vector3(1, 1, 1);

	m_parent = nullptr;

	// �f�t�H���g�ł̓I�C���[�p�Ōv�Z
	_useQuaternion = false;

}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	m_model = Model::CreateFromCMO(
		m_d3dDevice.Get(), fileName, *m_factory);
}

void Obj3d::Update()
{
	// ��ɍs��̌v�Z
	// �X�P�[��
	Matrix scalemat = Matrix::CreateScale(m_scale);
	// ���s�ړ�
	Matrix transmat = Matrix::CreateTranslation(m_translation);

	// ��]�s��
	Matrix rotmat;
	if (_useQuaternion)
	{
		rotmat = Matrix::CreateFromQuaternion(_rotationQ);
	}
	else
	{
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX*rotmatY;

	}

	// ���[���h�s�������
	m_world = scalemat * rotmat * transmat;

	// �e�̍s���������
	if (m_parent)
	{
		m_world *= m_parent->m_world;
	}

}

void Obj3d::Draw()
{
	assert(m_camera);
	// 3d���f���̕`��
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states, m_world,
			m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	}
}
