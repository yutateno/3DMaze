#include "Game.h"

// ���f���n���h���Ɣ���p�X�e�[�W�n���h���ƕ`��p�X�e�[�W�n���h����BGM�ƃX�e�[�W�ԍ��ƃJ�����^�C�v�������Ɏ��R���X�g���N�^
Game::Game(int ModelHandle, int CStageHandle, int DStageHandle, int bgm, int stagenum, bool cameratype, int playmodel)
{
	// �X�e�[�W���ƂɃ��[�h���قȂ����
	switch (stagenum)
	{
	case 0:
		gr_black = LoadGraph("media\\black.png");
		break;

	case 1:
		gr_black = LoadGraph("media\\black.png");
		bgm_sub = LoadSoundMem("sound\\���C����.wav");
		break;

	case 2:
		gr_black = LoadGraph("media\\black.png");
		bgm_bloodwindow = LoadSoundMem("sound\\SEMARU.wav");
		break;

	case 3:
		gr_black = LoadGraph("media\\black.png");
		gr_white = LoadGraph("media\\white.png");
		break;

	case 4:
		gr_black = LoadGraph("media\\black.png");
		gr_white = LoadGraph("media\\white.png");
		break;

	case 5:
		gr_black = LoadGraph("media\\black.png");
		gr_lastsign = LoadGraph("media\\lastsign.png");
		gr_white = LoadGraph("media\\white.png");
		gr_end = LoadGraph("media\\end.png");
		// �e����-----------------------------------------------------------------------------------------------------------
		// �쐬����摜�̃t�H�[�}�b�g��s�������_�^�łP�`�����l���A�P�U�r�b�g�ɂ���
		SetDrawValidFloatTypeGraphCreateFlag(TRUE);
		SetCreateDrawValidGraphChannelNum(1);
		SetCreateGraphColorBitDepth(16);

		// �e�p�[�x�L�^�摜�̍쐬
		DepthBufferGraphHandle = MakeScreen(8192, 8192, FALSE);	// �����̐��l��傫������Ƒ�G�c�ɂȂ��ď���������ƑN���ɂȂ�

																// �ݒ�����ɖ߂�
		SetDrawValidFloatTypeGraphCreateFlag(FALSE);
		SetCreateDrawValidGraphChannelNum(4);
		SetCreateGraphColorBitDepth(32);

		// �[�x�L�^�摜�`��p�̒��_�V�F�[�_�[��ǂݍ���
		Skin4_DepthShadow_Step1_VertexShader = LoadVertexShader("shadowshader\\SkinMesh4_DepthShadow_Step1VS.vso");
		Normal_DepthShadow_Step1_VertexShader = LoadVertexShader("shadowshader\\NormalMesh_DepthShadow_Step1VS.vso");

		// �[�x�L�^�摜�`��p�̃s�N�Z���V�F�[�_�[��ǂݍ���
		DepthShadow_Step1_PixelShader = LoadPixelShader("shadowshader\\DepthShadow_Step1PS.pso");

		// �[�x�L�^�摜���g�����f�B���N�V���i�����C�g��̕`��p���_�V�F�[�_�[��ǂݍ���
		Skin4_DirLight_DepthShadow_Step2_VertexShader = LoadVertexShader("shadowshader\\SkinMesh4_DirLight_DepthShadow_Step2VS.vso");
		Normal_DirLight_DepthShadow_Step2_VertexShader = LoadVertexShader("shadowshader\\NormalMesh_DirLight_DepthShadow_Step2VS.vso");

		// �[�x�L�^�摜���g�����f�B���N�V���i�����C�g��̕`��p�s�N�Z���V�F�[�_�[��ǂݍ���
		DirLight_DepthShadow_Step2_PixelShader = LoadPixelShader("shadowshader\\DirLight_DepthShadow_Step2PS.pso");

		// �e����-----------------------------------------------------------------------------------------------------------
		break;

	default:
		break;
	}
	
	// ����
	character = new Character(ModelHandle, CStageHandle, stagenum, playmodel);
	camera = new Camera(character->GetXYZ(), stagenum, CStageHandle, cameratype);
	stage = new Stage(DStageHandle);
	// �X�e�[�W
	this->stagenum = stagenum;

	// �G���f�B���O
	lastendflag = false;
	lastendcount = 0;
	character->SetEnd(lastendflag);

	// BGM
	this->bgm = bgm;

	// ���C�g������
	switch (stagenum)
	{
	case 0:		// �`���[�g���A��
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-153, 132, -244), 500, 1, 0.00025f, 0);
		break;
	case 1:		// �P�X�e�[�W
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-369, 267, -328), 1000, 1, 0.00025f, 0);
		break;
	case 2:		// �Q�X�e�[�W
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-308.071f, 222.496f, 308.071f), 900, 1, 0.00025f, 0);
		break;
	case 3:		// �Q�D�T�X�e�[�W
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		ChangeLightTypePoint(VGet(0.000f, 20.000f, -230.240f), 564, 0.5f, 0.0001f, 0.000005f);
		break;
	case 4:		// 3�X�e�[�W
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-356.556f, 435.791f, -356.556f), 1325.381f, 1.000f, 0.000f, 0.000f);
		break;
	case 5:		// end�X�e�[�W
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypeSpot(VGet(-891.650f, 534.990f, -1069.980f), VGet(0.509f, -0.402f, 0.761f), 2.000f, 2.000f, 2529.044f, 1, 0, 0);
		break;
	default:
		/*SetLightEnable(TRUE);		/// �����l�i��������܂ł���j
		SetLightDifColor(GetColorF(1, 1, 1, 1));
		SetLightSpcColor(GetColorF(1, 1, 1, 1));
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		ChangeLightTypeDir(VGet(0.577350f, 0.577350f, 0.577350f));
		SetLightPosition(VGet(0, 0, 0));
		SetLightRangeAtten(2000.0f, 0.1f, 0, 0);
		SetLightAngle(2.0f, 2.0f);*/
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-600, 1100, -1000), 2030, 1, 0.00025f, 0);
		break;
	}

	// ��
	bgm_feedout = LoadSoundMem("sound\\Stairs.wav");

	// �T�E���h�֘A
	listen = camera->GetXYZ();
	sePos = character->GetXYZ();
	prelisten = listen;
	moveflag = character->GetMove();
	sestart = false;
	secount = 0;
	movespeed = (int)(character->GetSpeed() * 150);
	footLR = false;
	dooropenflag = false;
	feedsoundcount = 0;
	silencecount = 0;
	silenceflag = false;

	// 4.0f �łP���[�g���Ƃ���
	Set3DSoundOneMetre(4.0f);

	// 3D�����Ƃ��ēǂݍ���
	SetCreate3DSoundFlag(TRUE);
	se_left = LoadSoundMem("sound\\leftfoot.wav");
	se_right = LoadSoundMem("sound\\rightfoot.wav");
	SetCreate3DSoundFlag(FALSE);

	if (stagenum == 3 || stagenum == 4)
	{
		SetCreate3DSoundFlag(TRUE);
		se_door = LoadSoundMem("sound\\Door.wav");
		SetCreate3DSoundFlag(FALSE);
		Set3DPositionSoundMem(VGet(0, 0, -210), se_door);
		Set3DRadiusSoundMem(128.0f, se_door);
	}

	// �T�E���h�̍��W��ݒ�
	Set3DPositionSoundMem(sePos, se_left);
	Set3DPositionSoundMem(sePos, se_right);

	// �T�E���h���͂�( ������������ )�͈͂�ݒ�
	Set3DRadiusSoundMem(64.0f, se_right);
	Set3DRadiusSoundMem(64.0f, se_left);
}

