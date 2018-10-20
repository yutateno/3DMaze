#include "Load.h"

Load::Load(int scene, int stagenum, int playmodel)
{
	loadend = false;

	this->scene = scene;

	// 背景読み込み
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

	// 次タイトル
	if (scene == 0)
	{
		max = 8;
		baxcount = 150;
		SetUseASyncLoadFlag(TRUE);
		TitleStageHandle = MV1LoadModel("media\\title\\屋敷\\描画\\タイトル屋敷.mv1");
		MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
		MV1SetLoadModelPhysicsWorldGravity(-122.5f);
		TitleModelHandle_one = MV1LoadModel("media\\character\\0814剛体実験\\剛体あり.pmx");
		TitleModelHandle_two = MV1LoadModel("media\\character\\大将丸式モデル001\\赤ずきん.pmx");
		ShadowModel = MV1LoadModel("media\\character\\00\\nothing.pmx");
		Titlebgm = LoadSoundMem("sound\\T.wav");
		SetUseASyncLoadFlag(FALSE);
	}
	// 次ゲーム
	else if (scene == 1)
	{
		// モデル番号０（四季美）だったら
		if (playmodel == 0)
		{
			// ステージ番号
			switch (stagenum)
			{
			case 0:
				max = 4;
				baxcount = 40;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage0 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage0 描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル002\\新月シキミ　v3.5 　MMD モーフ.pmx");
				bgm = LoadSoundMem("sound\\メインｍ.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 1:
				max = 4;
				baxcount = 80;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage1 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage1 描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル002\\新月シキミ　v3.5 　MMD モーフ.pmx");
				bgm = LoadSoundMem("sound\\メイン２.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 2:
				max = 4;
				baxcount = 120;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage2 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage2 描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル002\\新月シキミ　v3.5 　MMD モーフ.pmx");
				bgm = LoadSoundMem("sound\\メイン２.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 3: // (2.5)
				max = 4;
				baxcount = 160;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage2.5 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage2.5　描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル002\\新月シキミ　v3.5 　MMD モーフ.pmx");
				bgm = LoadSoundMem("sound\\メインｍ.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 4: // (3)
				max = 4;
				baxcount = 200;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage3 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage3\\stage3描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル002\\新月シキミ　v3.5 　MMD モーフ.pmx");
				bgm = LoadSoundMem("sound\\メインアレンジ.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 5: // (End)
				max = 4;
				baxcount = 125;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\title\\屋敷\\判定\\タイトル屋敷.mv1");
				DStageHandle = MV1LoadModel("media\\title\\屋敷\\描画\\タイトル屋敷.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル002\\新月シキミ　v3.5 　MMD モーフ.pmx");
				bgm = LoadSoundMem("sound\\F.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			default:
				break;
			}
		}
		// モデル番号１（紅音）だったら
		else if (playmodel == 1)
		{
			// ステージ番号
			switch (stagenum)
			{
			case 0:
				max = 4;
				baxcount = 40;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage0 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage0 描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル001\\赤ずきん.pmx");
				bgm = LoadSoundMem("sound\\メインｍ.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 1:
				max = 4;
				baxcount = 80;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage1 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage1 描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル001\\赤ずきん.pmx");
				bgm = LoadSoundMem("sound\\メイン２.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 2:
				max = 4;
				baxcount = 120;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage2 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage2 描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル001\\赤ずきん.pmx");
				bgm = LoadSoundMem("sound\\メイン２.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 3: // (2.5)
				max = 4;
				baxcount = 160;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage2.5 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage2.5　描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル001\\赤ずきん.pmx");
				bgm = LoadSoundMem("sound\\メインｍ.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 4: // (3)
				max = 4;
				baxcount = 200;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\判定ステージ\\stage3 判定.mv1");
				DStageHandle = MV1LoadModel("media\\描画ステージ\\stage3\\stage3描画.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル001\\赤ずきん.pmx");
				bgm = LoadSoundMem("sound\\メインアレンジ.wav");
				SetUseASyncLoadFlag(FALSE);
				break;

			case 5: // (End)
				max = 4;
				baxcount = 125;
				SetUseASyncLoadFlag(TRUE);
				CStageHandle = MV1LoadModel("media\\title\\屋敷\\判定\\タイトル屋敷.mv1");
				DStageHandle = MV1LoadModel("media\\title\\屋敷\\描画\\タイトル屋敷.mv1");
				MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
				MV1SetLoadModelPhysicsWorldGravity(-122.5f);
				ModelHandle = MV1LoadModel("media\\character\\大将丸式モデル001\\赤ずきん.pmx");
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

	// 非同期ロード個数が０になったら
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