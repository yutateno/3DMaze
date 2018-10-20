#pragma once
#include "DxLib.h"

class Load
{
private:
	int gr;			// �w�i�摜
	int gr_bax;		// �w�i�摜
	int baxcount;	// �Ԃ���
	int max;		// �񓯊������̌�
	int scene;
	int playmodel;
	int stagenum;

	bool loadend;

	int DStageHandle;	// �`��p�X�e�[�W�n���h��
	int CStageHandle;	// ����p�X�e�[�W�n���h��
	int ModelHandle;	// �v���C���[���f���n���h��
	int bgm;			// BGM�n���h��

	int TitleStageHandle;		// �X�e�[�W�n���h��
	int TitleModelHandle_one;	// ��ڂ̃��f���n���h��
	int TitleModelHandle_two;	// ��ڂ̃��f���n���h��
	int Titlebgm;				// BGM�n���h��

	int ShadowModel;	// �e�L�����̃��f���n���h��

public:
	Load(int scene, int stagenum, int playmodel);	// �X�e�[�W�ԍ��ƃv���C���f���������Ɏ�����R���X�g���N�^
	~Load();							// �f�X�g���N�^

	void Update();	// �X�V

	// �Q�b�^�[
	int GetDStageH();		// �`��p�X�e�[�W�n���h��
	int GetCStageH();		// ����p�X�e�[�W�n���h��
	int GetModelH();		// ���샂�f���n���h��
	int GetBGM();			// BGM
	// �^�C�g��
	int GetTitleStageH();		// �X�e�[�W�n���h��
	int GetTitleModelH_one();	// ��ڂ̃��f���n���h��
	int GetTitleModelH_two();	// ��ڂ̃��f���n���h��
	int GetTitlebgm();			// BGM�n���h��
	// ���܂�
	int GetShadowM();	//�e�L�����̃��f���n���h��

	bool GetLoadEnd();
};