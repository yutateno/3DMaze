#include "Character.h"

Character::Character(int ModelHandle, int StageHandle, int stagenum, int playmodel)
{
	charapshader = LoadPixelShader("charashader\\Character_DepthShadow_PS.pso");
	charavshader = LoadVertexShader("charashader\\Character_DepthShadow_VS.vso");

	this->collstagehandle = StageHandle;
	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo(collstagehandle, -1);
	// �X�e�[�W�̃��f���̍��W���X�V����
	MV1SetPosition(collstagehandle, VGet(0, 0, 0));
	// ���f���n���h�����������f���Ɋ܂܂��t���[���̕\����Ԃ�ύX����
	MV1SetFrameVisible(collstagehandle, -1, FALSE);
	// �R���W���������X�V����
	MV1RefreshCollInfo(collstagehandle, -1);

	// ���ɓǂݍ��ރ��f���̕������Z���[�h�����A���^�C���������Z�ɂ���
	//if (playmodel != 1)		// �V�L�~
	//{
	//}
	//else					// �A�J�l
	//{
	//}

	// �R�c���f���̓ǂݍ���
	this->charamodelhandle = ModelHandle;

	// �֊s�����ׂ�����
	materialnum = MV1GetMaterialNum(this->charamodelhandle);

	for (int i = 0; i != materialnum; ++i)
	{
		// �}�e���A���̌��X�̗֊s���̑������擾  
		dotwidth = (double)MV1GetMaterialOutLineDotWidth(this->charamodelhandle, i);
		// �}�e���A���̗֊s���̑�����5���̂P�ɂ���  
		MV1SetMaterialOutLineDotWidth(this->charamodelhandle, i, (float)(dotwidth / 5.0));
	}

	// �R�c���f����0�Ԗڂ̃A�j���[�V�������A�^�b�`����
	attachnum = 0;
	attachmotion = MV1AttachAnim(this->charamodelhandle, attachnum, -1, FALSE);

	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	totaltime = MV1GetAttachAnimTotalTime(this->charamodelhandle, attachmotion);

	// �t���C�������擾
	framenum = MV1GetFrameNum(this->charamodelhandle);

	modelheight = 20.0f;
	modelwigth = 5.0f;
	PLAYER_SHADOW_HEIGHT = 1;
	PLAYER_SHADOW_SIZE = 7.5f;

	// �X�e�[�W�ɂ���ăX�^�[�g�ʒu���w��
	switch (stagenum)
	{
	case 0:
		area = VGet(0, 0, 0);
		angle = 0;
		break;
	case 1:
		area = VGet(81, 0, -181);
		angle = 3.0f;
		break;
	case 2:
		area = VGet(142, 0, -71);
		angle = 0;
		break;
	case 3: // (2.5)
		area = VGet(-55, 38, 229);
		angle = -1.5f;
		break;
	case 4: // (3)
		area = VGet(227, 0, 354);
		angle = 0;
		break;
	case 5: // (End)
		area = VGet(-35.0f, 0.0f, -300.0f);
		modelheight = 20.0f;
		modelwigth = 5.0f;
		angle = 0;
		break;
	case 9: // bonus
		area = VGet(-25.0f, 0.0f, 810.0f);
		angle = 0;
		break;
	default:
		break;
	}

	shadowhandle = LoadGraph("media\\character\\Shadow.tga");

	this->stagenum = stagenum;

	// ���ꂼ��̑��x
	speed = DX_PI_F / 90;
	walkspeed = 0.6;
	animspeed = 0.3;

	// ���[�V�����֘A
	nowplaytime = 0.0;
	motionblendtime = 0.0;
	preattach = -1;
	premotionplaytime = 0.0;

	// �����蔻��Ɋ֘A
	wallnum = 0;
	hitflag = false;
	moveflag = false;
	floornum = 0;
	maxY = 0.0;

	// �V�F�C�v�֘A
	facewaitcount = 0;
	facecount = 0.0f;
	faceup = false;
	facedown = false;
	this->playmodel = playmodel;
	
	// �R���g���[���[�֘A
	inputx = 0;
	inputy = 0;

	// �G���f�B���O
	lastendcount = 1;
	lastendflag = false;
}

