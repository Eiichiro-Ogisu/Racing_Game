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
	//m_mapCnt[] = 0;

	m_stageObj.resize(8);

	int a = 0;
}

/// <summary>
/// 初期化処理
/// </summary>
void Stage::Initialize()
{
	// モデルの読み込み
	//m_stageObj.resize(1);

	for (int i = 0; i < m_stageObj.size(); i++)
	{
		m_stageObj[i].LoadModel(L"Resources\\stageBox.cmo");

		m_stageObj[i].SetScale(Vector3(1,1,1));
	}
}
/// <summary>
/// 更新処理
/// </summary>
void Stage::Update()
{
	for (auto it = m_stageObj.begin(); it != m_stageObj.end(); it++)
	{
		it->Update();
	}
}
/// <summary>
/// 描画処理	// 時間が余れば書き直したい...
/// </summary>
void Stage::Draw()
{
	int i_ = 0;

	int row = 0,colum = 0;
	for (auto i = m_data.begin(); i != m_data.end(); i++)
	{
		vector<int> v = *i;

		for (auto j = v.begin(); j != v.end(); j++)
		{
			m_stageObj[i_].SetTransform(Vector3(colum,0,row));
			if (m_data[row][colum] == 1)
			{
				int a = 0;
				m_stageObj[i_].Draw();
			}

			colum++;
			i_++;
		}
		row++;
		colum = 0;
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
