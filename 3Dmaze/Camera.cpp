#include "Camera.h"

Camera::Camera(VECTOR xyz, int stagenum, int StageHandle, bool cameratype)
{
	collstagehandle = StageHandle;

	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo(collstagehandle, -1);
	// �X�e�[�W�̃��f���̍��W���X�V����
	MV1SetPosition(collstagehandle, VGet(0, 0, 0));
	// ���f���n���h�����������f���Ɋ܂܂��t���[���̕\����Ԃ�ύX����
	MV1SetFrameVisible(collstagehandle, -1, FALSE);
	// �R���W���������X�V����
	MV1RefreshCollInfo(collstagehandle, -1);

	this->stagenum = stagenum;
	this->cameratype = cameratype;
	inputright = 0;
	inputleft = 0;

	view = VGet(0, 13, 0);
	camerazoom = 0;

	switch (stagenum)
	{
	case 0:
		area = VGet(0, 15, 32);
		break;
	case 1:
		area = VGet(0, 15, -35); 
		break;
	case 2:
		area = VGet(0, 15, 35);
		break;
	case 3: // (2.5)
		area = VGet(-17, 20, 0);
		camerazoom = 17;		// �K�i�J�n�Ȃ̂ŃY�[�������Ă����Ԃɂ��Ƃ�
		break;
	case 4:	// 3
		area = VGet(0, 15, 35);
		this->cameratype = 1;
		break;
	case 5: // (End)
		area = VGet(0, 15, 32);
		view = VGet(0, 13, 0);
		break;
	case 9: // bonus
		area = VGet(0, 15, 32);
		view = VGet(0, 17, 0);	// stagenum��9�̂Ƃ����������ʒu��ς���
		break;
	default:
		break;
	}

	chara = xyz;
	hitturn = DX_PI_F / 2;
	upperturn = DX_PI_F / 1.5;
	lowerturn = DX_PI_F / 4;
	moveflag = false;
	moveframe = 0;

	if (this->cameratype == 0)	// �؂�ւ��^
	{
		speed = DX_PI_F / 2;
	}
	else // ��]�^�i���܂��X�e�[�W�̉�]�X�s�[�h�͐������ɂ���Ɏw��j
	{
		speed = DX_PI_F / 90;
	}

	rightleft = false;	// �����l�͈���Ȃ��̂�false�ɂ��Ƃ�

	// �����܂Ŏc���Ƃ�
	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���(�����near�̑����Ŕ��Ƃ���������̒������������Z�u���b�V���𑝂₵��SetZBufferBitDepth( 24 ))
	//SetCameraNearFar(1.0f, 500.0f);

	//area + chara�̎��_����view + chara�̃^�[�Q�b�g������p�x�ɃJ������ݒu
	SetCameraPositionAndTarget_UpVecY(VAdd(area, chara), VAdd(view, chara));
}

Camera::~Camera()
{

}

