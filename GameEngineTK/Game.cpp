//
// Game.cpp
//

#include "pch.h"
#include "Game.h"


extern void ExitGame();

using namespace DirectX;

using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

//std::unique_ptr<BasicEffect> basicEffect;
//
//ComPtr<ID3D11InputLayout> inputLayout;
//
//std::unique_ptr<PrimitiveBatch<VertexPositionColor>> primitiveBatch;	// VertexPositionColor = 座標と色を持った頂点情報

Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	// 初期化はここに	
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	dxtk.Initializer(m_d3dDevice.Get(),m_d3dContext.Get());

	// キーボードの初期化
	//_keyboard = std::make_unique<Keyboard>();
	// カメラの生成
	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	
	// 3dオブジェクトの静的メンバを初期化
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	// プレイヤーの生成 
	_player = new Player();


	// カメラにキーボード設置
	//m_camera->SetKeyboard(_keyboard.get());


					// new
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());
																// 普通のポインターに置き換える
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	//// 縦幅、横幅の設定
	//m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
	//	m_outputWidth, m_outputHeight, 0, 0, 1));

	// 3D用

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	// デバッグカメラ生成
	//m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth,m_outputHeight);

	 //エフェクトファクトリ生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	 //テクスチャの読み込みパス指定
	m_factory->SetDirectory(L"Resources");

	// モデルの読み込み
	m_modelGround = Model::CreateFromCMO(m_d3dDevice.Get(),L"Resources\\Ground200m.cmo",*m_factory);

	m_objSkydome.LoadModel(L"Resources\\Skydome.cmo");
	
	m_skydome2 = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\Skydome2.cmo", *m_factory);

	m_teapot = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\teapot.cmo", *m_factory);

	//m_modelHead = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\head.cmo", *m_factory);

	// ティーポットのワールド座標設定
	for (int i = 0; i < 20; i++)
	{
		m_x[i] = rand() % 200 - 100;

		m_z[i] = rand() % 200 - 100;

		Matrix scaleMat = Matrix::CreateScale(2.0f);

		buf[i] = Matrix::CreateTranslation(m_x[i], 0, m_z[i]);
	}

	//// 自機パーツの読み込み
	//m_objPlayer1.resize(PLAYER_PARTS_NUM);
	//m_objPlayer1[PLAYER_PARTS_LEG].LoadModel(L"Resources\\leg.cmo");
	//m_objPlayer1[PLAYER_PARTS_BODY].LoadModel(L"Resources\\body.cmo");
	//m_objPlayer1[PLAYER_PARTS_BATTERY].LoadModel(L"Resources\\battery.cmo");
	//m_objPlayer1[PLAYER_PARTS_BATTERY2].LoadModel(L"Resources\\battery.cmo");
	//m_objPlayer1[PLAYER_PARTS_HAND].LoadModel(L"Resources\\hand.cmo");
	//m_objPlayer1[PLAYER_PARTS_HAND2].LoadModel(L"Resources\\hand.cmo");
	//m_objPlayer1[PLAYER_PARTS_HEAD].LoadModel(L"Resources\\head.cmo");


	//// パーツの親子関係をセット
	//m_objPlayer1[PLAYER_PARTS_HEAD].SetParent(
	//	&m_objPlayer1[PLAYER_PARTS_BODY]);

	//m_objPlayer1[PLAYER_PARTS_HAND].SetParent(
	//	&m_objPlayer1[PLAYER_PARTS_BODY]);

	//m_objPlayer1[PLAYER_PARTS_HAND2].SetParent(
	//	&m_objPlayer1[PLAYER_PARTS_BODY]);

	//m_objPlayer1[PLAYER_PARTS_LEG].SetParent(
	//	&m_objPlayer1[PLAYER_PARTS_BODY]);

	//m_objPlayer1[PLAYER_PARTS_BATTERY].SetParent(
	//	&m_objPlayer1[PLAYER_PARTS_BODY]);

	//m_objPlayer1[PLAYER_PARTS_BATTERY2].SetParent(
	//	&m_objPlayer1[PLAYER_PARTS_BODY]);


	//// 親からのオフセット
	//// ボディの座標をいい感じの位置に
	//m_objPlayer1[PLAYER_PARTS_BODY].SetTransform(Vector3(0,0.25,0));

	//// 足をいい感じに
	//m_objPlayer1[PLAYER_PARTS_LEG].SetScale(Vector3(0.8, 0.25, 0.5));

	//m_objPlayer1[PLAYER_PARTS_LEG].SetTransform(Vector3(0, -0.25, 0));

	//// 両手をいい感じに
	//m_objPlayer1[PLAYER_PARTS_HAND].SetTransform(Vector3(-0.5, 0.6, -0.2));
	//m_objPlayer1[PLAYER_PARTS_HAND].SetScale(Vector3(0.25, 0.25, 0.75));

	//m_objPlayer1[PLAYER_PARTS_HAND2].SetTransform(Vector3(0.5, 0.6, -0.2));
	//m_objPlayer1[PLAYER_PARTS_HAND2].SetScale(Vector3(0.25, 0.25, 0.75));

	//// 頭をいい感じに
	//m_objPlayer1[PLAYER_PARTS_HEAD].SetTransform(Vector3(0, 1, 0));
	//m_objPlayer1[PLAYER_PARTS_HEAD].SetScale(Vector3(1.5, 1.5, 1.5));

	//// タンクをいい感じに
	//m_objPlayer1[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0, 135, -0.55));
	//m_objPlayer1[PLAYER_PARTS_BATTERY].SetTransform(Vector3(-0.5,0.9,-0.15));
	//m_objPlayer1[PLAYER_PARTS_BATTERY].SetScale(Vector3(0.5, 0.5, 0.5));


	//m_objPlayer1[PLAYER_PARTS_BATTERY2].SetRotation(Vector3(0, 135, 0.55));
	//m_objPlayer1[PLAYER_PARTS_BATTERY2].SetTransform(Vector3(0.5, 0.9, -0.15));
	//m_objPlayer1[PLAYER_PARTS_BATTERY2].SetScale(Vector3(0.5, 0.5, 0.5));

}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	//g_key = _keyboard->GetState();

	//// ここ危ない
	//Keyboard::State keyboardState = _keyboard->GetState();

	//m_PlayerKey.Update(keyboardState);

	// DXTKを管理するインスタンスを取得
	DXTK::DXTKResources& dxtk = DXTK::DXTKResources::singleton();

	dxtk.UpdateInputState();

	// キ-ボードの状態
	Keyboard::State keyboardState = dxtk.m_keyboard->GetState();


    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    //elapsedTime;

	_player->Update();

	{// 自機に追従するカメラ
		m_camera->SetTargetPos(_player->GetPosition());

		m_camera->SetTargetAngle(_player->GetAngle().y);

		m_camera->SetFovY(XMConvertToRadians(60.0f));

		m_camera->SetAspect((float)m_outputWidth / m_outputHeight);

		m_camera->SetNearclip(0.01f);

		m_camera->SetFarclip(1000.0f);

		m_camera->SetUpVec(Vector3(0, 1, 0));

		m_camera->Update();
	}

	m_objSkydome.Update();

	// ビュー行列を取得
	m_view = m_camera->GetViewMatrix();
	m_proj = m_camera->GetProjectionMatrix();


	m_angle++;


	if (m_scale2 >= 300)
	{
		scaleFlag = false;
	}
	else if (m_scale2 <= 0)
	{
		scaleFlag = true;
	}

	if (scaleFlag == false)
	{
		m_scale2--;
	}
	else if(scaleFlag == true)
	{
		m_scale2++;
	}


	m_scale = Matrix::CreateScale(m_scale2 / 60);

	Matrix translation;

	Matrix transRotY = Matrix::CreateRotationY(XMConvertToRadians(m_angle));

	for (int i = 0; i < 20; i++)
	{
		translation = m_scale * transRotY * buf[i];

		m_worldTeapot[i] = translation;

		buf[i] = Matrix::CreateTranslation(m_x[i]*worldTimer,0,m_z[i]*worldTimer);
	}

	if (worldTimer >= 0)
	{
		worldTimer -= 0.1 / 60;
	}

	//// キー操作
	//if (g_key.W)
	//{
	//	// 移動量
	//	Vector3 moveV = Vector3(0.0f, 0.0f, -0.1f);

	//	float angle = m_objPlayer1[PLAYER_PARTS_BODY].GetRotation().y;

	//	Matrix rotmat = Matrix::CreateRotationY(angle);

	//	moveV = Vector3::TransformNormal(moveV,rotmat);

	//	// 自機移動
	//	Vector3 pos = m_objPlayer1[PLAYER_PARTS_BODY].GetTranslation();
	//	m_objPlayer1[PLAYER_PARTS_BODY].SetTransform(pos + moveV);
	//}

	//if (g_key.S)
	//{
	//	// 移動量
	//	Vector3 moveV = Vector3(0.0f, 0.0f, 0.1f);

	//	float angle = m_objPlayer1[PLAYER_PARTS_BODY].GetRotation().y;

	//	Matrix rotmat = Matrix::CreateRotationY(angle);

	//	moveV = Vector3::TransformNormal(moveV, rotmat);

	//	// 自機移動
	//	Vector3 pos = m_objPlayer1[PLAYER_PARTS_BODY].GetTranslation();
	//	m_objPlayer1[PLAYER_PARTS_BODY].SetTransform(pos + moveV);
	//}

	//if (g_key.A)
	//{
	//	// 回転量
	//	//float rot = 0.03f;
	//	float angle = m_objPlayer1[PLAYER_PARTS_BODY].GetRotation().y;
	//	m_objPlayer1[PLAYER_PARTS_BODY].SetRotation(Vector3(0, angle + 0.03f, 0));

	//	//m_objPlayer1[PLAYER_PARTS_BODY]
	//}

	//if (g_key.D)
	//{
	//	// 回転量
	//	//float rot = -0.03f;
	//	float angle = m_objPlayer1[PLAYER_PARTS_BODY].GetRotation().y;
	//	m_objPlayer1[PLAYER_PARTS_BODY].SetRotation(Vector3(0, angle - 0.03f, 0));

	//}

	//for (std::vector<Obj3d>::iterator it = m_objPlayer1.begin(); 
	//	it != m_objPlayer1.end();
	//	it++)
	//{
	//	it->Update();
	//}

	/// <summary>
	/// 近距離攻撃 後で関数化
	/// </summary>
	if (keyboardState.E)
	{
		_shortRangeAttackRot += 0.5;

		//m_objPlayer1[PLAYER_PARTS_HAND].SetRotation(Vector3(0, 0, _shortRangeAttackRot));
		//m_objPlayer1[PLAYER_PARTS_HAND2].SetRotation(Vector3(0, 0, -_shortRangeAttackRot));

		//m_objPlayer1[PLAYER_PARTS_HAND].SetScale(Vector3(0.25, 0.25, 1.25));
		//m_objPlayer1[PLAYER_PARTS_HAND2].SetScale(Vector3(0.25, 0.25, 1.25));
		
		// ここ危ない
		//if (m_PlayerKey.IsKeyReleased(Keyboard::Keys::E))
		//{
		//	m_objPlayer1[PLAYER_PARTS_HAND].SetScale(Vector3(0.25, 0.25, 0.75));
		//	m_objPlayer1[PLAYER_PARTS_HAND2].SetScale(Vector3(0.25, 0.25, 0.75));
		//}

	}

	//{// 自機のワールド行列を計算

	//	// パーツ1(親)
	//	Matrix rot = Matrix::CreateRotationY(tankRot);

	//	Matrix trans = Matrix::CreateTranslation(tankPos);

	//	// ワールド行列を合成
	//	m_worldHead = rot * trans;

	//	// パーツ2(子)
	//	Matrix rot2 = Matrix::CreateRotationZ(XMConvertToRadians(90))*Matrix::CreateRotationY(0);

	//	Matrix trans2 = Matrix::CreateTranslation(Vector3(0,0.5f,0));

	//	m_worldHead2 = rot2 * trans2 * m_worldHead;

	//}
}

