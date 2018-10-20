#include "Title.h"

Title::Title(int TitleStageHandle, int TitleModelHandle_one, int TitleModelHandle_two, int Titlebgm, int ShadowModel)
{
	gr_black = LoadGraph("media\\black.png");
	gr_start = LoadGraph("media\\title\\屋敷に入る.png");
	gr_end = LoadGraph("media\\title\\帰る.png");
	gr_icon = LoadGraph("media\\title\\カーソル.png");
	gr_player1 = LoadGraph("media\\title\\TGS　キャラ１.png");
	gr_player2 = LoadGraph("media\\title\\TGS キャラ２.png");
	gr_chara_ok = LoadGraph("media\\title\\キャラ選択画面　決定.png");
	gr_chara_no = LoadGraph("media\\title\\キャラ選択画面　戻る..png");
	gr_camera1 = LoadGraph("media\\title\\切り替えタイプ.png");
	gr_camera2 = LoadGraph("media\\title\\回転タイプ.png");
	gr_cameraimage = LoadGraph("media\\title\\カメラの切り替えタイプ選択.png");
	gr_last = LoadGraph("media\\title\\最終確認.png");
	gr_ok = LoadGraph("media\\title\\最終確認　YESゲームを始める.png");
	gr_no = LoadGraph("media\\title\\最終確認　戻る.png");
	gr_red = LoadGraph("media\\red.png");
	gr_bonus = LoadGraph("media\\title\\おまけ.png");
	gr_returnreturn = LoadGraph("media\\title\\戻る.png");

	se_select = LoadSoundMem("sound\\Select２.wav");
	se_kettei = LoadSoundMem("sound\\KIMETA.wav");
	se_return = LoadSoundMem("sound\\Select.wav");

	iconY = 0;
	iconX = 3;

	menumode = 0;
	playmodel = 3;

	bonus = false;

	camera = VGet(-25.0f, 90.0f, -632.0f);
	view = VGet(-25.0f, 90.0f, 0.0f);
	character = VGet(-35.0f, 0.0f, -600.0f);
	character2 = VGet(-15.0f, 0.0f, -600.0f);
	light = VGet(-600, 1100, -1000);

	// 頂点シェーダーを読み込む
	stagevshader = LoadVertexShader("stageshader\\Stage_PhongVS.vso");
	// ピクセルシェーダーを読み込む
	stagepshader = LoadPixelShader("stageshader\\Stage_PhongPS.pso");

	this->TitleStageHandle = TitleStageHandle;
	this->TitleModelHandle_one = TitleModelHandle_one;
	this->TitleModelHandle_two = TitleModelHandle_two;
	this->Titlebgm = Titlebgm;
	this->ShadowModel = ShadowModel;

	objectchara1 = new Character(this->TitleModelHandle_one, -1, -1, 0);
	objectchara2 = new Character(this->TitleModelHandle_two, -1, -1, 1);
	bonuschara = new Character(this->ShadowModel, -1, 9, 9);
	bonuscamera = new Camera(bonuschara->GetXYZ(), 9, -1, 1);

	// 環境証明
	SetLightDifColor(GetColorF(1, 1, 1, 1));
	SetLightSpcColor(GetColorF(1, 1, 1, 1));
	SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
	ChangeLightTypePoint(light, 3047.822f, 1.545455f, 0, 0);
	/*SetLightAmbColor(GetColorF(0, 0, 0, 0));
	SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.0f, 0.0f));*/
	/*SetLightEnable(TRUE);		/// 初期値（完成するまでいる）
	SetLightDifColor(GetColorF(1, 1, 1, 1));
	SetLightSpcColor(GetColorF(1, 1, 1, 1));
	SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
	ChangeLightTypeDir(VGet(0.577350f, 0.577350f, 0.577350f));
	SetLightPosition(VGet(0, 0, 0));
	SetLightRangeAtten(2000.0f, 0.1f, 0, 0);
	SetLightAngle(2.0f, 2.0f);*/

	//奥行0.1～1000までをカメラの描画範囲とする(これのnearの増加で髪とかが消えるの直せるもしくはZブラッシュを増やしてSetZBufferBitDepth( 24 ))
	SetCameraNearFar(5.0f, 900.0f);

	// cameraの視点から viewのターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(camera, view);


	// 第二引数の回転角度をセット
	MV1SetRotationXYZ(this->TitleModelHandle_one, VGet(0.0f, -3.0f, 0.0f));
	// 指定位置にモデルを配置
	MV1SetPosition(this->TitleModelHandle_one, character);
	// 第二引数の回転角度をセット
	MV1SetRotationXYZ(this->TitleModelHandle_two, VGet(0.0f, 3.0f, 0.0f));
	// 指定位置にモデルを配置
	MV1SetPosition(this->TitleModelHandle_two, character2);


	// ステージのモデルの座標を更新する
	MV1SetPosition(this->TitleStageHandle, VGet(0, 0, 0));
}