Game::~Game()
{
	switch (stagenum)
	{
	case 0:
		DeleteGraph(gr_black);
		break;
	case 1:
		StopSoundMem(bgm_sub);
		DeleteGraph(gr_black);
		DeleteSoundMem(bgm_sub);
		break;
	case 2:
		StopSoundMem(bgm_bloodwindow);
		DeleteGraph(gr_black);
		DeleteSoundMem(bgm_bloodwindow);
		break;
	case 3:
		DeleteSoundMem(se_door);
		DeleteGraph(gr_black);
		DeleteGraph(gr_white);
		break;
	case 4:
		DeleteSoundMem(se_door);
		DeleteGraph(gr_white);
		DeleteGraph(gr_black);
		break;
	case 5:
		DeleteGraph(gr_white);
		DeleteGraph(gr_end);
		DeleteGraph(gr_black);
		DeleteGraph(gr_lastsign);
		DeleteGraph(DepthBufferGraphHandle);
		DeleteShader(Skin4_DepthShadow_Step1_VertexShader);
		DeleteShader(Normal_DepthShadow_Step1_VertexShader);
		DeleteShader(DepthShadow_Step1_PixelShader);
		DeleteShader(Skin4_DirLight_DepthShadow_Step2_VertexShader);
		DeleteShader(Normal_DirLight_DepthShadow_Step2_VertexShader);
		DeleteShader(DirLight_DepthShadow_Step2_PixelShader);
		break;
	default:
		break;
	}
	StopSoundMem(bgm);
	StopSoundMem(se_left);
	StopSoundMem(se_right);
	StopSoundMem(bgm_feedout);

	delete character;
	delete camera;
	delete stage;

	MV1DeleteModel(DStageHandle);
	MV1DeleteModel(CStageHandle);
	MV1DeleteModel(ModelHandle);

	DeleteSoundMem(bgm);
	DeleteSoundMem(se_left);
	DeleteSoundMem(se_right);
	DeleteSoundMem(bgm_feedout);

	InitGraph();
	InitSoundMem();
	InitShader();
}

