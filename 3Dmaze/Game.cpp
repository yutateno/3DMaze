#include "Game.h"

// モデルハンドルと判定用ステージハンドルと描画用ステージハンドルとBGMとステージ番号とカメラタイプを引数に持つコンストラクタ
Game::Game(int ModelHandle, int CStageHandle, int DStageHandle, int bgm, int stagenum, bool cameratype, int playmodel)
{
	// ステージごとにロードが異なるもの
	switch (stagenum)
	{
	case 0:
		gr_black = LoadGraph("media\\black.png");
		break;

	case 1:
		gr_black = LoadGraph("media\\black.png");
		bgm_sub = LoadSoundMem("sound\\メインｍ.wav");
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
		// 影生成-----------------------------------------------------------------------------------------------------------
		// 作成する画像のフォーマットを不動小数点型で１チャンネル、１６ビットにする
		SetDrawValidFloatTypeGraphCreateFlag(TRUE);
		SetCreateDrawValidGraphChannelNum(1);
		SetCreateGraphColorBitDepth(16);

		// 影用深度記録画像の作成
		DepthBufferGraphHandle = MakeScreen(8192, 8192, FALSE);	// ここの数値を大きくすると大雑把になって小さくすると鮮明になる

																// 設定を元に戻す
		SetDrawValidFloatTypeGraphCreateFlag(FALSE);
		SetCreateDrawValidGraphChannelNum(4);
		SetCreateGraphColorBitDepth(32);

		// 深度記録画像描画用の頂点シェーダーを読み込む
		Skin4_DepthShadow_Step1_VertexShader = LoadVertexShader("shadowshader\\SkinMesh4_DepthShadow_Step1VS.vso");
		Normal_DepthShadow_Step1_VertexShader = LoadVertexShader("shadowshader\\NormalMesh_DepthShadow_Step1VS.vso");

		// 深度記録画像描画用のピクセルシェーダーを読み込む
		DepthShadow_Step1_PixelShader = LoadPixelShader("shadowshader\\DepthShadow_Step1PS.pso");

		// 深度記録画像を使ったディレクショナルライト一つの描画用頂点シェーダーを読み込む
		Skin4_DirLight_DepthShadow_Step2_VertexShader = LoadVertexShader("shadowshader\\SkinMesh4_DirLight_DepthShadow_Step2VS.vso");
		Normal_DirLight_DepthShadow_Step2_VertexShader = LoadVertexShader("shadowshader\\NormalMesh_DirLight_DepthShadow_Step2VS.vso");

		// 深度記録画像を使ったディレクショナルライト一つの描画用ピクセルシェーダーを読み込む
		DirLight_DepthShadow_Step2_PixelShader = LoadPixelShader("shadowshader\\DirLight_DepthShadow_Step2PS.pso");

		// 影生成-----------------------------------------------------------------------------------------------------------
		break;

	default:
		break;
	}
	
	// 実体
	character = new Character(ModelHandle, CStageHandle, stagenum, playmodel);
	camera = new Camera(character->GetXYZ(), stagenum, CStageHandle, cameratype);
	stage = new Stage(DStageHandle);
	// ステージ
	this->stagenum = stagenum;

	// エンディング
	lastendflag = false;
	lastendcount = 0;
	character->SetEnd(lastendflag);

	// BGM
	this->bgm = bgm;

	// ライト初期化
	switch (stagenum)
	{
	case 0:		// チュートリアル
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-153, 132, -244), 500, 1, 0.00025f, 0);
		break;
	case 1:		// １ステージ
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-369, 267, -328), 1000, 1, 0.00025f, 0);
		break;
	case 2:		// ２ステージ
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-308.071f, 222.496f, 308.071f), 900, 1, 0.00025f, 0);
		break;
	case 3:		// ２．５ステージ
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		ChangeLightTypePoint(VGet(0.000f, 20.000f, -230.240f), 564, 0.5f, 0.0001f, 0.000005f);
		break;
	case 4:		// 3ステージ
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypePoint(VGet(-356.556f, 435.791f, -356.556f), 1325.381f, 1.000f, 0.000f, 0.000f);
		break;
	case 5:		// endステージ
		SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.33f));
		SetGlobalAmbientLight(GetColorF(0.25f, 0.25f, 0.25f, 0.25f));
		ChangeLightTypeSpot(VGet(-891.650f, 534.990f, -1069.980f), VGet(0.509f, -0.402f, 0.761f), 2.000f, 2.000f, 2529.044f, 1, 0, 0);
		break;
	default:
		/*SetLightEnable(TRUE);		/// 初期値（完成するまでいる）
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

	// 音
	bgm_feedout = LoadSoundMem("sound\\Stairs.wav");

	// サウンド関連
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

	// 4.0f で１メートルとする
	Set3DSoundOneMetre(4.0f);

	// 3D音源として読み込み
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

	// サウンドの座標を設定
	Set3DPositionSoundMem(sePos, se_left);
	Set3DPositionSoundMem(sePos, se_right);

	// サウンドが届く( 音が聞こえる )範囲を設定
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
	// 描画
	Draw();

	// サウンド
	Sound();

	// ステージ番号０のエンド位置
	if (character->GetXYZ().y >= 25 && stagenum == 0)
	{
		end = true;
	}
	// ステージ番号１のエンド位置
	if (character->GetXYZ().y >= 50 && stagenum == 1)
	{
		end = true;
	}
	// ステージ番号２のエンド位置（まだ未完テスト用 xを<=か>=にして）
	if (((character->GetXYZ().x >= 159 && character->GetXYZ().x <= 161)
		&& (character->GetXYZ().z <= -159 && character->GetXYZ().z >= -174))
		&& stagenum == 2)
	{
		moveflag = false;
		sestart = false;
		end = true;
	}
	// ステージ番号2.5のエンド位置
	if (character->GetXYZ().z <= -230 && stagenum == 3)
	{
		end = true;
	}
	// ステージ番号３のエンド位置
	if (stagenum == 4
		&& character->GetXYZ().x <= 281 && character->GetXYZ().x >= 276
		&& character->GetXYZ().z <= -229 && character->GetXYZ().z >= -254)
	{
		end = true;
	}

	// フェードイン
	if (start == false)
	{
		count += 5;
		if (count == 255)
		{
			start = true;
		}
	}
	// エンディング
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

	// 終了フラッグが立ったら
	if (end == true)
	{
		if (count >= 250)
		{
			PlaySoundMem(bgm_feedout, DX_PLAYTYPE_BACK, false);
		}
		moveflag = false;
		sestart = false;
		// フェードアウトさせる
		character->Player_AnimProcess();	// キャラクターを立ちモーションにする
		character->Player_PlayAnim(0);
		count -= 3;
		if (count <= 0)
		{
			if (stagenum != 5)
			{
				Scene::scene = LOAD;			// シーンをロード画面に移行させる
			}
			else
			{
				Scene::scene = STARTLOAD;		// シーンをスタートロード画面に移行させる
			}
		}
	}
	else
	{
		// プロセス
		Process();
	}
	
	// デバッグ用----------------------------------ソフコンの時に消して
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
		// 影生成-----------------------------------------------------------------------------------------------------------
		// 描画先を影用深度記録画像にする
		SetDrawScreen(DepthBufferGraphHandle);

		// 影用深度記録画像を真っ白にクリア
		SetBackgroundColor(255, 255, 255);
		ClearDrawScreen();
		SetBackgroundColor(0, 0, 0);


		// カメラのタイプを正射影タイプにセット、描画範囲も指定
		SetupCamera_Ortho(2000.0f);

		// 描画する奥行き範囲をセット
		SetCameraNearFar(0.0f, 2000.0f);

		// カメラの位置と注視点はステージ全体が見渡せる位置 このカメラはライトの位置を示す カメラはすぐ下のプロセスで行っている
		LightPosition = VGet(-563.127f, 561.5834f, -512.355f);
		LightTarget = VGet(812.3557f, 0.0f, 612.3557f);
		SetCameraPositionAndTarget_UpVecY(LightPosition, LightTarget);

		// 設定したカメラのビュー行列と射影行列を取得しておく
		LightCamera_ViewMatrix = GetCameraViewMatrix();
		LightCamera_ProjectionMatrix = GetCameraProjectionMatrix();


		// モデルの描画にオリジナルのシェーダーを使用するように設定する
		MV1SetUseOrigShader(TRUE);

		// 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
		SetUseVertexShader(Normal_DepthShadow_Step1_VertexShader);
		// 深度記録画像への描画用のピクセルシェーダーをセット
		SetUsePixelShader(DepthShadow_Step1_PixelShader);

		if (lastendcount <= 50)
		{
			stage->Draw(stagenum);			// ステージの描画
		}

		// 深度記録画像へのスキニングメッシュ描画用の頂点シェーダーをセット
		SetUseVertexShader(Skin4_DepthShadow_Step1_VertexShader);
		// 深度記録画像への描画用のピクセルシェーダーをセット
		SetUsePixelShader(DepthShadow_Step1_PixelShader);

		if (lastendcount <= 100)
		{
			character->Draw();		// キャラクターの描画
		}

		// モデルの描画にオリジナルのシェーダーを使用するようにした設定を解除
		MV1SetUseOrigShader(FALSE);

		// 描画先を裏画面に戻す
		SetDrawScreen(DX_SCREEN_BACK);

		// カメラの設定を行う
		camera->Process(character->GetXYZ());

		// モデルの描画にオリジナルのシェーダーを使用するように設定する
		MV1SetUseOrigShader(TRUE);
		
		// 影用深度記録画像を描画したときのカメラのビュー行列と射影行列を定数に設定する
		SetVSConstFMtx(43, LightCamera_ViewMatrix);
		SetVSConstFMtx(47, LightCamera_ProjectionMatrix);

		// 影用深度記録画像をテクスチャ１にセット
		SetUseTextureToShader(1, DepthBufferGraphHandle);


		// 深度記録画像を使った影＋ディレクショナルライト一つの剛体メッシュ描画用の頂点シェーダーをセット
		SetUseVertexShader(Normal_DirLight_DepthShadow_Step2_VertexShader);
		// 深度記録画像を使った影＋ディレクショナルライト一つ描画用のピクセルシェーダーをセット
		SetUsePixelShader(DirLight_DepthShadow_Step2_PixelShader);

		if (lastendcount <= 50)
		{
			stage->Draw(stagenum);			// ステージの描画
		}
		if (camera->GetXYZ().z <= -30 && camera->GetXYZ().z >= -34 && lastendcount <= 50)
		{
			DrawBillboard3D(VGet(-20.0f, 25.0f, -190.0f), 1.0f, 1.0f, 15.0f, 0.0f, gr_lastsign, TRUE);
		}

		// 深度記録画像を使った影＋ディレクショナルライト一つのスキニングメッシュ描画用の頂点シェーダーをセット
		SetUseVertexShader(Skin4_DirLight_DepthShadow_Step2_VertexShader);
		// 深度記録画像を使った影＋ディレクショナルライト一つ描画用のピクセルシェーダーをセット
		SetUsePixelShader(DirLight_DepthShadow_Step2_PixelShader);

		if (lastendcount <= 100)
		{
			character->Draw();		// キャラクターの描画
		}

		// モデルの描画にオリジナルのシェーダーを使用するようにした設定を解除
		MV1SetUseOrigShader(FALSE);

		// 使用テクスチャの設定を解除
		SetUseTextureToShader(1, -1);

		// 設定した定数を解除
		ResetVSConstF(43, 8);
		// 影生成-----------------------------------------------------------------------------------------------------------
	}
	else
	{
		stage->Draw(stagenum);	// ステージの描画

		character->Draw();		// キャラクターの描画
	}

	// フェードインフェードアウト関連
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

	// デバッグ----------------------------------------------------------------
	/*DrawFormatString(50, 0, GetColor(255, 255, 255), "%f", character->GetXYZ().x);
	DrawFormatString(50, 50, GetColor(255, 255, 255), "%f", character->GetXYZ().y);
	DrawFormatString(50, 100, GetColor(255, 255, 255), "%f", character->GetXYZ().z);*/
	// ------------------------------------------------------------------------
}

