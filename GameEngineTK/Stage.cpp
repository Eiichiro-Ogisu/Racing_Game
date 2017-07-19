//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Stage.cpp
//!
//! @brief  ステージ関連のソースファイル
//!
//! @date   2017/07/18
//!
//! @author E.Ogisu
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "pch.h"
#include "Stage.h"

#include<iostream>
#include<string>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="csvFile">CSVファイル名</param>
Stage::Stage(string csvFile)
{
	this->m_csvFile = csvFile;
}

/// <summary>
/// 初期化処理
/// </summary>
void Stage::Initialize()
{
	// モデルの読み込み
	m_stageObj.resize(1);
	m_stageObj[0].LoadModel(L"Resources\\stageBox.cmo");

	// 座標の設定
	m_stageObj[0].SetTransform(Vector3(0, 0, 0));
}
/// <summary>
/// 更新処理
/// </summary>
void Stage::Update()
{
}
/// <summary>
/// 描画処理
/// </summary>
void Stage::Draw()
{
	for (vector<Obj3d>::iterator it = m_stageObj.begin();
		it != m_stageObj.end();
		it++)
	{
		it->Draw();
	}
}

/// <summary>
/// CSV読込
/// </summary>
/// <param name=""></param>
/// <param name="delim">デリミッタ</param>
/// <returns>true:成功,false:失敗</returns>
bool Stage::LoadMapData(vector<vector<string>>& data, const char delim)
{
	// ファイルオープン
	fstream filestream(m_csvFile);

	// 開けなかったら終了
	if (!filestream.is_open())
	{
		return false;
	}

	// ファイルの読み込み
	while (!filestream.eof())
	{
		// 1行分バッファ
		string buffer;

		// 1行読み込む
		filestream >> buffer;

		// ファイルから読み込んだ1行の文字列をデリミッタで分けてリストに追加
		vector<string> record;				// 1行分の文字列リスト

		istringstream streambuffer(buffer);	// 文字列ストリーム

		string token;

		while (getline(streambuffer,token,delim))
		{
			// 1セル分の文字列をリストに追加
			record.push_back(token);
		}
		data.push_back(record);
	}
	return true;
}


Stage::~Stage()
{
}
