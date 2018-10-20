#pragma once
#include "DxLib.h"
#include "input.h"
#include <math.h>
#include <string>

using namespace std;

class Character
{
private:
	// ���ꂼ��̈ʒu�Ɋւ���
	VECTOR area;	// �L�����ʒu
	float angle;	// �A���O��

	// �����Ɋւ���
	double speed;		// ��]�X�s�[�h
	double walkspeed;	// �ړ��X�s�[�h
	double animspeed;	// ���[�V�����X�s�[�h

	// ���f���̃}�e���A���Ɋւ���
	int materialnum;	// �֊s���̐�
	double dotwidth;	// �֊s���̕�

	// ���f���Ɋւ���
	int charamodelhandle;	// ���f���̃n���h��
	int playmodel;			// ���샂�f��
	float modelheight;
	float modelwigth;

	// �t���C��
	int framenum;		// �t���C����

	// ���[�V�����Ɋւ���
	int attachmotion;			// ���[�V�����̃A�^�b�`
	int attachnum;				// ���݂̃��[�V�����ԍ�
	double totaltime;			// ���[�V�����̑�������
	double nowplaytime;			// ���[�V�����̌��݂̎���
	double motionblendtime;		// �u�����h����
	int preattach;				// ���O�̃��[�V�����A�^�b�`
	double premotionplaytime;	// ���O�̃��[�V��������

	// �X�e�[�W
	int stagenum;							// �X�e�[�W�̔ԍ�
	int collstagehandle;					// �X�e�[�W�n���h��
	int wallnum;							// ���肷��ǂ̌�
	int floornum;							// ���肷�鏰�̌�
	bool hitflag;							// �������Ă���t���b�O
	bool moveflag;							// �����Ă���t���b�O
	float maxY;								// �������Ă���ő��y���W
	MV1_COLL_RESULT_POLY_DIM hitdim;		// �v���C���[�̎��͂ɂ���|���S�������o�������ʂ��������铖���蔻�茋�ʍ\����
	MV1_COLL_RESULT_POLY* poly;				// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^( �g��Ȃ��Ă��ς܂����܂����v���O�����������Ȃ�̂ŁE�E�E )
	MV1_COLL_RESULT_POLY* wallpoly[2048];	// �Ǘp�̃|���S���\����
	MV1_COLL_RESULT_POLY* floorpoly[2048];	// ���p�̃|���S���\����
	HITRESULT_LINE lineresult;				// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
	void StageHit();						// �s���֐�

	// ���[�t�A�V�F�C�v�A�܂�\��Ȃǂ̓����Ɋւ�邱��
	float facecount;		// �ڂ�ς��̃J�E���g
	int facewaitcount;		// �ʏ�̊�̂��ێ�����J�E���g
	bool faceup;			// �\��̊J��
	bool facedown;			// �[���̕�
	void FaceShape();		// �s���֐�

	// �R���g���[���[
	int inputx;		// ��ɉ�]�Ɋւ��
	int inputy;		// ��Ɉړ��Ɋւ��

	// �V�F�[�_�[
	int charapshader;		// �s�N�Z���V�F�[�_
	int charavshader;		// ���_�V�F�[�_

	MV1_COLL_RESULT_POLY_DIM ShadowHitResDim;
	MV1_COLL_RESULT_POLY *ShadowHitRes;
	VERTEX3D ShadowVertex[3];
	VECTOR ShadowSlideVec;
	int PLAYER_SHADOW_HEIGHT;
	float PLAYER_SHADOW_SIZE;
	int shadowhandle;
	void ShadowFoot();

	// �G���f�B���O
	float lastendcount;		// �Ō�̃t�F�[�h�A�E�g
	bool lastendflag;		// �Ō�ɏI����

public:
	Character(int ModelHandle, int StageHandle, int stagenum, int playmodel);	// ���f���n���h���ƃX�e�[�W�n���h���ƃX�e�[�W�ԍ��������Ɏ�����R���X�g���N�^
	~Character();																// �f�X�g���N�^

	void Draw();		// �`��
	void Process();		// �v���Z�X

	void Player_PlayAnim(int attach);		// ���[�V�����ύX
	void Player_AnimProcess();				// ���[�V��������

	// �Q�b�^�[
	VECTOR GetXYZ();	// �L�����ʒu
	double GetSpeed();	// ���[�V�����̃X�s�[�h
	bool GetMove();		// �����Ă��邩�ǂ���
	// �Z�b�^�[
	void SetEnd(bool lastendflag);		// �G���f�B���O���I��邩�ǂ���
};