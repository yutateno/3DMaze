#include "Manager.h"

Manager::Manager()
{
	preScene = STARTLOAD;	// 直前をスタートロードにする
	nowScene = STARTLOAD;	// 今をスタートロードにする

	stagenum = 0;		// 最初のステージ番号を０にする
	playmodel = 0;		// 適当に初期化
	cameratype = 0;		// 適当に初期化

	scene->SetScene(STARTLOAD);

	load = new Load(0, stagenum, playmodel);	// スタートロードをnewする
}

Manager::~Manager()
{
	if (scene != NULL)
	{
		delete scene;
	}
	if (load != NULL)
	{
		delete load;
	}

	InitGraph();
	InitSoundMem();
	InitShader();
}

void Manager::Update()
{
	if (preScene == TITLE || preScene == GAME)
	{
		nowScene = Scene::GetScene();	// 今のシーンを入手する
		scene->Update();				// 更新
		// 直前のシーンで処理をさせる
		if (preScene == TITLE)
		{
			playmodel = scene->GetPlayModel();		// モデル番号とカメラのモードを取得するゲームが終わるまでは不変
			cameratype = scene->GetCameraType();
		}
	}
	else if (preScene == STARTLOAD || preScene == LOAD)
	{
		load->Update();
		if (load->GetLoadEnd() == true)
		{
			if (preScene == STARTLOAD)
			{
				scene->SetScene(TITLE);
				nowScene = TITLE;
			}
			else if (preScene == LOAD)
			{
				scene->SetScene(GAME);
				nowScene = GAME;
			}
		}
	}
	if (stagenum >= 6)
	{
		stagenum = 0;
	}
	// 直前と今のシーンが違ったら
	if (preScene != nowScene)
	{
		Manage();				// new deleteさせる
		preScene = nowScene;	// 直前のシーンを今のシーンにする
	}
}

void Manager::Manage()
{
	if (nowScene == STARTLOAD || nowScene == LOAD)
	{
		delete load;
		delete scene;		// 消す
	}

	switch (nowScene)
	{
	case STARTLOAD:
		stagenum = 0;
		load = new Load(0, stagenum, playmodel);
		break;

	case TITLE:
		scene = new Title(load->GetTitleStageH(), load->GetTitleModelH_one(), load->GetTitleModelH_two(), load->GetTitlebgm(), load->GetShadowM());
		break;

	case LOAD:
		load = new Load(1, stagenum, playmodel);
		break;

	case GAME:
		scene = new Game(load->GetModelH(), load->GetCStageH(), load->GetDStageH(), load->GetBGM(), stagenum, cameratype, playmodel);
		stagenum++;
		break;

	default:
		break;
	}
}