void Game::Process()
{
	character->Process();					// キャラクターのプロセス
	if (stagenum != 5)
	{
		camera->Process(character->GetXYZ());	// カメラのプロセス
	}
	// ステージ番号３（クリア疑惑エリア）
	if (character->GetXYZ().z <= -190 && stagenum == 3)
	{
		if (dooropenflag == false)
		{
			PlaySoundMem(se_door, DX_PLAYTYPE_BACK, true);
			dooropenflag = true;
		}
		stage->ShapeStage(1, 0, stagenum);	// 扉開く
	}
	//else if(character->GetXYZ().z > -190 && stagenum == 3)
	//{
	//	//stage->ShapeStage(0, 1);	// 扉閉じる // 扉開けっ放しになったので閉めません
	//}
	
	// ステージ番号４
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
		stage->ShapeStage(1, 0, stagenum);	// 扉開く
	}

	// ステージ番号２（壁の血みどろ）
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

	// 足音
	//ChangeVolumeSoundMem(255, se_left);
	//ChangeVolumeSoundMem(255, se_right);
	prelisten = listen;
	moveflag = character->GetMove();
	listen = camera->GetXYZ();
	sePos = character->GetXYZ();

	// サウンドの座標を設定（カメラのY軸は下がることないから少し足音の出る場所を高くする）
	Set3DPositionSoundMem(VGet(sePos.x + 0.5f, sePos.y + 5, sePos.z), se_left);
	Set3DPositionSoundMem(VGet(sePos.x - 0.5f, sePos.y + 5, sePos.z), se_right);

	// リスナーの位置を向きを設定
	Set3DSoundListenerPosAndFrontPos_UpVecY(VAdd(listen, sePos), VSub(VGet(sePos.x, 0, sePos.z), VGet(listen.x, 0, listen.z)));

	/*リスナーの速度を設定( 60倍しているのは、指定の単位が秒速なので、60fpsで動作する場合は
	１フレーム分の移動距離＝６０分の１秒分の距離であるため )*/
	Set3DSoundListenerVelocity(VScale(VSub(listen, prelisten), 60.0f));

	// 動いてるとき 
	/*まずモーションスピードを整数値にするため１００倍にしてそのあと操作量で変化させているためその数値の最大の２倍である４０を引く、
	それを二歩目の歩数感覚に合わせるため４倍して、基本的な歩数感覚に大体合わせるために１１を引く*/
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
	// 音を出す
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