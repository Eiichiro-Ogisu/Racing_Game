#include "pch.h"
#include "Stage.h"

#include<fstream>
#include<iostream>
#include<string>
#include<sstream>

using namespace std;

/// <summary>
/// CSV代入
/// </summary>
/// <param name="csvFile">CSVファイル</param>
Stage::Stage(string csvFile)
{
	this->m_csvFile = csvFile;
}

/// <summary>
/// CSV読込
/// </summary>
/// <param name=""></param>
/// <param name="delim">デリミッタ</param>
/// <returns>true:成功,false:失敗</returns>
bool Stage::LoadMapData(vector<vector<string>>& data, const char delim = ',')
{
	// ファイルオープン
	ifstream ifs(m_csvFile);
	if (!ifs.is_open()) return false;	// 読込失敗

	// ファイル読込
	string buf;					// 1行分のバッファ

	while (getline(ifs,buf))
	{
		vector<string> rec;		// 1行分のベクター配列

		istringstream iss(buf);	// 文字列ストリーム

		string field;			//1列分文字列

		// 1列分文字列を1行分ベクターに追加
		while (getline(iss, field, delim))
		{
			rec.push_back(field);
		}

		// 1行分ベクタをdataベクターに追加
		if (rec.size() != 0)
		{
			data.push_back(rec);
		}
	}
	return true;
}


Stage::~Stage()
{
}
