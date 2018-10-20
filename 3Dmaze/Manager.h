#pragma once
#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Load.h"

class Manager
{
private:
	// ����
	eScene preScene;	// ���O�̃V�[��
	eScene nowScene;	// ���̃V�[��

	Scene* scene;	// �V�[���̖{��
	Load* load;

	void Manage();	// �V�[���ύX����

	int stagenum;	// �X�e�[�W�ԍ�

	int playmodel;		// �v���C���[���f���ԍ�
	bool cameratype;	// �J�������[�h�̔ԍ�

public:
	Manager();	// �R���X�g���N�^
	~Manager();	// �f�X�g���N�^

	void Update();	// �X�V
};