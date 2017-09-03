//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Stage.h
//!
//! @brief  �X�e�[�W�֘A�̃w�b�_�[�t�@�C��
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

	// �X�e�[�W�I�u�W�F�N�g(vector�R���e�i�̓񎟌��z��)
	std::vector<Obj3d> m_stageObj;

	// csv�t�@�C����
	std::string m_csvFile;

	// ���W
	DirectX::SimpleMath::Vector3 m_position;

	// �}�b�v�f�[�^
	std::vector<std::vector<int>> m_data;

	std::vector<int> m_mapCnt;

public:
	// �R���X�g���N�^
	Stage(/*std::string csvFile*/);

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// CSV�t�@�C���Z�b�g�p�֐�
	void SetCsvFile(const std::string csvFile);

	// �}�b�v�f�[�^�擾
	void SetMapData(const char delim = ',');



	//// ���W���擾
	//const DirectX::SimpleMath::Vector3& GetPosition();

	//// ���W��ݒ�
	//void SetPosition(const DirectX::SimpleMath::Vector3& trans);

	//// ���[���h�s����擾
	//const DirectX::SimpleMath::Matrix& GetLocalWorld();


	~Stage();
};

