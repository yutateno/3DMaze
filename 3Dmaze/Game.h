#pragma once
#include "Character.h"
#include "Camera.h"
#include "Stage.h"
#include "Scene.h"

class Game : public Scene
{
private:
	// ���̊֘A
	Character* character;	// �L�����N�^�[
	Camera* camera;			// �J����
	Stage* stage;			// �X�e�[�W

	int DStageHandle;	// �`��p�X�e�[�W�n���h��
	int CStageHandle;	// ����p�X�e�[�W�n���h��
	int ModelHandle;	// �v���C���[���f���n���h��
	int bgm;			// BGM�n���h��

	// ���X�g�t�F�[�h�A�E�g
	int gr_white;			// ��
	int gr_black;

	// �G���f�B���O
	int gr_end;				// �G���f�B���O�̔w�i
	bool lastendflag;		// �I���t���b�O
	int lastendcount;		// ���񂾂�I���
	int gr_lastsign;

	// ���y
	int bgm_sub;			// �����Đ����y
	int bgm_feedout;		// �t�F�[�h�A�E�g�pBGM
	int se_door;			// ���̊J
	int feedsoundcount;		// �N���A�^�f�G���A��BGM���ڂ肩����
	int bgm_bloodwindow;	// ����
	int silencecount;
	int silenceflag;

	// ��
	bool dooropenflag;		// �����J����

	// �����Ɋւ���
	VECTOR listen;		// ���[�U�[�ʒu
	VECTOR prelisten;	// ���O�̃��[�U�[�ʒu
	VECTOR sePos;		// �����̈ʒu
	int se_right;		// �E���̃T�E���h�n���h��
	int se_left;		// �����̃T�E���h�n���h��
	bool footLR;		// 0:R 1:L
	int movespeed;		// �����X�s�[�h
	int secount;		// �ҋ@����
	bool moveflag;		// �����Ă��邩�ǂ���
	bool sestart;		// �����o��

	// �X�e�[�W�ԍ��i�X�e�[�W�G���h�ɕK�v
	int stagenum;		// �X�e�[�W�̔ԍ�

	void Draw();		// �`��
	void Process();		// �v���Z�X
	void Sound();		// �T�E���h

	// �e����-------------------------------------------------------
	// �e�p�̐[�x�l��ۑ�����`��\�O���t�B�b�N
	int DepthBufferGraphHandle;

	// �e�p�̐[�x�L�^�摜���쐬�����ۂ̃J�����̃r���[�s��Ǝˉe�s��
	MATRIX LightCamera_ViewMatrix;
	MATRIX LightCamera_ProjectionMatrix;

	// �[�x�L�^�摜�ւ̕`��p�̍��̃��b�V���p���_�V�F�[�_�[��
	// �X�L�j���O���b�V���p�̒��_�V�F�[�_�[
	int Skin4_DepthShadow_Step1_VertexShader;
	int Normal_DepthShadow_Step1_VertexShader;

	// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[
	int DepthShadow_Step1_PixelShader;

	// �[�x�L�^�摜���g�����f�B���N�V���i�����C�g��t���̕`��p�̍��̃��b�V���p���_�V�F�[�_�[��
	// �X�L�j���O���b�V���p�̒��_�V�F�[�_�[
	int Skin4_DirLight_DepthShadow_Step2_VertexShader;
	int Normal_DirLight_DepthShadow_Step2_VertexShader;

	// �[�x�L�^�摜���g�����f�B���N�V���i�����C�g��t���̕`��p�̃s�N�Z���V�F�[�_�[
	int DirLight_DepthShadow_Step2_PixelShader;

	VECTOR LightDirection;
	VECTOR LightPosition;
	VECTOR LightTarget;
	// �e����-------------------------------------------------------

public:
	Game(int ModeHandle, int CStageHandle, int DStageHanle, int bgm, int stagenum, bool cameratype, int playmodel);	// �����������񎝂R���X�g���N�^(.cpp�ɂĐ���)
	~Game();																							// �f�X�g���N�^

	void Update();	// �X�V
};