Character::~Character()
{
	MV1DetachAnim(charamodelhandle, preattach);
	MV1DetachAnim(charamodelhandle, attachmotion);
	MV1DeleteModel(charamodelhandle);
	MV1DeleteModel(collstagehandle);

	DeleteGraph(shadowhandle);

	DeleteShader(charapshader);
	DeleteShader(charavshader);

	InitGraph();
	InitShader();
}

void Character::Process()
{
	GetJoypadAnalogInput(&inputx, &inputy, DX_INPUT_KEY_PAD1);		// �R���g���[���[���擾

	// �X�e�B�b�N�̉�����ňړ����x�𒲐�
	if (inputy <= 0 && inputy >= -200)
	{
		walkspeed = 0.14;
		animspeed = 0.06;
	}
	else if ((inputy > 200 && inputy <= 400)
		|| (inputy < -200 && inputy >= -400))
	{
		walkspeed = 0.28;
		animspeed = 0.12;
	}
	else if ((inputy > 400 && inputy <= 600)
		|| (inputy < -400 && inputy >= -600))
	{
		walkspeed = 0.42;
		animspeed = 0.18;
	}
	else if ((inputy > 600 && inputy <= 800)
		|| (inputy < -600 && inputy >= -800))
	{
		walkspeed = 0.56;
		animspeed = 0.24;
	}
	else if ((inputy > 800 && inputy <= 1000)
		|| (inputy < -800 && inputy >= -1000))
	{
		walkspeed = 0.7;
		animspeed = 0.3;
	}
	if (inputy > 0)
	{
		walkspeed = 0.42;
		animspeed = 0.18;
	}

	// W�L�[�܂��͍��X�e�B�b�N���O�ɉ����ꂽ��O�i����
	if (KeyData::Get(KEY_INPUT_W) > 0
		|| inputy < 0)
	{
		area.x += (float)(sin(angle) * -walkspeed);
		area.z += (float)(cos(angle) * -walkspeed);
		moveflag = true;
		Player_PlayAnim(1);
	}

	// S�L�[�܂��͍��X�e�B�b�N�����ɉ����ꂽ���ނ���
	if (KeyData::Get(KEY_INPUT_S) > 0
		|| inputy > 0)
	{
		area.x += (float)(sin(angle) * walkspeed);
		area.z += (float)(cos(angle) * walkspeed);
		moveflag = true;
		Player_PlayAnim(2);
	}

	// A�L�[�܂��͍��X�e�B�b�N�����ɉ����ꂽ�獶��]����
	if (KeyData::Get(KEY_INPUT_A) > 0
		|| inputx < 0)
	{
		angle -= (float)speed;
	}

	// D�L�[�܂��͍��X�e�B�b�N���E�ɉ����ꂽ��E��]����
	if (KeyData::Get(KEY_INPUT_D) > 0
		|| inputx > 0)
	{
		angle += (float)speed;
	}

	// W�L�[��S�L�[���������ꂽ�烂�[�V�����𗧂��ɂ���i�L�[�{�[�h�̎���inputy�����Ȃ��ƃ��[�V������������
	if (KeyData::Get(KEY_INPUT_W) == -1
		|| KeyData::Get(KEY_INPUT_S) == -1
		|| inputy == 0)
	{
		Player_PlayAnim(0);
	}

	if (lastendflag == true)
	{
		if (lastendcount >= 0)
		{
			lastendcount -= 0.01f;
		}
	}

	// ���[�V�����̎���
	Player_AnimProcess();

	// �������̉�]�p�x���Z�b�g
	MV1SetRotationXYZ(charamodelhandle, VGet(0, angle, 0));
	// �w��ʒu�Ƀ��f����z�u
	MV1SetPosition(charamodelhandle, area);

	// �����蔻��̎���
	StageHit();

	// �V�F�C�v�̎���
	FaceShape();
}