void Game::Update()
{
	// �`��
	Draw();

	// �T�E���h
	Sound();

	// �X�e�[�W�ԍ��O�̃G���h�ʒu
	if (character->GetXYZ().y >= 25 && stagenum == 0)
	{
		end = true;
	}
	// �X�e�[�W�ԍ��P�̃G���h�ʒu
	if (character->GetXYZ().y >= 50 && stagenum == 1)
	{
		end = true;
	}
	// �X�e�[�W�ԍ��Q�̃G���h�ʒu�i�܂������e�X�g�p x��<=��>=�ɂ��āj
	if (((character->GetXYZ().x >= 159 && character->GetXYZ().x <= 161)
		&& (character->GetXYZ().z <= -159 && character->GetXYZ().z >= -174))
		&& stagenum == 2)
	{
		moveflag = false;
		sestart = false;
		end = true;
	}
	// �X�e�[�W�ԍ�2.5�̃G���h�ʒu
	if (character->GetXYZ().z <= -230 && stagenum == 3)
	{
		end = true;
	}
	// �X�e�[�W�ԍ��R�̃G���h�ʒu
	if (stagenum == 4
		&& character->GetXYZ().x <= 281 && character->GetXYZ().x >= 276
		&& character->GetXYZ().z <= -229 && character->GetXYZ().z >= -254)
	{
		end = true;
	}

	// �t�F�[�h�C��
	if (start == false)
	{
		count += 5;
		if (count == 255)
		{
			start = true;
		}
	}
	// �G���f�B���O
	if (PadData::Get(XINPUT_BUTTON_Y, 0) == 1 && stagenum == 5)
	{
		lastendflag = true;
		character->SetEnd(lastendflag);
	}
	if (lastendflag == true)
	{
		lastendcount++;
		if (lastendcount >= 200)
		{
			end = true;
		}
	}

	// �I���t���b�O����������
	if (end == true)
	{
		if (count >= 250)
		{
			PlaySoundMem(bgm_feedout, DX_PLAYTYPE_BACK, false);
		}
		moveflag = false;
		sestart = false;
		// �t�F�[�h�A�E�g������
		character->Player_AnimProcess();	// �L�����N�^�[�𗧂����[�V�����ɂ���
		character->Player_PlayAnim(0);
		count -= 3;
		if (count <= 0)
		{
			if (stagenum != 5)
			{
				Scene::scene = LOAD;			// �V�[�������[�h��ʂɈڍs������
			}
			else
			{
				Scene::scene = STARTLOAD;		// �V�[�����X�^�[�g���[�h��ʂɈڍs������
			}
		}
	}
	else
	{
		// �v���Z�X
		Process();
	}
	
	// �f�o�b�O�p----------------------------------�\�t�R���̎��ɏ�����
	if (KeyData::Get(KEY_INPUT_H) == 1)
	{
		Scene::scene = LOAD;
	}
	// --------------------------------------------
}

