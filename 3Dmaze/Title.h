#pragma once
#include "Scene.h"
#include "Character.h"
#include "Camera.h"
#include <math.h>

class Title : public Scene
{
private:
	// �摜
	int gr_icon;			// �A�C�R���̉摜
	// �ŏ�
	int gr_start;			// �X�^�[�g�R�}���h
	int gr_end;				// �I���R�}���h
	// 2�Ԗ�
	int gr_player1;			// �ŏ��̃L�����̐����i�V�L�~�j
	int gr_player2;			// ��Ԗڂ̃L�����̐����i�A�J�l�j
	int gr_chara_ok;		// �L�����I����OK
	int gr_chara_no;		// �L�����I����NO
	// 3�Ԗ�
	int gr_camera1;			// �J�������[�h�P
	int gr_camera2;			// �J�������[�h�Q
	int gr_cameraimage;		// �J�����̃C���[�W�}
	// 4�Ԗ�
	int gr_last;			// �Ō�ɂ������ǂ����̔w�i�摜
	int gr_ok;				// �Ō��OK
	int gr_no;				// �Ō��NO
	// �Ō�
	int gr_red;				// �t�F�[�h�A�E�g�̎��̉摜
	// ���܂�
	int gr_bonus;			// ���܂��̃X�^�[�g�R�}���h
	int gr_returnreturn;	// ���܂�����߂�R�}���h
	int gr_black;

	// ��
	int se_select;			// �I����
	int se_kettei;			// ���艹
	int se_return;			// �߂鉹

	// �J�[�\���ʒu
	int iconY;	// �J�[�\����Y���W
	int iconX;	// �J�[�\����X���W

	int menumode;	//0:�J�n 1:�L�������f�� 2:������ 3:�J���� 4:�ŏI

	bool bonus;		// ���܂��T�C�h�Ɉڂ邩�ǂ���

	// �ʒu���W
	VECTOR camera;		// �J����
	VECTOR view;		// �J�����̒���
	VECTOR character;	// �L�����N�^�[�P
	VECTOR character2;	// �L�����N�^�[�Q
	VECTOR light;		// ����

	// ����
	Character* objectchara1;	// �L�����N�^�[�P
	Character* objectchara2;	// �L�����N�^�[�Q
	Character* bonuschara;		// �{�[�i�X�L�����N�^�[
	Camera* bonuscamera;		// �{�[�i�X���̃J����

	int TitleStageHandle;		// �X�e�[�W�n���h��
	int TitleModelHandle_one;	// ��ڂ̃��f���n���h��
	int TitleModelHandle_two;	// ��ڂ̃��f���n���h��
	int Titlebgm;				// BGM�n���h��
	int ShadowModel;	// �e�L�����̃��f���n���h��

	int stagepshader;		// �s�N�Z���V�F�[�_
	int stagevshader;		// ���_�V�F�[�_

	void Process();		// �v���Z�X
	void Draw();		// �`��

public:
	Title(int TitleStageHandle, int TitleModelHandle_one, int TitleModelHandle_two, int Titlebgm, int ShadowModel);		// �R���X�g���N�^
	~Title();																											// �f�X�g���N�^

	void Update();		// �X�V
};