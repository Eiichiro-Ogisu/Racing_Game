//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Stage.h
//!
//! @brief  ステージ関連のヘッダーファイル
//!
//! @date   2017/07/17
//!
//! @author E.Ogisu
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include <fstream>
#include <string>
#include <vector>
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <fstream>
#include <sstream>

#include "DXTKResouces.h"
#include "obj3d.h"

#pragma once
class Stage
{
private:

	// ステージオブジェクト
	std::vector<Obj3d> m_stageObj;

	// csvファイル名
	std::string m_csvFile;

	// 座標
	DirectX::SimpleMath::Vector3 m_position;

public:
	// コンストラクタ
	Stage(std::string csvFile);

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// マップデータ取得
	bool LoadMapData(std::vector<std::vector<std::string>>& data, const char delim = ',');

	~Stage();
};