void Game::Draw()
{
	if (stagenum == 5 && lastendcount > 50)
	{
		DrawGraph(0, 0, gr_end, false);
	}

	if (stagenum == 5)
	{
		// �e����-----------------------------------------------------------------------------------------------------------
		// �`�����e�p�[�x�L�^�摜�ɂ���
		SetDrawScreen(DepthBufferGraphHandle);

		// �e�p�[�x�L�^�摜��^�����ɃN���A
		SetBackgroundColor(255, 255, 255);
		ClearDrawScreen();
		SetBackgroundColor(0, 0, 0);


		// �J�����̃^�C�v�𐳎ˉe�^�C�v�ɃZ�b�g�A�`��͈͂��w��
		SetupCamera_Ortho(2000.0f);

		// �`�悷�鉜�s���͈͂��Z�b�g
		SetCameraNearFar(0.0f, 2000.0f);

		// �J�����̈ʒu�ƒ����_�̓X�e�[�W�S�̂����n����ʒu ���̃J�����̓��C�g�̈ʒu������ �J�����͂������̃v���Z�X�ōs���Ă���
		LightPosition = VGet(-563.127f, 561.5834f, -512.355f);
		LightTarget = VGet(812.3557f, 0.0f, 612.3557f);
		SetCameraPositionAndTarget_UpVecY(LightPosition, LightTarget);

		// �ݒ肵���J�����̃r���[�s��Ǝˉe�s����擾���Ă���
		LightCamera_ViewMatrix = GetCameraViewMatrix();
		LightCamera_ProjectionMatrix = GetCameraProjectionMatrix();


		// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɐݒ肷��
		MV1SetUseOrigShader(TRUE);

		// �[�x�L�^�摜�ւ̍��̃��b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(Normal_DepthShadow_Step1_VertexShader);
		// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader(DepthShadow_Step1_PixelShader);

		if (lastendcount <= 50)
		{
			stage->Draw(stagenum);			// �X�e�[�W�̕`��
		}

		// �[�x�L�^�摜�ւ̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(Skin4_DepthShadow_Step1_VertexShader);
		// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader(DepthShadow_Step1_PixelShader);

		if (lastendcount <= 100)
		{
			character->Draw();		// �L�����N�^�[�̕`��
		}

		// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɂ����ݒ������
		MV1SetUseOrigShader(FALSE);

		// �`���𗠉�ʂɖ߂�
		SetDrawScreen(DX_SCREEN_BACK);

		// �J�����̐ݒ���s��
		camera->Process(character->GetXYZ());

		// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɐݒ肷��
		MV1SetUseOrigShader(TRUE);
		
		// �e�p�[�x�L�^�摜��`�悵���Ƃ��̃J�����̃r���[�s��Ǝˉe�s���萔�ɐݒ肷��
		SetVSConstFMtx(43, LightCamera_ViewMatrix);
		SetVSConstFMtx(47, LightCamera_ProjectionMatrix);

		// �e�p�[�x�L�^�摜���e�N�X�`���P�ɃZ�b�g
		SetUseTextureToShader(1, DepthBufferGraphHandle);


		// �[�x�L�^�摜���g�����e�{�f�B���N�V���i�����C�g��̍��̃��b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(Normal_DirLight_DepthShadow_Step2_VertexShader);
		// �[�x�L�^�摜���g�����e�{�f�B���N�V���i�����C�g��`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader(DirLight_DepthShadow_Step2_PixelShader);

		if (lastendcount <= 50)
		{
			stage->Draw(stagenum);			// �X�e�[�W�̕`��
		}
		if (camera->GetXYZ().z <= -30 && camera->GetXYZ().z >= -34 && lastendcount <= 50)
		{
			DrawBillboard3D(VGet(-20.0f, 25.0f, -190.0f), 1.0f, 1.0f, 15.0f, 0.0f, gr_lastsign, TRUE);
		}

		// �[�x�L�^�摜���g�����e�{�f�B���N�V���i�����C�g��̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader(Skin4_DirLight_DepthShadow_Step2_VertexShader);
		// �[�x�L�^�摜���g�����e�{�f�B���N�V���i�����C�g��`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader(DirLight_DepthShadow_Step2_PixelShader);

		if (lastendcount <= 100)
		{
			character->Draw();		// �L�����N�^�[�̕`��
		}

		// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɂ����ݒ������
		MV1SetUseOrigShader(FALSE);

		// �g�p�e�N�X�`���̐ݒ������
		SetUseTextureToShader(1, -1);

		// �ݒ肵���萔������
		ResetVSConstF(43, 8);
		// �e����-----------------------------------------------------------------------------------------------------------
	}
	else
	{
		stage->Draw(stagenum);	// �X�e�[�W�̕`��

		character->Draw();		// �L�����N�^�[�̕`��
	}

	// �t�F�[�h�C���t�F�[�h�A�E�g�֘A
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - count);
	if ((stagenum == 3 || stagenum == 4 || stagenum == 5)&& end == true)
	{
		DrawGraph(0, 0, gr_white, false);
	}
	else
	{
		DrawGraph(0, 0, gr_black, false);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// �f�o�b�O----------------------------------------------------------------
	/*DrawFormatString(50, 0, GetColor(255, 255, 255), "%f", character->GetXYZ().x);
	DrawFormatString(50, 50, GetColor(255, 255, 255), "%f", character->GetXYZ().y);
	DrawFormatString(50, 100, GetColor(255, 255, 255), "%f", character->GetXYZ().z);*/
	// ------------------------------------------------------------------------
}

