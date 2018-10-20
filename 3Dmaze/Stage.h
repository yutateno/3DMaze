#pragma once
#include "DxLib.h"

class Stage
{
private:
	int drawstagehandle;	// �X�e�[�W�n���h��

	// ���Ȃǂ̓���
	float shapecount;	// �V�F�C�v�J�E���g
	bool shapeup;		// �V�F�C�v�A�b�v�̊J�n
	bool shapedown;		// �V�F�C�v�A�b�v�̏I���J�n

	int stagepshader;		// �s�N�Z���V�F�[�_��ǂݍ��ޕϐ�
	int stagevshader;		// ���_�V�F�[�_��ǂݍ��ޕϐ�

public:
	Stage(int StageHandle);		// �X�e�[�W�n���h���������Ɏ��R���X�g���N�^
	~Stage();					// �f�X�g���N�^

	void Draw(int stagenum);	// �`��

	void ShapeStage(bool shapeup, bool shapedown, int stagenum);	// �V�F�C�v�̓����̊֐�
};