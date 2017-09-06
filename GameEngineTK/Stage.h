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
#include <array>
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

	// ステージオブジェクト(vectorコンテナの二次元配列)
	std::vector<Obj3d> m_stageObj;

	// csvファイル名
	std::string m_csvFile;

	// 座標
	DirectX::SimpleMath::Vector3 m_position;

	// マップデータ
	std::vector<std::vector<int>> m_data;

	//std::array<std::array<int,0>,0> m_mapData;

	std::vector<int> m_mapCnt;

public:
	// コンストラクタ
	Stage(std::string csvData);

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// CSVファイルセット用関数
	void SetCsvFile(const std::string csvFile);

	// マップデータ取得
	void SetMapData(const char delim = ',');

	~Stage();
};

