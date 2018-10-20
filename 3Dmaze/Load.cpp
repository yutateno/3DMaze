#include "Load.h"

Load::Load(int scene, int stagenum, int playmodel)
{
	loadend = false;

	this->scene = scene;

	// �w�i�ǂݍ���
	gr = LoadGraph("media\\back2.png");
	if (scene == 0)
	{
		gr_bax = LoadGraph("media\\black.png");
	}
	else if (scene == 1)
	{
		if (stagenum != 5)
		{
			gr_bax = LoadGraph("media\\red.png");
		}
		else
		{
			gr_bax = LoadGraph("media\\white.png");
		}
	}

	// ���^�C�g��
	if (scene == 0)
	{
		max = 8;
		baxcount = 150;
		SetUseASyncLoadFlag(TRUE);
		TitleStageHandle = MV1LoadModel("media\\title\\���~\\�`��\\�^�C�g�����~.mv1");
		MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
		MV1SetLoadModelPhysicsWorldGravity(-122.5f);
		TitleModelHandle_one = MV1LoadModel("media\\character\\0814���̎���\\���̂���.pmx");
		TitleModelHandle_two = MV1LoadModel("media\\character\\�叫�ێ����f��001\\�Ԃ�����.pmx");
		ShadowModel = MV1LoadModel("media\\character\\00\\nothing.pmx");
		Titlebgm = LoadSoundMem("sound\\T.wav");
		SetUseASyncLoadFlag(FALSE);
	}
	// ���Q�[��
	else if (scene == 1)
	{
		// ���f���ԍ��O�i�l�G���j��������
		if (playmodel == 0)
		{
			// �X�e�[�W�ԍ�
			switch (stagenum)
			{
			case 0:
				max = 4;
				baxcount = 40;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage0 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage0 �`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��002\\�V���V�L�~�@v3.5 �@MMD ���[�t.pmx");
				bgm = LoadSoundMem("sound\\���C����.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 1:
				max = 4;
				baxcount = 80;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage1 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage1 �`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��002\\�V���V�L�~�@v3.5 �@MMD ���[�t.pmx");
				bgm = LoadSoundMem("sound\\���C���Q.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 2:
				max = 4;
				baxcount = 120;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage2 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage2 �`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��002\\�V���V�L�~�@v3.5 �@MMD ���[�t.pmx");
				bgm = LoadSoundMem("sound\\���C���Q.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 3: // (2.5)
				max = 4;
				baxcount = 160;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage2.5 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage2.5�@�`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��002\\�V���V�L�~�@v3.5 �@MMD ���[�t.pmx");
				bgm = LoadSoundMem("sound\\���C����.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 4: // (3)
				max = 4;
				baxcount = 200;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage3 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage3\\stage3�`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��002\\�V���V�L�~�@v3.5 �@MMD ���[�t.pmx");
				bgm = LoadSoundMem("sound\\���C���A�����W.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 5: // (End)
				max = 4;
				baxcount = 125;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\title\\���~\\����\\�^�C�g�����~.mv1");
				DStageHandle = MV1LoadModel("media\\title\\���~\\�`��\\�^�C�g�����~.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��002\\�V���V�L�~�@v3.5 �@MMD ���[�t.pmx");
				bgm = LoadSoundMem("sound\\F.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			default:
				break;
			}
		}
		// ���f���ԍ��P�i�g���j��������
		else if (playmodel == 1)
		{
			// �X�e�[�W�ԍ�
			switch (stagenum)
			{
			case 0:
				max = 4;
				baxcount = 40;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage0 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage0 �`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��001\\�Ԃ�����.pmx");
				bgm = LoadSoundMem("sound\\���C����.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 1:
				max = 4;
				baxcount = 80;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage1 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage1 �`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��001\\�Ԃ�����.pmx");
				bgm = LoadSoundMem("sound\\���C���Q.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 2:
				max = 4;
				baxcount = 120;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage2 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage2 �`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��001\\�Ԃ�����.pmx");
				bgm = LoadSoundMem("sound\\���C���Q.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 3: // (2.5)
				max = 4;
				baxcount = 160;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage2.5 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage2.5�@�`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��001\\�Ԃ�����.pmx");
				bgm = LoadSoundMem("sound\\���C����.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 4: // (3)
				max = 4;
				baxcount = 200;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\����X�e�[�W\\stage3 ����.mv1");
				DStageHandle = MV1LoadModel("media\\�`��X�e�[�W\\stage3\\stage3�`��.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��001\\�Ԃ�����.pmx");
				bgm = LoadSoundMem("sound\\���C���A�����W.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 5: // (End)
				max = 4;
				baxcount = 125;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\title\\���~\\����\\�^�C�g�����~.mv1");
				DStageHandle = MV1LoadModel("media\\title\\���~\\�`��\\�^�C�g�����~.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\�叫�ێ����f��001\\�Ԃ�����.pmx");
				bgm = LoadSoundMem("sound\\F.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			default:
				break;
			}
		}
	}
}

Load::~Load()
{
	DeleteGraph(gr);
	DeleteGraph(gr_bax);

	if (scene == 0)
	{
		MV1DeleteModel(TitleStageHandle);
		MV1DeleteModel(TitleModelHandle_one);
		MV1DeleteModel(TitleModelHandle_two);
		MV1DeleteModel(ShadowModel);
		DeleteSoundMem(Titlebgm);
	}
	else if (scene == 1)
	{
		MV1DeleteModel(CStageHandle);
		MV1DeleteModel(DStageHandle);
		MV1DeleteModel(ModelHandle);
		DeleteSoundMem(bgm);
	}

	InitGraph();
	InitSoundMem();
}

void Load::Update()
{
	if (scene == 0)
	{
		DrawGraph(0, 0, gr, false);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, baxcount);
		DrawGraph((((max - GetASyncLoadNum()) * 480) / 2), 0, gr_bax, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else if (scene == 1)
	{
		DrawGraph(0, 0, gr, false);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, baxcount);
		DrawGraph(((max - GetASyncLoadNum()) * 480) - 1920, 0, gr_bax, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	// �񓯊����[�h�����O�ɂȂ�����
	if (GetASyncLoadNum() == 0)
	{
		loadend = true;
	}
}

int Load::GetDStageH()
{
	return DStageHandle;
}

int Load::GetCStageH()
{
	return CStageHandle;
}

int Load::GetModelH()
{
	return ModelHandle;
}

int Load::GetBGM()
{
	return bgm;
}

int Load::GetTitleStageH()
{
	return TitleStageHandle;
}

int Load::GetTitleModelH_one()
{
	return TitleModelHandle_one;
}

int Load::GetTitleModelH_two()
{
	return TitleModelHandle_two;
}

int Load::GetTitlebgm()
{
	return Titlebgm;
}

int Load::GetShadowM()
{
	return ShadowModel;
}

bool Load::GetLoadEnd()
{
	return loadend;
}