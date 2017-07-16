//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Stage.h
//!
//! @brief  ステージ関連のヘッダーファイル
//!
//! @date   2017/07/17
//!
//! @author E.Ogisu
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include <string>
#include <vector>

#pragma once
class Stage
{
private:
	// csvファイル名
	std::string m_csvFile;

public:
	// コンストラクタ
	Stage(std::string csvFile);

	// マップデータ取得
	bool LoadMapData(std::vector<std::vector<std::string>>& csvFile, const char delim = ',');

	~Stage();
};

