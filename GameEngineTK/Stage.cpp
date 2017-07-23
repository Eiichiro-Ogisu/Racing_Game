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

#include<string>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="csvFile">CSVファイル名</param>
Stage::Stage(/*string csvFile*/)
{
	//this->m_csvFile = csvFile;

	//// 初期化呼び出し
	//Initialize();
}

/// <summary>
/// 初期化処理
/// </summary>
void Stage::Initialize()
{
	//// モデルの読み込み
	//m_stageObj.resize(1);
	//m_stageObj[0][].LoadModel(L"Resources\\stageBox.cmo");

	//// 座標の設定
	//m_stageObj[0][].SetTransform(Vector3(0, 0, 0));
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
void Stage::Draw(vector<string> mapData)
{
	for (vector<vector<Obj3d>>::iterator it = m_stageObj.begin();
		it != m_stageObj.end();
		it++)
	{
		
	}
}

/// <summary>
/// csvファイルのセット
/// </summary>
/// <param name="csvFile">csvファイル</param>
void Stage::SetCsvFile(const std::string csvFile)
{
	m_csvFile = csvFile;
}

/// <summary>
/// マップデータをセット
/// </summary>
/// <param name="delim">区切り文字</param>
void Stage::SetMapData(const char delim)
{
	// ファイルオープン
	ifstream ifs(m_csvFile);

	// 開けなかったら終了
	if (!ifs)
	{
		exit(1);
	}

	// ファイルから読み込んだ1行の文字列をデリミッタで分けてリストに追加
	
	// 完成したデータ配列
	vector<vector<int>> record;

	// 1行分の文字列リスト
	vector<int> rowRecord;

	// ファイルの終了まで読み込んだかチェック
	while (!ifs.eof())
	{
		// 1行分のデータ
		string tmp;

		// 1行読み込む
		while (getline(ifs,tmp))
		{
			// 1文字分のデータ保持用
			string token;

			// 文字列ストリーム
			istringstream stream(tmp);

			// カンマを除いた1文字ずつ配列に格納
			while (getline(stream,token,delim))
			{
				// string型をfloat 型に変換
				int temp = stof(token);

				// 1行分のデータ配列の中に1文字分格納
				rowRecord.push_back(temp);
			}
			// 二次元配列に1行分のデータを格納
			record.push_back(rowRecord);

			// 1行分のデータをクリア
			rowRecord.clear();

		}
	}

	// マップデータに二次元配列を代入
	m_data = record;
}

Stage::~Stage()
{
}