// Draws the scene.
void Game::Render()
{
	// 頂点インデックス
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3
	};
	
	// 頂点座標
	VertexPositionNormal vertices[] =
	{
		// 法線方向ベクトル
		{ Vector3(-1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(-1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
	};


    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.

	// 描画処理
	DirectX::CommonStates m_states(m_d3dDevice.Get());
	
	// 地面モデルの描画
	m_modelGround->Draw(m_d3dContext.Get(), m_states, m_world, m_view, m_proj);

	m_objSkydome.Draw();


	//for (int i = 0; i < 10; i++)
	//{
	//	m_skydome2->Draw(m_d3dContext.Get(), m_states, m_worldBall[i], m_view, m_proj);

	//	m_skydome2->Draw(m_d3dContext.Get(), m_states, m_worldBall2[i], m_view, m_proj);

	//}
			
	m_modelGround->Draw(m_d3dContext.Get(), m_states, m_worldGround, m_view, m_proj);
	//// ティーポットのモデルの描画
	//for (int i = 0; i < 20; i++)
	//{
	//	m_teapot->Draw(m_d3dContext.Get(), m_states, m_worldTeapot[i], m_view, m_proj);
	//}

	//// 頭部モデル
	//m_modelHead->Draw(m_d3dContext.Get(), m_states, m_worldHead, m_view, m_proj);
	//// 頭部モデル2
	//m_modelHead->Draw(m_d3dContext.Get(), m_states, m_worldHead2, m_view, m_proj);

	// プレイヤーの描画
	_player->Draw();

	// タンクの平行移動

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}