Title::~Title()
{
	DeleteGraph(gr_start);
	DeleteGraph(gr_end);
	DeleteGraph(gr_icon);
	DeleteGraph(gr_player1);
	DeleteGraph(gr_player2);
	DeleteGraph(gr_camera1);
	DeleteGraph(gr_camera2);
	DeleteGraph(gr_cameraimage);
	DeleteGraph(gr_ok);
	DeleteGraph(gr_no);
	DeleteGraph(gr_black);
	DeleteGraph(gr_red);
	DeleteGraph(gr_bonus);
	DeleteGraph(gr_returnreturn);

	MV1DeleteModel(TitleStageHandle);
	MV1DeleteModel(TitleModelHandle_one);
	MV1DeleteModel(TitleModelHandle_two);
	MV1DeleteModel(ShadowModel);

	DeleteSoundMem(Titlebgm);
	DeleteSoundMem(se_select);
	DeleteSoundMem(se_kettei);
	DeleteSoundMem(se_return);

	DeleteShader(stagevshader);
	DeleteShader(stagepshader);

	delete objectchara1;
	delete objectchara2;
	delete bonuschara;
	delete bonuscamera;

	InitGraph();
	InitSoundMem();
	InitShader();
}

void Title::Update()
{
	PlaySoundMem(Titlebgm, DX_PLAYTYPE_LOOP, false);

	ChangeLightTypePoint(light, 3047.822f, 1.545455f, 0, 0);

	if (start == true && end == false && bonus == false)
	{
		SetCameraPositionAndTarget_UpVecY(camera, view);
		Process();
	}
	if (bonus == true)
	{
		bonuschara->Process();
		if (camera.y >= 12.0f)
		{
			camera.y -= 0.5f;
			SetCameraPositionAndTarget_UpVecY(camera, view);
		}
		else
		{
			bonuscamera->Process(bonuschara->GetXYZ());
		}
		if (bonuschara->GetXYZ().z >= 0)
		{
			if (light.z <= 1000.0f)
			{
				light.z += 15.0f;
			}
		}
		else
		{
			if (light.z >= -1000.0f)
			{
				light.z -= 15.0f;
			}
		}
		SetLightPosition(light);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, count);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if (end == false)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - count);
		DrawGraph(0, 0, gr_black, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - count);
		DrawGraph(0, 0, gr_red, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	if (start == false)
	{
		count += 5;
		if (count == 255)
		{
			start = true;
		}
	}
	if (end == true)
	{
		count--;
		if (count <= 0)
		{
			StopSoundMem(Titlebgm);
			Scene::scene = LOAD;
		}
	}


	// キャラ表示にかかわること、ここでないとプロセスを止める都合上止まってしまう
	// 指定位置にモデルを配置
	MV1SetPosition(TitleModelHandle_one, character);
	// 指定位置にモデルを配置
	MV1SetPosition(TitleModelHandle_two, character2);
	// モーション関連
	objectchara1->Player_AnimProcess();
	objectchara2->Player_AnimProcess();
	if (end == false)
	{
		objectchara1->Player_PlayAnim(0);
		objectchara2->Player_PlayAnim(0);
	}
	else
	{
		if (playmodel == 0)
		{
			character.x += 0.01f;
			character.z += 0.3f;
			objectchara1->Player_PlayAnim(1);
			objectchara2->Player_PlayAnim(0);
		}
		else if (playmodel == 1)
		{
			character2.x -= 0.01f;
			character2.z += 0.3f;
			objectchara1->Player_PlayAnim(0);
			objectchara2->Player_PlayAnim(1);
		}
	}
}