void Camera::Process(VECTOR xyz)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);	// �R���g���[���[���擾
	if (input.Rx > 0)
	{
		inputright++;
	}
	if (input.Rx < 0)
	{
		inputleft++;
	}
	if (input.Rx == 0)
	{
		inputleft = 0;
		inputright = 0;
	}

	// �L�����̈ʒu���X�V��������
	chara = xyz;

	// �X�e�[�W�Ƃ̂��ꂼ��̃J�����̂����蔻��i8�����j
	nowdim = MV1CollCheck_Capsule(collstagehandle, -1, VAdd(area, chara), VAdd(view, chara), 0.1f);

	rightdim = MV1CollCheck_Capsule(collstagehandle, -1, VAdd(VGet((float)(area.x * cos(-hitturn) + area.z * sin(-hitturn))
		, area.y, (float)(-area.x * sin(-hitturn) + area.z * cos(-hitturn)))
		, chara), VAdd(view, chara), 0.1f);

	leftdim = MV1CollCheck_Capsule(collstagehandle, -1, VAdd(VGet((float)(area.x * cos(hitturn) + area.z * sin(hitturn))
		, area.y, (float)(-area.x * sin(hitturn) + area.z * cos(hitturn)))
		, chara), VAdd(view, chara), 0.1f);

	topdim = MV1CollCheck_Capsule(collstagehandle, -1, VAdd(VGet(area.x * -1, area.y, area.z * -1), chara), VAdd(view, chara), 0.1f);

	lowerrightdim = MV1CollCheck_Capsule(collstagehandle, -1, VAdd(VGet((float)(area.x * cos(-lowerturn) + area.z * sin(-lowerturn))
		, area.y, (float)(-area.x * sin(-lowerturn) + area.z * cos(-lowerturn)))
		, chara), VAdd(view, chara), 0.1f);

	lowerleftdim = MV1CollCheck_Capsule(collstagehandle, -1, VAdd(VGet((float)(area.x * cos(lowerturn) + area.z * sin(lowerturn))
		, area.y, (float)(-area.x * sin(lowerturn) + area.z * cos(lowerturn)))
		, chara), VAdd(view, chara), 0.1f);

	nowcamera = nowdim.HitNum;
	rightcamera = rightdim.HitNum;
	leftcamera = leftdim.HitNum;
	topcamera = topdim.HitNum;
	lowerrightcamera = lowerrightdim.HitNum;
	lowerleftcamera = lowerleftdim.HitNum;
	// �����܂ŃJ�����ƃX�e�[�W�̂����蔻��

	// �X�e�[�W�ԍ��X�i���܂��X�e�[�W�j����Ȃ��Ƃ�
	if (stagenum != 9 && stagenum != 4)
	{
		switch (stagenum)
		{
		case 0:
			SetCameraNearFar(0.5f, 300.0f);
			break;
		case 1:
			SetCameraNearFar(0.5f, 350.0f);
			break;
		case 2:
			SetCameraNearFar(0.5f, 350.0f);
			break;
		case 3:
			SetCameraNearFar(0.5f, 500.0f);
			break;
		case 4:
			SetCameraNearFar(0.5f, 500.0f);
			break;
		case 5:
			SetCameraNearFar(0.5f, 1800.0f);
			break;
		default:
			SetCameraNearFar(0.5f, 500.0f);
			break;
		}
		// �؂�ւ��^
		if (cameratype == 0)
		{
			// �O��ύX topcamera�����܂��ĂȂ���������s
			if ((KeyData::Get(KEY_INPUT_RCONTROL) == 1
				|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
				&& topcamera == 0)
			{
				area = VGet(area.x*-1, area.y, area.z*-1);
			}
			// ���ɉ�]�� leftcamera�����܂��ĂȂ���������s
			if ((KeyData::Get(KEY_INPUT_LEFT) == 1
				|| inputleft == 1)
				&& leftcamera == 0)
			{
				area = RLrotate(speed);
			}
			// �E�ɉ�]�� rightcamera�����܂��ĂȂ���������s
			if ((KeyData::Get(KEY_INPUT_RIGHT) == 1
				|| inputright == 1)
				&& rightcamera == 0)
			{
				area = RLrotate(-speed);
			}

			// ���̃J���������܂��Ă�����
			if (nowcamera != 0)
			{
				// topcamera�����܂��ĂȂ�������
				if (topcamera == 0)
				{
					// �X�e�[�W�ԍ��R�i�N���A�^�f�G���A�j����Ȃ�������
					if (stagenum != 3)
					{
						// �L�����N�^�[���K�i������Ă��Ȃ�������i�O�ł�����ł����ꉞ0.5f�ɂ��Ă��܂��j
						if (chara.y <= 0.5f)
						{
							area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// �Y�[���C��������������
							camerazoom++;
						}
						else
						{
							area = VGet(area.x*-1, area.y, area.z*-1);	// �J������O�㔽�΂ɂ���
						}
					}
					else
					{
						area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// �Y�[���C��������������
						camerazoom++;
					}
				}
				else
				{
					// �����J���������܂��Ă��Ȃ����E���J���������܂��Ă���Ƃ�
					if (lowerleftcamera == 0 && lowerrightcamera != 0)
					{
						area = RLrotate(speed);	// ����]������
					}
					// �����J���������܂��Ă��邪�E���J���������܂��Ă��Ȃ�������
					else if (lowerleftcamera != 0 && lowerrightcamera == 0)
					{
						area = RLrotate(-speed);	// �E��]������
					}
					// �����J�����ƉE���J���������܂��Ă�����
					else if (lowerleftcamera != 0 && lowerrightcamera != 0)
					{
						area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// �Y�[���C��������������
						camerazoom++;
					}
				}
			}
			// �J�������Y�[������Ă����Ԃō������E�������܂��Ă��Ȃ�������
			if (camerazoom != 0 && (lowerleftcamera == 0 || lowerrightcamera == 0))
			{
				area = VAdd(area, VAdd(VGet(0, -0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), 1)));	// �Y�[���A�E�g������������
				camerazoom--;
			}
		}
		// ��]�^
		else
		{
			// �O��ύX topcamera�����܂��Ă��Ȃ������炳����
			if ((KeyData::Get(KEY_INPUT_RCONTROL) == 1
				|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
				&& topcamera == 0 && moveflag == false)
			{
				area = VGet(area.x*-1, area.y, area.z*-1);
			}
			// ���ɉ�]�� leftcamera�����܂��Ă��Ȃ������珈��������
			if ((KeyData::Get(KEY_INPUT_LEFT) == 1
				|| inputleft == 1)
				&& leftcamera == 0 && moveflag == false)
			{
				rightleft = 1;
				moveflag = true;
			}
			// �E�ɉ�]�� rightcamera�����܂��Ă��Ȃ������珈��������
			if ((KeyData::Get(KEY_INPUT_RIGHT) == 1
				|| inputright == 1)
				&& rightcamera == 0 && moveflag == false)
			{
				rightleft = 0;
				moveflag = true;
			}
			// ���̃J���������܂��Ă�����
			if (nowcamera != 0 && moveflag == false)
			{
				// topcamera�����܂��Ă��Ȃ�������
				if (topcamera == 0)
				{
					// �X�e�[�W�ԍ��R�i�N���A�^�f�G���A�j����Ȃ�������
					if (stagenum != 3)
					{
						// �L�����N�^�[���K�i������Ă��Ȃ�������
						if (chara.y <= 0.5f)
						{
							area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// �Y�[���C��������������
							camerazoom++;
						}
						else
						{
							area = VGet(area.x*-1, area.y, area.z*-1);	// �J������O�㔽�΂ɂ���
						}
					}
					else
					{
						area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// �Y�[���C��������������
						camerazoom++;
					}
				}
				else
				{
					moveflag = true;	// �J��������]���Ă�Œ��ɂ���

					// ���������܂��Ă��Ȃ��E�������܂��Ă���Ƃ�
					if (lowerleftcamera == 0 && lowerrightcamera != 0)
					{
						rightleft = 1;	// ����]�Ƃ��ď���������
					}
					// ���������܂��ĂĂ��ĉE�������܂��Ă��Ȃ��Ƃ�
					else if (lowerleftcamera != 0 && lowerrightcamera == 0)
					{
						rightleft = 0;	// �E��]�Ƃ��ď�������
					}
					// ���������܂��Ă��ĉE�������܂��Ă�Ƃ�
					else if (lowerleftcamera != 0 && lowerrightcamera != 0)
					{
						area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// �Y�[���C������������
						camerazoom++;
					}
				}
			}
			// �E��]
			if (rightleft == 0)
			{
				speed = -(DX_PI_F / 90);	// ��]���������߂�
			}
			else // ����]
			{
				speed = DX_PI_F / 90;		// ��]���������߂�
			}
			// // 45�t���[����]������(������)
			if (moveflag == true)
			{
				if (moveframe != 45)
				{
					moveframe++;
					area = RLrotate(speed);
					if (moveframe == 45)
					{
						moveflag = false;
						moveframe = 0;
					}
				}
			}
			// �Y�[�����Ă��č������E�������܂��Ă��Ȃ�������
			if (camerazoom != 0 && (lowerleftcamera == 0 || lowerrightcamera == 0))
			{
				area = VAdd(area, VAdd(VGet(0, -0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), 1)));	// �Y�[���A�E�g����������
				camerazoom--;
			}
		}
	}
	else if (stagenum == 4)
	{
		SetCameraNearFar(0.5f, 1500.0f);	// �J�����̕`��͈͂��w��
											// ���ɉ�]��
		if ((KeyData::Get(KEY_INPUT_LEFT) == 1
			|| input.Rx < 0))
		{
			area = RLrotate(speed);	// ��]����
		}
		// �E�ɉ�]��
		if ((KeyData::Get(KEY_INPUT_RIGHT) == 1
			|| input.Rx > 0))
		{
			area = RLrotate(-speed);	// ��]����
		}
		// ��L�[��������Ă�����
		if (KeyData::Get(KEY_INPUT_UP) > 0
			|| input.Ry < 0)
		{
			// ����
			if (area.y > 10)
			{
				area = VAdd(area, VScale(VNorm(area), -1));
			}
		}

		// ���L�[��������Ă�����
		if (KeyData::Get(KEY_INPUT_DOWN) > 0
			|| input.Ry > 0)
		{
			// ����
			if (area.y < 20)
			{
				area = VAdd(area, VScale(VNorm(area), 1));
			}
		}
	}
	else if(stagenum == 9)  // �X�e�[�W�ԍ��X�i���܂��X�e�[�W�j��������
	{
		SetCameraNearFar(1.0f, 3000.0f);	// �J�����̕`��͈͂��w��
		// ���ɉ�]��
		if ((KeyData::Get(KEY_INPUT_LEFT) == 1
			|| input.Rx < 0))
		{
			 area = RLrotate(speed);	// ��]����
		}
		// �E�ɉ�]��
		if ((KeyData::Get(KEY_INPUT_RIGHT) == 1
			|| input.Rx > 0))
		{
			area = RLrotate(-speed);	// ��]����
		}
		// ��L�[��������Ă�����
		if (KeyData::Get(KEY_INPUT_UP) > 0
			|| input.Ry < 0)
		{
			// ����
			if (area.y > 10)
			{
				area = VAdd(area, VScale(VNorm(area), -1));
			}
		}

		// ���L�[��������Ă�����
		if (KeyData::Get(KEY_INPUT_DOWN) > 0
			|| input.Ry > 0)
		{
			// ����
			if (area.y < 30)
			{
				area = VAdd(area, VScale(VNorm(area), 1));
			}
		}
	}

	// �������̎��_����������̃^�[�Q�b�g������p�x�ɃJ������ݒu
	SetCameraPositionAndTarget_UpVecY(VAdd(area, chara), VAdd(view, chara));

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	MV1CollResultPolyDimTerminate(nowdim);
	MV1CollResultPolyDimTerminate(rightdim);
	MV1CollResultPolyDimTerminate(leftdim);
	MV1CollResultPolyDimTerminate(topdim);
	MV1CollResultPolyDimTerminate(lowerrightdim);
	MV1CollResultPolyDimTerminate(lowerleftdim);

	//// �f�o�b�O-----------------------------------------------------------------------------------
	//DrawFormatString(50, 300, GetColor(255, 255, 255), "cameraspeed:%f", speed);
	//// -------------------------------------------------------------------------------------------
}

// ang�p��]����
VECTOR Camera::RLrotate(double speed)
{
	VECTOR oxyz;
	oxyz = VGet(area.x, area.y, area.z);

	return VGet((float)(oxyz.x * cos(speed) + oxyz.z * sin(speed))
		, oxyz.y, (float)(-oxyz.x * sin(speed) + oxyz.z * cos(speed)));
}

// �Q�b�^�[
VECTOR Camera::GetXYZ()
{
	return area;
}