void Game::Process()
{
	character->Process();					// �L�����N�^�[�̃v���Z�X
	if (stagenum != 5)
	{
		camera->Process(character->GetXYZ());	// �J�����̃v���Z�X
	}
	// �X�e�[�W�ԍ��R�i�N���A�^�f�G���A�j
	if (character->GetXYZ().z <= -190 && stagenum == 3)
	{
		if (dooropenflag == false)
		{
			PlaySoundMem(se_door, DX_PLAYTYPE_BACK, true);
			dooropenflag = true;
		}
		stage->ShapeStage(1, 0, stagenum);	// ���J��
	}
	//else if(character->GetXYZ().z > -190 && stagenum == 3)
	//{
	//	//stage->ShapeStage(0, 1);	// ������ // ���J���������ɂȂ����̂ŕ߂܂���
	//}
	
	// �X�e�[�W�ԍ��S
	if (stagenum == 4
		&& (character->GetXYZ().x >= 201
			&& (character->GetXYZ().z <= -229
				&& character->GetXYZ().z >= -254)))
	{
		if (dooropenflag == false)
		{
			Set3DPositionSoundMem(VGet(281, 0, -242), se_door);
			PlaySoundMem(se_door, DX_PLAYTYPE_BACK, true);
			dooropenflag = true;
		}
		stage->ShapeStage(1, 0, stagenum);	// ���J��
	}

	// �X�e�[�W�ԍ��Q�i�ǂ̌��݂ǂ�j
	if (stagenum == 2
		&& (character->GetXYZ().x <= -165
			&& character->GetXYZ().x >= -182 
			&& character->GetXYZ().z >= -118 
			&& character->GetXYZ().z <= -115))
	{
		stage->ShapeStage(1, 0, stagenum);
	}
}