void Title::Process()
{
	// 戻る
	if ((KeyData::Get(KEY_INPUT_Q) == 1
		|| PadData::Get(XINPUT_BUTTON_B, 0) == 1)
		&& menumode != 9)
	{
		PlaySoundMem(se_return, DX_PLAYTYPE_BACK, true);
		if (menumode != 0)
		{
			menumode--;
		}
		if (menumode == 0)
		{
			camera = VGet(-25.0f, 90.0f, -632.0f);
			iconX = 3;
			playmodel = 3;
		}
		else if (menumode == 1)
		{
			camera = VGet(-25.0f, 8.0f, -632.0f);
			iconX = 3;
			playmodel = 3;
		}
	}

	if (menumode == 0)	// 0:開始
	{
		if (KeyData::Get(KEY_INPUT_UP) == 1
			|| PadData::Get(XINPUT_BUTTON_DPAD_UP, 0) == 1)
		{
			PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
			if (iconY != 0)
			{
				iconY--;
			}
		}
		if (KeyData::Get(KEY_INPUT_DOWN) == 1
			|| PadData::Get(XINPUT_BUTTON_DPAD_DOWN, 0) == 1)
		{
			PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
			if (iconY != 1)
			{
				iconY++;
			}
		}
		if (KeyData::Get(KEY_INPUT_Z) == 1
			|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
		{
			PlaySoundMem(se_kettei, DX_PLAYTYPE_BACK, true);
			if (iconY == 0)	// 開始
			{
				menumode = 1;
			}
			else  // 終了 
			{
				Scene::endflag = true;
			}
		}
	}
	else if (menumode == 1)	// 1:キャラモデル
	{
		if (camera.y != 8.0f)
		{
			camera.y--;
			if (KeyData::Get(KEY_INPUT_Z) == 1
				|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
			{
				camera.y = 8.0f;
			}
		}
		else if(camera.y <= 8.0f)
		{
			if (KeyData::Get(KEY_INPUT_RIGHT) == 1
				|| PadData::Get(XINPUT_BUTTON_DPAD_RIGHT, 0) == 1)
			{
				PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
				iconX = 1;
			}
			if (KeyData::Get(KEY_INPUT_LEFT) == 1
				|| PadData::Get(XINPUT_BUTTON_DPAD_LEFT, 0) == 1)
			{
				PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
				iconX = 0;
			}
			if (KeyData::Get(KEY_INPUT_Z) == 1
				|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
			{
				if (iconX == 0) // キャラモデル０番(左)
				{
					PlaySoundMem(se_kettei, DX_PLAYTYPE_BACK, true);
					playmodel = 0;
					menumode = 2;
					iconX = 0;
				}
				else if (iconX == 1) // キャラモデル１番(右)
				{
					PlaySoundMem(se_kettei, DX_PLAYTYPE_BACK, true);
					playmodel = 1;
					menumode = 2;
					iconX = 0;
				}
			}
		}
		if (iconX == 0)
		{
			if (camera.x != -37)
			{
				camera.x -= 0.5f;
			}
		}
		else if (iconX == 1)
		{
			if (camera.x != -13)
			{
				camera.x += 0.5f;
			}
		}
	}
	else if (menumode == 2)	// 2:説明文
	{
		if (KeyData::Get(KEY_INPUT_DOWN) == 1
			|| PadData::Get(XINPUT_BUTTON_DPAD_DOWN, 0) == 1)
		{
			PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
			if (iconY != 1)
			{
				iconY++;
			}
		}
		if (KeyData::Get(KEY_INPUT_UP) == 1
			|| PadData::Get(XINPUT_BUTTON_DPAD_UP, 0) == 1)
		{
			PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
			if (iconY != 0)
			{
				iconY--;
			}
		}
		if (KeyData::Get(KEY_INPUT_Z) == 1
			|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
		{
			PlaySoundMem(se_kettei, DX_PLAYTYPE_BACK, true);
			if (iconY == 0)	// それでよろしいか
			{
				menumode = 3;
			}
			else // 他をご所望のようです
			{
				menumode = 1;
				camera = VGet(-25.0f, 8.0f, -632.0f);
				iconX = 3;
				playmodel = 3;
				iconY = 0;
			}
		}
	}
	else if (menumode == 3)	// 3:カメラ
	{
		if (KeyData::Get(KEY_INPUT_DOWN) == 1
			|| PadData::Get(XINPUT_BUTTON_DPAD_DOWN, 0) == 1)
		{
			PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
			if (iconY!= 1)
			{
				iconY++;
			}
		}
		if (KeyData::Get(KEY_INPUT_UP) == 1
			|| PadData::Get(XINPUT_BUTTON_DPAD_UP, 0) == 1)
		{
			PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
			if (iconY != 0)
			{
				iconY--;
			}
		}
		if (KeyData::Get(KEY_INPUT_Z) == 1
			|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
		{
			PlaySoundMem(se_kettei, DX_PLAYTYPE_BACK, true);
			if (iconY == 0) // カメラモード０
			{
				cameratype = 0;
				menumode = 4;
				iconY = 0;
			}
			else // カメラモード１
			{
				cameratype = 1;
				menumode = 4;
				iconY = 0;
			}
		}
	}
	else if (menumode == 4)	//4:最終
	{
		if (KeyData::Get(KEY_INPUT_DOWN) == 1
			|| PadData::Get(XINPUT_BUTTON_DPAD_DOWN, 0) == 1)
		{
			PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
			if (iconY != 1)
			{
				iconY++;
			}
		}
		if (KeyData::Get(KEY_INPUT_UP) == 1
			|| PadData::Get(XINPUT_BUTTON_DPAD_UP, 0) == 1)
		{
			PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
			if (iconY != 0)
			{
				iconY--;
			}
		}
		if (KeyData::Get(KEY_INPUT_Z) == 1
			|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
		{
			PlaySoundMem(se_kettei, DX_PLAYTYPE_BACK, true);
			if (iconY == 0) // これで満足
			{
				end = true;
			}
			else // やっぱ変える
			{
				menumode = 1;
				camera = VGet(-25.0f, 8.0f, -632.0f);
				iconX = 3;
				playmodel = 3;
				iconY = 0;
			}
		}
	}

	if (playmodel == 0)
	{
		if (camera.x != -37.0f)
		{
			camera.x -= 0.5f;
		}
	}
	else if (playmodel == 1)
	{
		if (camera.x != -13.0f)
		{
			camera.x += 0.5f;
		}
	}

	// おまけ
	if (KeyData::Get(KEY_INPUT_O) == 1
		|| (PadData::Get(XINPUT_BUTTON_X, 0) >= 1
			&& PadData::Get(XINPUT_BUTTON_LEFT_SHOULDER, 0) >= 1
			&& PadData::Get(XINPUT_BUTTON_RIGHT_SHOULDER, 0) >= 1))
	{
		menumode = 9;
	}
	if (menumode == 9)
	{
		if (camera.z <= 832.0f)
		{
			camera.z += 10.0f;
			light.z += 16.4f;
			if (camera.y <= 90.0f)
			{
				camera.y += 0.55f;
			}
		}
		else
		{
			if (KeyData::Get(KEY_INPUT_DOWN) == 1
				|| PadData::Get(XINPUT_BUTTON_DPAD_DOWN, 0) == 1)
			{
				PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
				if (iconY != 1)
				{
					iconY++;
				}
			}
			if (KeyData::Get(KEY_INPUT_UP) == 1
				|| PadData::Get(XINPUT_BUTTON_DPAD_UP, 0) == 1)
			{
				PlaySoundMem(se_select, DX_PLAYTYPE_BACK, true);
				if (iconY != 0)
				{
					iconY--;
				}
			}
			if (KeyData::Get(KEY_INPUT_Z) == 1
				|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
			{
				if (iconY == 0) // おまけ開始
				{
					bonus = true;
				}
				else // 戻る
				{
					menumode = 0;
					camera = VGet(-25.0f, 90.0f, -632.0f);
					light = VGet(-660, 1167, -1066);
					iconX = 3;
					playmodel = 3;
					iconY = 0;
				}
			}
		}
	}
}

void Title::Draw()
{
	MV1SetUseOrigShader(TRUE);
	// 使用するピクセルシェーダーをセット
	SetUsePixelShader(stagepshader);
	// 使用する頂点シェーダーをセット
	SetUseVertexShader(stagevshader);
	MV1DrawModel(TitleStageHandle);
	MV1SetUseOrigShader(FALSE);
	// 設定した定数を解除
	ResetVSConstF(43, 8);

	objectchara1->Draw();
	objectchara2->Draw();

	if (bonus == true)
	{
		bonuschara->Draw();
	}

	// ３Ｄ空間上に画像を描画
	if (menumode != 9)
	{
		DrawBillboard3D(VGet(-25.0f, 90.0f, -600.0f), 0.5f, 0.5f, 10.0f, 0.0f, gr_start, TRUE);
		DrawBillboard3D(VGet(-25.0f, 85.0f, -600.0f), 0.5f, 0.5f, 10.0f, 0.0f, gr_end, TRUE);
		DrawBillboard3D(VGet(-31.0f, 90.0f - (iconY * 5.0f), -600.0f), 0.5f, 0.5f, 2.0f, 0.0f, gr_icon, TRUE);
	}
	else
	{
		DrawBillboard3D(VGet(-25.0f, 90.0f, 810.0f), 0.5f, 0.5f, 10.0f, 0.0f, gr_bonus, TRUE);
		DrawBillboard3D(VGet(-25.0f, 85.0f, 810.0f), 0.5f, 0.5f, 10.0f, 0.0f, gr_returnreturn, TRUE);
		DrawBillboard3D(VGet(-19.0f, 90.0f - (iconY * 5.0f), 810.0f), 0.5f, 0.5f, 2.0f, 0.0f, gr_icon, TRUE);
		if (bonuschara->GetXYZ().z < 0)
		{
			DrawBillboard3D(VGet(-25.0f, 90.0f, -600.0f), 0.5f, 0.5f, 10.0f, 0.0f, gr_start, TRUE);
			DrawBillboard3D(VGet(-25.0f, 85.0f, -600.0f), 0.5f, 0.5f, 10.0f, 0.0f, gr_end, TRUE);
			DrawBillboard3D(VGet(-31.0f, 90.0f - (iconY * 5.0f), -600.0f), 0.5f, 0.5f, 2.0f, 0.0f, gr_icon, TRUE);
		}
	}
	
	if (menumode == 2)	// 2:説明文
	{
		if (playmodel == 0)
		{
			DrawGraph(0, 0, gr_player1, true);
		}
		else if (playmodel == 1)
		{
			DrawGraph(0, 0, gr_player2, true);
		}
		DrawGraph(200, 900, gr_chara_ok, true);
		DrawGraph(200, 1000, gr_chara_no, true);
		DrawGraph(150, 925 + (iconY * 100), gr_icon, true);
	}
	else if (menumode == 3)	// 3:カメラ
	{
		DrawGraph(300, 200, gr_camera1, true);
		DrawGraph(300, 600, gr_camera2, true);
		DrawGraph(1000, 90, gr_cameraimage, true);
		DrawGraph(250, 275 + (iconY * 400), gr_icon, true);
	}
	else if (menumode == 4)	// 4:最終
	{
		DrawGraph(0, 0, gr_last, true);
		if (end == false)
		{
			DrawGraph(800, 850, gr_ok, true);
			DrawGraph(800, 900, gr_no, true);
			DrawGraph(750, 850 + (iconY * 50), gr_icon, true);
		}
	}
}