void Character::ShadowFoot()
{
	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
	ShadowHitResDim = MV1CollCheck_Capsule(collstagehandle, -1, area, VAdd(area, VGet(0.0f, (float)PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

	// ���_�f�[�^�ŕω��������������Z�b�g
	ShadowVertex[0].dif = GetColorU8(255, 255, 255, 255);
	ShadowVertex[0].spc = GetColorU8(0, 0, 0, 0);
	ShadowVertex[0].su = 0.0f;
	ShadowVertex[0].sv = 0.0f;
	ShadowVertex[1] = ShadowVertex[0];
	ShadowVertex[2] = ShadowVertex[0];

	// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
	ShadowHitRes = ShadowHitResDim.Dim;
	for (int i = 0; i != ShadowHitResDim.HitNum; ++i, ++ShadowHitRes)
	{
		// �|���S���̍��W�͒n�ʃ|���S���̍��W
		ShadowVertex[0].pos = ShadowHitRes->Position[0];
		ShadowVertex[1].pos = ShadowHitRes->Position[1];
		ShadowVertex[2].pos = ShadowHitRes->Position[2];

		// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
		ShadowSlideVec = VScale(ShadowHitRes->Normal, 0.5f);
		ShadowVertex[0].pos = VAdd(ShadowVertex[0].pos, ShadowSlideVec);
		ShadowVertex[1].pos = VAdd(ShadowVertex[1].pos, ShadowSlideVec);
		ShadowVertex[2].pos = VAdd(ShadowVertex[2].pos, ShadowSlideVec);

		// �|���S���̕s�����x��ݒ肷��
		ShadowVertex[0].dif.a = 0;
		ShadowVertex[1].dif.a = 0;
		ShadowVertex[2].dif.a = 0;
		if (ShadowHitRes->Position[0].y > area.y - PLAYER_SHADOW_HEIGHT)
		{
			ShadowVertex[0].dif.a = (BYTE)(128 * (1.0f - fabs(ShadowHitRes->Position[0].y - area.y) / PLAYER_SHADOW_HEIGHT));
		}

		if (ShadowHitRes->Position[1].y > area.y - PLAYER_SHADOW_HEIGHT)
		{
			ShadowVertex[1].dif.a = (BYTE)(128 * (1.0f - fabs(ShadowHitRes->Position[1].y - area.y) / PLAYER_SHADOW_HEIGHT));
		}

		if (ShadowHitRes->Position[2].y > area.y - PLAYER_SHADOW_HEIGHT)
		{
			ShadowVertex[2].dif.a = (BYTE)(128 * (1.0f - fabs(ShadowHitRes->Position[2].y - area.y) / PLAYER_SHADOW_HEIGHT));
		}

		// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
		ShadowVertex[0].u = (ShadowHitRes->Position[0].x - area.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[0].v = (ShadowHitRes->Position[0].z - area.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[1].u = (ShadowHitRes->Position[1].x - area.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[1].v = (ShadowHitRes->Position[1].z - area.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[2].u = (ShadowHitRes->Position[2].x - area.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[2].v = (ShadowHitRes->Position[2].z - area.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

		// �e�|���S����`��
		DrawPolygon3D(ShadowVertex, 1, shadowhandle, TRUE);
	}

	// ���o�����n�ʃ|���S�����̌�n��
	MV1CollResultPolyDimTerminate(ShadowHitResDim);

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);
}

void Character::Draw()
{
	// �v���C���[���f���̕`��
	if (playmodel != 9)
	{
		if (stagenum == 5)
		{
			MV1DrawModel(charamodelhandle);
		}
		else
		{
			MV1SetUseOrigShader(TRUE);
			// �g�p���钸�_�V�F�[�_�[���Z�b�g
			SetUseVertexShader(charavshader);
			// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
			SetUsePixelShader(charapshader);
			MV1DrawModel(charamodelhandle);
			MV1SetUseOrigShader(FALSE);
			// �ݒ肵���萔������
			ResetVSConstF(43, 8);
			// �e
			ShadowFoot();
		}
	}
	else
	{
		MV1DrawModel(charamodelhandle);
	}

	if (lastendflag == true)
	{
		for (int i = 0; i != framenum; ++i)
		{
			MV1SetFrameOpacityRate(charamodelhandle, i, lastendcount);
		}
	}

	//// �f�o�b�O----------------------------------------------------------------
	/*DrawFormatString(50, 0, GetColor(255, 255, 255), "%f", area.x);
	DrawFormatString(50, 25, GetColor(255, 255, 255), "%f", area.y);
	DrawFormatString(50, 50, GetColor(255, 255, 255), "%f", area.z);*/
	//DrawFormatString(50, 100, GetColor(255, 255, 255), "angle:%f", angle);
	//DrawFormatString(50, 125, GetColor(255, 255, 255), "YukaNum:%d", YukaNum);
	//DrawFormatString(50, 150, GetColor(255, 255, 255), "KabeNum:%d", KabeNum);
	//// ------------------------------------------------------------------------
}

void Character::Player_PlayAnim(int attach)
{
	// ���̃��[�V�������Ⴄ���̂�������
	if (attachnum != attach)
	{
		// ���O�̃��[�V�������L����������f�^�b�`����
		if (preattach != -1 && motionblendtime < 1.0)
		{
			MV1DetachAnim(charamodelhandle, preattach);
			preattach = -1;
		}
		// ���̃��[�V����������
		preattach = attachmotion;
		premotionplaytime = nowplaytime;

		// ���������̃��[�V�����ԍ��ɓ����
		attachnum = attach;

		// �V���Ɏw��̃��[�V���������f���ɃA�^�b�`����
		attachmotion = MV1AttachAnim(charamodelhandle, attachnum, -1, false);

		// ���쎞�Ԃ�����������
		nowplaytime = 0.0;

		// �u�����h���͒��O�̃��[�V�������L���ł͂Ȃ��ꍇ�͂P�D�O��( �Đ����̃��[�V�����P���P�O�O���̏�� )�ɂ���
		if (premotionplaytime == -1)
		{
			motionblendtime = 1.0;
		}
		else
		{
			motionblendtime = 0.0;
		}
	}
}


void Character::Player_AnimProcess()
{
	// �u�����h�����P�ȉ��̏ꍇ�͂P�ɋ߂Â���
	if (motionblendtime < 1.0)
	{
		motionblendtime += 0.1;
		if (motionblendtime >= 1.0)
		{
			motionblendtime = 1.0;
		}
	}

	// �Đ����Ă��錻�݂̃��[�V�����̏���
	if (attachmotion != -1)
	{
		// ���[�V�����̑����Ԃ��擾
		totaltime = MV1GetAttachAnimTotalTime(charamodelhandle, attachmotion);

		// �Đ����Ԃ�i�߂�
		nowplaytime += animspeed;


		// �Đ����Ԃ������Ԃɓ��B���Ă����烋�[�v������
		if (nowplaytime >= totaltime)
		{
			nowplaytime = fmod(nowplaytime, totaltime);
		}

		// �ύX�����Đ����Ԃ����f���ɔ��f������
		MV1SetAttachAnimTime(charamodelhandle, attachmotion, (float)nowplaytime);

		// �A�j���[�V�����P�̃��f���ɑ΂��锽�f�����Z�b�g
		MV1SetAttachAnimBlendRate(charamodelhandle, attachmotion, (float)motionblendtime);
	}

	// �Đ����Ă��钼�O�̃��[�V�����̏���
	if (preattach != -1)
	{
		// �A�j���[�V�����̑����Ԃ��擾
		totaltime = MV1GetAttachAnimTotalTime(charamodelhandle, preattach);

		// �Đ����Ԃ�i�߂�
		premotionplaytime += animspeed;

		// �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
		if (premotionplaytime > totaltime)
		{
			premotionplaytime = fmod(premotionplaytime, totaltime);
		}

		// �ύX�����Đ����Ԃ����f���ɔ��f������
		MV1SetAttachAnimTime(charamodelhandle, preattach, (float)premotionplaytime);

		// �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
		MV1SetAttachAnimBlendRate(charamodelhandle, preattach, (float)(1.0 - motionblendtime));
	}
}

void Character::StageHit()
{
	// �v���C���[�̎��͂ɂ���X�e�[�W�|���S�����擾����
	hitdim = MV1CollCheck_Capsule(collstagehandle, -1, area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), 2000.f + VSize(area));

	// �ǃ|���S���Ə��|���S���̐�������������
	wallnum = 0;
	floornum = 0;

	// ���o���ꂽ�|���S���̐������J��Ԃ�
	for (int i = 0; i != hitdim.HitNum; ++i)
	{
		// �w�y���ʂɐ������ǂ����̓|���S���̖@���̂x�������O�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
		if (hitdim.Dim[i].Normal.y < 0.001f && hitdim.Dim[i].Normal.y > -0.001f)
		{
			// �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�v���C���[�̂x���W�{�P�D�O����荂���|���S���̂ݓ����蔻����s��
			if (hitdim.Dim[i].Position[0].y > area.y + 1.0f
				|| hitdim.Dim[i].Position[1].y > area.y + 1.0f
				|| hitdim.Dim[i].Position[2].y > area.y + 1.0f)
			{
				// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
				if (wallnum < 2048)
				{
					// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
					wallpoly[wallnum] = &hitdim.Dim[i];

					// �ǃ|���S���̐������Z����
					wallnum++;
				}
			}
		}
		else
		{
			// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
			if (floornum < 2048)
			{
				// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
				floorpoly[floornum] = &hitdim.Dim[i];

				// ���|���S���̐������Z����
				floornum++;
			}
		}
	}

	// �ǃ|���S���Ƃ̓����蔻�菈��
	if (wallnum != 0)
	{
		// �ǂɓ����������ǂ����̃t���O�͏�����Ԃł́u�������Ă��Ȃ��v�ɂ��Ă���
		hitflag = 0;

		// �ړ��������ǂ����ŏ����𕪊�
		if (moveflag == 1)
		{
			// �ǃ|���S���̐������J��Ԃ�
			for (int i = 0; i != wallnum; ++i)
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				poly = wallpoly[i];

				// �|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
				if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE)
				{
					continue;
				}

				// �����ɂ�����|���S���ƃv���C���[���������Ă���Ƃ������ƂȂ̂ŁA�|���S���ɓ��������t���O�𗧂Ă�
				hitflag = 1;

				// �V���Ȉړ����W�ŕǃ|���S���Ɠ������Ă��Ȃ����ǂ����𔻒肷��
				for (int j = 0; j != wallnum; ++j)
				{
					// j�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
					poly = wallpoly[j];

					// �������Ă����烋�[�v���甲����
					if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
					{
						break;
					}
				}
			}
		}
		else
		{
			// �ǃ|���S���̐������J��Ԃ�
			for (int i = 0; i != wallnum; ++i)
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				poly = wallpoly[i];

				// �|���S���ɓ������Ă����瓖�������t���O�𗧂Ă���Ń��[�v���甲����
				if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
				{
					hitflag = 1;
					break;
				}
			}
		}

		// �ǂɓ������Ă�����ǂ��牟���o���������s��
		if (hitflag == 1)
		{
			// �ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
			for (int k = 0; k != 32; ++k)
			{
				// �ǃ|���S���̐������J��Ԃ�
				for (int i = 0; i != wallnum; ++i)
				{
					// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
					poly = wallpoly[i];

					// �v���C���[�Ɠ������Ă��邩�𔻒�
					if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE)
					{
						continue;
					}

					// �������Ă�����K�苗�����v���C���[��ǂ̖@�������Ɉړ�������
					area = VAdd(area, VScale(poly->Normal, (float)walkspeed / 6));		// X��Z���W�������Ⴄ�Ȃ炱��ɂ��ĕЕ������Ȃ�ړ����x�Ɠ��������ɂ������Ȃ����O���W����Ă��ׂ�

					// �ړ�������ŕǃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
					for (int j = 0; j != wallnum; ++j)
					{
						// �������Ă����烋�[�v�𔲂���
						poly = wallpoly[j];
						if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
						{
							break;
						}
					}

					// i �� KabeNum �ł͂Ȃ��ꍇ�͑S���̃|���S���ŉ����o�������݂�O�ɑS�Ă̕ǃ|���S���ƐڐG���Ȃ��Ȃ����Ƃ������ƂȂ̂Ń��[�v���甲����
					break;
				}
			}
		}
	}

	// ���|���S���Ƃ̓����蔻��
	if (floornum != 0)
	{
		// ���|���S���ɓ����������ǂ����̃t���O��|���Ă���
		hitflag = 0;

		// ��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
		maxY = 0.0;

		// ���|���S���̐������J��Ԃ�
		for (int i = 0; i != floornum; ++i)
		{
			// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
			poly = floorpoly[i];

			// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
			lineresult = HitCheck_Line_Triangle(VAdd(area, VGet(0.0f, modelheight, 0.0f)), VAdd(area, VGet(0.0f, -1.0f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);

			// �������Ă��Ȃ������牽�����Ȃ�
			if (lineresult.HitFlag == FALSE)
			{
				continue;
			}

			// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
			if (hitflag == 1 && maxY > lineresult.Position.y)
			{
				continue;
			}
			// �ڐG�����x���W��ۑ�����
			maxY = lineresult.Position.y;

			// �|���S���ɓ��������t���O�𗧂Ă�
			hitflag = 1;
		}

			// ���|���S���ɓ����������ǂ����ŏ����𕪊�
		if (hitflag == 1)
		{
			// ���������ꍇ
			// �ڐG�����|���S���ň�ԍ����x���W���v���C���[�̂x���W�ɂ���
			area.y = maxY;
		}
	}

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	MV1CollResultPolyDimTerminate(hitdim);
}

void Character::FaceShape()
{
	// �ҋ@�t���[��������Ȃ�������
	if (facewaitcount <= 300)
	{
		++facewaitcount;
		// ������x���肽��
		if (facewaitcount == 295)
		{
			faceup = true;	// �t�F�C�X��������
		}
	}
	// �t�F�C�X��������
	if (faceup == true)
	{
		if (facecount <= 1)
		{
			facecount += 0.1f;
		}
		// �ő�܂ŒB������
		if (facecount >= 1)
		{
			faceup = false;
			facedown = true;	// ������
		}
	}
	// �t�F�C�X����߂�����
	if (facedown == true)
	{
		if (facecount >= 0)
		{
			facecount -= 0.1f;
		}
		// �Œ�܂ŒB������
		if (facecount <= 0)
		{
			faceup = false;
			facedown = false;
			facewaitcount = 0;
		}
	}
	
	if (playmodel == 0)
	{
		MV1SetShapeRate(charamodelhandle, MV1SearchShape(charamodelhandle, "��1"), facecount);
		MV1SetShapeRate(charamodelhandle, MV1SearchShape(charamodelhandle, "��2"), facecount);
	}
	else if (playmodel == 1)
	{
		MV1SetShapeRate(charamodelhandle, MV1SearchShape(charamodelhandle, "��1"), facecount);
		MV1SetShapeRate(charamodelhandle, MV1SearchShape(charamodelhandle, "�������܂�"), 1.0f);
	}
}

// �Q�b�^�[
VECTOR Character::GetXYZ()
{
	return area;
}

double Character::GetSpeed()
{
	return animspeed;
}

bool Character::GetMove()
{
	return { inputy != 0 };
}

void Character::SetEnd(bool lastendflag)
{
	this->lastendflag = lastendflag;
}