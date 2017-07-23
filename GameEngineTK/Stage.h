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

	// ステージオブジェクト(vectorコンテナの二次元配列)
	std::vector<std::vector<Obj3d>> m_stageObj;

	// csvファイル名
	std::string m_csvFile;

	// 座標
	DirectX::SimpleMath::Vector3 m_position;

	// マップデータ
	std::vector<std::vector<int>> m_data;


public:
	// コンストラクタ
	Stage(/*std::string csvFile*/);

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(std::vector<std::string> mapData);

	// CSVファイルセット用関数
	void SetCsvFile(const std::string csvFile);

	// マップデータ取得
	void GetMapData(const char delim = ',');

	~Stage();
};

