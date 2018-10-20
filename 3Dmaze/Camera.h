#pragma once
#include "DxLib.h"
#include "input.h"
#include <math.h>

class Camera
{
private:
	// ���ꂼ��̈ʒu�Ɋւ���
	VECTOR area;		// �J�����̈ʒu
	VECTOR view;		// �����������
	VECTOR chara;		// �L�����ʒu

	// �X�e�[�W�Ɋւ���
	int collstagehandle;	// �X�e�[�W�n���h��
	int stagenum;			// �X�e�[�W�̔ԍ�

	// �������̃J�����̃X�e�[�W�̂����蔻��Ɋւ���
	int nowcamera;							// ���̃J����
	int rightcamera;						// �E
	int leftcamera;							// ��
	int topcamera;							// �O�㔽��
	int lowerrightcamera;					// �E��
	int lowerleftcamera;					// ����
	MV1_COLL_RESULT_POLY_DIM nowdim;		// ���̃J�����̃R���W��������
	MV1_COLL_RESULT_POLY_DIM rightdim;		// �E
	MV1_COLL_RESULT_POLY_DIM leftdim;		// ��
	MV1_COLL_RESULT_POLY_DIM topdim;		// �O�㔽��
	MV1_COLL_RESULT_POLY_DIM lowerrightdim;	// �E��
	MV1_COLL_RESULT_POLY_DIM lowerleftdim;	// ����
	double hitturn;							// �l�����̃J�����̈ʒu����p
	double lowerturn;						// ��������lower�J�����̈ʒu����p
	double upperturn;						// ��������upper�J�����̈ʒu����p
	int moveframe;

	// �R���g���[���[
	DINPUT_JOYSTATE input;		// ��ɉ�]�Ɋւ��
	int inputright;
	int inputleft;

	// �J�����̃Y�[�����]�̓����Ɋւ���
	double speed;					// ��]�X�s�[�h
	int camerazoom;					// �ǂ̂��炢�Y�[���������ǂ���
	bool cameratype;				// 0:��]�^�� 1:�؂�ւ��^��
	bool rightleft;					// 0:�E��]�� 1:����]��
	bool moveflag;					// �J�������������ǂ���
	VECTOR RLrotate(double speed);	// ��]���s���֐�

public:
	Camera(VECTOR xyz, int stagenum, int StageHandle, bool cameratype);		// �L�����̈ʒu�ƃX�e�[�W�ԍ��ƃX�e�[�W�n���h���ƃJ�����^�C�v�������Ɏ�����R���X�g���N�^
	~Camera();																// �f�X�g���N�^

	void Process(VECTOR xyz);		// �L�����̈ʒu�������Ɏ�����v���Z�X

	// �Q�b�^�[
	VECTOR GetXYZ();				// �J�����̈ʒu
};