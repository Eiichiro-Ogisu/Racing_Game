#include "Obj3d.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

// 静的メンバ変数の実態

// デバイス
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;

// デバイスコンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;

// カメラ
Camera* Obj3d::m_camera;

// 汎用描画ステート
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;

// エフェクトファクトリ
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera * camera)
{
	m_d3dDevice = device;
	m_d3dContext = context;
	m_camera = camera;

	// 汎用描画ステート生成
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	// エフェクトファクトリ生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// テクスチャの読み込みパス指定
	m_factory->SetDirectory(L"Resources");
}

Obj3d::Obj3d()
{
	m_scale = Vector3(1, 1, 1);

	m_parent = nullptr;

	// デフォルトではオイラー角で計算
	_useQuaternion = false;

}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	m_model = Model::CreateFromCMO(
		m_d3dDevice.Get(), fileName, *m_factory);
}

void Obj3d::Update()
{
	// 主に行列の計算
	// スケール
	Matrix scalemat = Matrix::CreateScale(m_scale);
	// 平行移動
	Matrix transmat = Matrix::CreateTranslation(m_translation);

	// 回転行列
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

	// ワールド行列を合成
	m_world = scalemat * rotmat * transmat;

	// 親の行列をかける
	if (m_parent)
	{
		m_world *= m_parent->m_world;
	}

}

void Obj3d::Draw()
{
	assert(m_camera);
	// 3dモデルの描画
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states, m_world,
			m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	}
}
