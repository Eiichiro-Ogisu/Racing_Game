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
/// csvファイルの設定
/// </summary>
/// <param name="csvFile"></param>
void Stage::SetCsvFile(const std::string csvFile)
{
	m_csvFile = csvFile;
}

/// <summary>
/// csv読み込み
/// </summary>
/// <param name="data">1行分データ</param>
/// <param name="delim">デリミッタ</param>
/// <returns>マップデータの配列</returns>
void Stage::GetMapData(const char delim)
{
	// ファイルオープン
	ifstream ifs(m_csvFile);

	// 開けなかったら終了
	if (!ifs)
	{
		exit(1);
	}

	// ファイルから読み込んだ1行の文字列をデリミッタで分けてリストに追加
	
	vector<vector<int>> record;					// 完成したデータ配列

	vector<int> lineRecord;				// 1行分の文字列リスト
	// ファイルの読み込み
	while (!ifs.eof())
	{
		char c;
		string msg = "";

		//
		istringstream iss(msg);

		string tmp;

		while (getline(ifs,tmp))
		{
			string token;

			istringstream stream(tmp);

			while (getline(stream,token,delim))
			{
				int temp = stof(token);

				lineRecord.push_back(temp);
			}
			record.push_back(lineRecord);
			lineRecord.clear();

		}


		//while (filestream.get(c)) 
		//{
		//	if (c != '\n') {
		//		msg += c;
		//	}
		//	else {
		//		msg += ',';
		//	}
		//}

		//// 1行分バッファ
		//string tmp;

		//istringstream iss(msg);

		//while (getline(iss, tmp, delim))
		//{
		//	// 1セル分の文字列をリストに追加
		//	record.push_back(tmp);
		//}
	}
	m_data = record;
}

Stage::~Stage()
{
}
