//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Stage.cpp
//!
//! @brief  �X�e�[�W�֘A�̃\�[�X�t�@�C��
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
/// �R���X�g���N�^
/// </summary>
/// <param name="csvFile">CSV�t�@�C����</param>
Stage::Stage(/*string csvFile*/)
{
	//this->m_csvFile = csvFile;

	//// �������Ăяo��
	//Initialize();
	//m_mapCnt[] = 0;

	m_stageObj.resize(8);

	int a = 0;
}

/// <summary>
/// ����������
/// </summary>
void Stage::Initialize()
{
	// ���f���̓ǂݍ���
	//m_stageObj.resize(1);

	for (int i = 0; i < m_stageObj.size(); i++)
	{
		m_stageObj[i].LoadModel(L"Resources\\stageBox.cmo");

		m_stageObj[i].SetScale(Vector3(1,1,1));
	}
}
/// <summary>
/// �X�V����
/// </summary>
void Stage::Update()
{
	for (auto it = m_stageObj.begin(); it != m_stageObj.end(); it++)
	{
		it->Update();
	}
}
/// <summary>
/// �`�揈��	// ���Ԃ��]��Ώ�����������...
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
/// csv�t�@�C���̃Z�b�g
/// </summary>
/// <param name="csvFile">csv�t�@�C��</param>
void Stage::SetCsvFile(const std::string csvFile)
{
	m_csvFile = csvFile;
}

/// <summary>
/// �}�b�v�f�[�^���Z�b�g
/// </summary>
/// <param name="delim">��؂蕶��</param>
void Stage::SetMapData(const char delim)
{
	// �t�@�C���I�[�v��
	ifstream ifs(m_csvFile);

	// �J���Ȃ�������I��
	if (!ifs)
	{
		exit(1);
	}

	// �t�@�C������ǂݍ���1�s�̕�������f���~�b�^�ŕ����ă��X�g�ɒǉ�
	
	// ���������f�[�^�z��
	vector<vector<int>> record;

	// 1�s���̕����񃊃X�g
	vector<int> rowRecord;

	// �t�@�C���̏I���܂œǂݍ��񂾂��`�F�b�N
	while (!ifs.eof())
	{
		// 1�s���̃f�[�^
		string tmp;

		// 1�s�ǂݍ���
		while (getline(ifs,tmp))
		{
			// 1�������̃f�[�^�ێ��p
			string token;

			// ������X�g���[��
			istringstream stream(tmp);

			// �J���}��������1�������z��Ɋi�[
			while (getline(stream,token,delim))
			{
				// string�^��float �^�ɕϊ�
				int temp = stof(token);

				// 1�s���̃f�[�^�z��̒���1�������i�[
				rowRecord.push_back(temp);
			}
			// �񎟌��z���1�s���̃f�[�^���i�[
			record.push_back(rowRecord);

			// 1�s���̃f�[�^���N���A
			rowRecord.clear();

		}
	}

	// �}�b�v�f�[�^�ɓ񎟌��z�����
	m_data = record;
}

Stage::~Stage()
{
}