void Game::Sound()
{
	// BGM
	if (stagenum == 3)
	{
		ChangeVolumeSoundMem(feedsoundcount, bgm);
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, false);
		if (character->GetXYZ().z <= 70 && feedsoundcount <= 150)
		{
			++feedsoundcount;
		}
	}
	else if (stagenum == 2)
	{
		SetLoopPosSoundMem(23820, bgm_bloodwindow);
		if (silenceflag == false)
		{
			PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, false);
		}
		else
		{
			StopSoundMem(bgm);
			if (silencecount <= 200)
			{
				++silencecount;
			}
			else
			{
				PlaySoundMem(bgm_bloodwindow, DX_PLAYTYPE_LOOP, false);
			}
		}
		if (character->GetXYZ().x <= -165
			&& character->GetXYZ().x >= -182
			&& character->GetXYZ().z >= -118
			&& character->GetXYZ().z <= -115)
		{
			silenceflag = true;
		}
	}
	else
	{
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, false);
		if (stagenum == 1)
		{
			ChangeVolumeSoundMem(225, bgm_sub);
			PlaySoundMem(bgm_sub, DX_PLAYTYPE_LOOP, false);
		}
	}

	// ����
	//ChangeVolumeSoundMem(255, se_left);
	//ChangeVolumeSoundMem(255, se_right);
	prelisten = listen;
	moveflag = character->GetMove();
	listen = camera->GetXYZ();
	sePos = character->GetXYZ();

	// �T�E���h�̍��W��ݒ�i�J������Y���͉����邱�ƂȂ����班�������̏o��ꏊ����������j
	Set3DPositionSoundMem(VGet(sePos.x + 0.5f, sePos.y + 5, sePos.z), se_left);
	Set3DPositionSoundMem(VGet(sePos.x - 0.5f, sePos.y + 5, sePos.z), se_right);

	// ���X�i�[�̈ʒu��������ݒ�
	Set3DSoundListenerPosAndFrontPos_UpVecY(VAdd(listen, sePos), VSub(VGet(sePos.x, 0, sePos.z), VGet(listen.x, 0, listen.z)));

	/*���X�i�[�̑��x��ݒ�( 60�{���Ă���̂́A�w��̒P�ʂ��b���Ȃ̂ŁA60fps�œ��삷��ꍇ��
	�P�t���[�����̈ړ��������U�O���̂P�b���̋����ł��邽�� )*/
	Set3DSoundListenerVelocity(VScale(VSub(listen, prelisten), 60.0f));

	// �����Ă�Ƃ� 
	/*�܂����[�V�����X�s�[�h�𐮐��l�ɂ��邽�߂P�O�O�{�ɂ��Ă��̂��Ƒ���ʂŕω������Ă��邽�߂��̐��l�̍ő�̂Q�{�ł���S�O�������A
	��������ڂ̕������o�ɍ��킹�邽�߂S�{���āA��{�I�ȕ������o�ɑ�̍��킹�邽�߂ɂP�P������*/
	//movespeed =((40 - (int)(character->GetSpeed() * 100)) * 4) - 11;
	movespeed =(int)(character->GetSpeed() * 100);
	if (movespeed == 6)
	{
		movespeed = 30 * 4;
	}
	else if (movespeed == 12)
	{
		movespeed = 24 * 4;
	}
	else if (movespeed == 18)
	{
		movespeed = 18 * 4;
	}
	else if (movespeed == 24)
	{
		movespeed = 12 * 4;
	}
	else
	{
		movespeed = (int)(6 * 6.5);
	}
	if (moveflag == true)
	{
		secount++;
		if (secount % movespeed == 0 || secount == 1)
		{
			sestart = true;
		}
	}
	else
	{
		secount = 0;
		footLR = 0;
	}
	// �����o��
	if (sestart == true)
	{
		if (footLR == 0)
		{
			PlaySoundMem(se_right, DX_PLAYTYPE_BACK, true);
		}
		else
		{
			PlaySoundMem(se_left, DX_PLAYTYPE_BACK, true);
			secount = movespeed;
		}
		footLR = !footLR;
		sestart = false;
	}
}