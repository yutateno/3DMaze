#include "Character.h"

Character::Character(int ModelHandle, int StageHandle, int stagenum, int playmodel)
{
	charapshader = LoadPixelShader("charashader\\Character_DepthShadow_PS.pso");
	charavshader = LoadVertexShader("charashader\\Character_DepthShadow_VS.vso");

	this->collstagehandle = StageHandle;
	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(collstagehandle, -1);
	// ステージのモデルの座標を更新する
	MV1SetPosition(collstagehandle, VGet(0, 0, 0));
	// モデルハンドルが示すモデルに含まれるフレームの表示状態を変更する
	MV1SetFrameVisible(collstagehandle, -1, FALSE);
	// コリジョン情報を更新する
	MV1RefreshCollInfo(collstagehandle, -1);

	// 次に読み込むモデルの物理演算モードをリアルタイム物理演算にする
	//if (playmodel != 1)		// シキミ
	//{
	//}
	//else					// アカネ
	//{
	//}

	// ３Ｄモデルの読み込み
	this->charamodelhandle = ModelHandle;

	// 輪郭線を細くする
	materialnum = MV1GetMaterialNum(this->charamodelhandle);

	for (int i = 0; i != materialnum; ++i)
	{
		// マテリアルの元々の輪郭線の太さを取得  
		dotwidth = (double)MV1GetMaterialOutLineDotWidth(this->charamodelhandle, i);
		// マテリアルの輪郭線の太さを5分の１にする  
		MV1SetMaterialOutLineDotWidth(this->charamodelhandle, i, (float)(dotwidth / 5.0));
	}

	// ３Ｄモデルの0番目のアニメーションをアタッチする
	attachnum = 0;
	attachmotion = MV1AttachAnim(this->charamodelhandle, attachnum, -1, FALSE);

	// アタッチしたアニメーションの総再生時間を取得する
	totaltime = MV1GetAttachAnimTotalTime(this->charamodelhandle, attachmotion);

	// フレイム数を取得
	framenum = MV1GetFrameNum(this->charamodelhandle);

	modelheight = 20.0f;
	modelwigth = 5.0f;
	PLAYER_SHADOW_HEIGHT = 1;
	PLAYER_SHADOW_SIZE = 7.5f;

	// ステージによってスタート位置を指定
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

	// それぞれの速度
	speed = DX_PI_F / 90;
	walkspeed = 0.6;
	animspeed = 0.3;

	// モーション関連
	nowplaytime = 0.0;
	motionblendtime = 0.0;
	preattach = -1;
	premotionplaytime = 0.0;

	// あたり判定に関連
	wallnum = 0;
	hitflag = false;
	moveflag = false;
	floornum = 0;
	maxY = 0.0;

	// シェイプ関連
	facewaitcount = 0;
	facecount = 0.0f;
	faceup = false;
	facedown = false;
	this->playmodel = playmodel;
	
	// コントローラー関連
	inputx = 0;
	inputy = 0;

	// エンディング
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
	GetJoypadAnalogInput(&inputx, &inputy, DX_INPUT_KEY_PAD1);		// コントローラーを取得

	// スティックの押し具合で移動速度を調整
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

	// Wキーまたは左スティックが前に押されたら前進する
	if (KeyData::Get(KEY_INPUT_W) > 0
		|| inputy < 0)
	{
		area.x += (float)(sin(angle) * -walkspeed);
		area.z += (float)(cos(angle) * -walkspeed);
		moveflag = true;
		Player_PlayAnim(1);
	}

	// Sキーまたは左スティックが後ろに押されたら後退する
	if (KeyData::Get(KEY_INPUT_S) > 0
		|| inputy > 0)
	{
		area.x += (float)(sin(angle) * walkspeed);
		area.z += (float)(cos(angle) * walkspeed);
		moveflag = true;
		Player_PlayAnim(2);
	}

	// Aキーまたは左スティックが左に押されたら左回転する
	if (KeyData::Get(KEY_INPUT_A) > 0
		|| inputx < 0)
	{
		angle -= (float)speed;
	}

	// Dキーまたは左スティックが右に押されたら右回転する
	if (KeyData::Get(KEY_INPUT_D) > 0
		|| inputx > 0)
	{
		angle += (float)speed;
	}

	// WキーかSキーがが離されたらモーションを立ちにする（キーボードの時はinputy消さないとモーションおかしい
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

	// モーションの実態
	Player_AnimProcess();

	// 第二引数の回転角度をセット
	MV1SetRotationXYZ(charamodelhandle, VGet(0, angle, 0));
	// 指定位置にモデルを配置
	MV1SetPosition(charamodelhandle, area);

	// あたり判定の実態
	StageHit();

	// シェイプの実態
	FaceShape();
}

void Character::ShadowFoot()
{
	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// プレイヤーの直下に存在する地面のポリゴンを取得
	ShadowHitResDim = MV1CollCheck_Capsule(collstagehandle, -1, area, VAdd(area, VGet(0.0f, (float)PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

	// 頂点データで変化が無い部分をセット
	ShadowVertex[0].dif = GetColorU8(255, 255, 255, 255);
	ShadowVertex[0].spc = GetColorU8(0, 0, 0, 0);
	ShadowVertex[0].su = 0.0f;
	ShadowVertex[0].sv = 0.0f;
	ShadowVertex[1] = ShadowVertex[0];
	ShadowVertex[2] = ShadowVertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	ShadowHitRes = ShadowHitResDim.Dim;
	for (int i = 0; i != ShadowHitResDim.HitNum; ++i, ++ShadowHitRes)
	{
		// ポリゴンの座標は地面ポリゴンの座標
		ShadowVertex[0].pos = ShadowHitRes->Position[0];
		ShadowVertex[1].pos = ShadowHitRes->Position[1];
		ShadowVertex[2].pos = ShadowHitRes->Position[2];

		// ちょっと持ち上げて重ならないようにする
		ShadowSlideVec = VScale(ShadowHitRes->Normal, 0.5f);
		ShadowVertex[0].pos = VAdd(ShadowVertex[0].pos, ShadowSlideVec);
		ShadowVertex[1].pos = VAdd(ShadowVertex[1].pos, ShadowSlideVec);
		ShadowVertex[2].pos = VAdd(ShadowVertex[2].pos, ShadowSlideVec);

		// ポリゴンの不透明度を設定する
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

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		ShadowVertex[0].u = (ShadowHitRes->Position[0].x - area.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[0].v = (ShadowHitRes->Position[0].z - area.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[1].u = (ShadowHitRes->Position[1].x - area.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[1].v = (ShadowHitRes->Position[1].z - area.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[2].u = (ShadowHitRes->Position[2].x - area.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		ShadowVertex[2].v = (ShadowHitRes->Position[2].z - area.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		DrawPolygon3D(ShadowVertex, 1, shadowhandle, TRUE);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(ShadowHitResDim);

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
}

void Character::Draw()
{
	// プレイヤーモデルの描画
	if (playmodel != 9)
	{
		if (stagenum == 5)
		{
			MV1DrawModel(charamodelhandle);
		}
		else
		{
			MV1SetUseOrigShader(TRUE);
			// 使用する頂点シェーダーをセット
			SetUseVertexShader(charavshader);
			// 使用するピクセルシェーダーをセット
			SetUsePixelShader(charapshader);
			MV1DrawModel(charamodelhandle);
			MV1SetUseOrigShader(FALSE);
			// 設定した定数を解除
			ResetVSConstF(43, 8);
			// 影
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

	//// デバッグ----------------------------------------------------------------
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
	// 今のモーションが違うものだったら
	if (attachnum != attach)
	{
		// 直前のモーションが有効だったらデタッチする
		if (preattach != -1 && motionblendtime < 1.0)
		{
			MV1DetachAnim(charamodelhandle, preattach);
			preattach = -1;
		}
		// 今のモーションを入れる
		preattach = attachmotion;
		premotionplaytime = nowplaytime;

		// 引数を今のモーション番号に入れる
		attachnum = attach;

		// 新たに指定のモーションをモデルにアタッチする
		attachmotion = MV1AttachAnim(charamodelhandle, attachnum, -1, false);

		// 動作時間を初期化する
		nowplaytime = 0.0;

		// ブレンド率は直前のモーションが有効ではない場合は１．０ｆ( 再生中のモーション１が１００％の状態 )にする
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
	// ブレンド率が１以下の場合は１に近づける
	if (motionblendtime < 1.0)
	{
		motionblendtime += 0.1;
		if (motionblendtime >= 1.0)
		{
			motionblendtime = 1.0;
		}
	}

	// 再生している現在のモーションの処理
	if (attachmotion != -1)
	{
		// モーションの総時間を取得
		totaltime = MV1GetAttachAnimTotalTime(charamodelhandle, attachmotion);

		// 再生時間を進める
		nowplaytime += animspeed;


		// 再生時間が総時間に到達していたらループさせる
		if (nowplaytime >= totaltime)
		{
			nowplaytime = fmod(nowplaytime, totaltime);
		}

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(charamodelhandle, attachmotion, (float)nowplaytime);

		// アニメーション１のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(charamodelhandle, attachmotion, (float)motionblendtime);
	}

	// 再生している直前のモーションの処理
	if (preattach != -1)
	{
		// アニメーションの総時間を取得
		totaltime = MV1GetAttachAnimTotalTime(charamodelhandle, preattach);

		// 再生時間を進める
		premotionplaytime += animspeed;

		// 再生時間が総時間に到達していたら再生時間をループさせる
		if (premotionplaytime > totaltime)
		{
			premotionplaytime = fmod(premotionplaytime, totaltime);
		}

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(charamodelhandle, preattach, (float)premotionplaytime);

		// アニメーション２のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(charamodelhandle, preattach, (float)(1.0 - motionblendtime));
	}
}

void Character::StageHit()
{
	// プレイヤーの周囲にあるステージポリゴンを取得する
	hitdim = MV1CollCheck_Capsule(collstagehandle, -1, area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), 2000.f + VSize(area));

	// 壁ポリゴンと床ポリゴンの数を初期化する
	wallnum = 0;
	floornum = 0;

	// 検出されたポリゴンの数だけ繰り返し
	for (int i = 0; i != hitdim.HitNum; ++i)
	{
		// ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
		if (hitdim.Dim[i].Normal.y < 0.001f && hitdim.Dim[i].Normal.y > -0.001f)
		{
			// 壁ポリゴンと判断された場合でも、プレイヤーのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
			if (hitdim.Dim[i].Position[0].y > area.y + 1.0f
				|| hitdim.Dim[i].Position[1].y > area.y + 1.0f
				|| hitdim.Dim[i].Position[2].y > area.y + 1.0f)
			{
				// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
				if (wallnum < 2048)
				{
					// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
					wallpoly[wallnum] = &hitdim.Dim[i];

					// 壁ポリゴンの数を加算する
					wallnum++;
				}
			}
		}
		else
		{
			// ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
			if (floornum < 2048)
			{
				// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
				floorpoly[floornum] = &hitdim.Dim[i];

				// 床ポリゴンの数を加算する
				floornum++;
			}
		}
	}

	// 壁ポリゴンとの当たり判定処理
	if (wallnum != 0)
	{
		// 壁に当たったかどうかのフラグは初期状態では「当たっていない」にしておく
		hitflag = 0;

		// 移動したかどうかで処理を分岐
		if (moveflag == 1)
		{
			// 壁ポリゴンの数だけ繰り返し
			for (int i = 0; i != wallnum; ++i)
			{
				// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				poly = wallpoly[i];

				// ポリゴンとプレイヤーが当たっていなかったら次のカウントへ
				if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE)
				{
					continue;
				}

				// ここにきたらポリゴンとプレイヤーが当たっているということなので、ポリゴンに当たったフラグを立てる
				hitflag = 1;

				// 新たな移動座標で壁ポリゴンと当たっていないかどうかを判定する
				for (int j = 0; j != wallnum; ++j)
				{
					// j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					poly = wallpoly[j];

					// 当たっていたらループから抜ける
					if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
					{
						break;
					}
				}
			}
		}
		else
		{
			// 壁ポリゴンの数だけ繰り返し
			for (int i = 0; i != wallnum; ++i)
			{
				// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				poly = wallpoly[i];

				// ポリゴンに当たっていたら当たったフラグを立てた上でループから抜ける
				if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
				{
					hitflag = 1;
					break;
				}
			}
		}

		// 壁に当たっていたら壁から押し出す処理を行う
		if (hitflag == 1)
		{
			// 壁からの押し出し処理を試みる最大数だけ繰り返し
			for (int k = 0; k != 32; ++k)
			{
				// 壁ポリゴンの数だけ繰り返し
				for (int i = 0; i != wallnum; ++i)
				{
					// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					poly = wallpoly[i];

					// プレイヤーと当たっているかを判定
					if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE)
					{
						continue;
					}

					// 当たっていたら規定距離分プレイヤーを壁の法線方向に移動させる
					area = VAdd(area, VScale(poly->Normal, (float)walkspeed / 6));		// XとZ座標が両方違うならこれにして片方だけなら移動速度と同じだけにしたいなぁ直前座標取ってやるべし

					// 移動した上で壁ポリゴンと接触しているかどうかを判定
					for (int j = 0; j != wallnum; ++j)
					{
						// 当たっていたらループを抜ける
						poly = wallpoly[j];
						if (HitCheck_Capsule_Triangle(area, VAdd(area, VGet(0.0f, modelheight, 0.0f)), modelwigth, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
						{
							break;
						}
					}

					// i が KabeNum ではない場合は全部のポリゴンで押し出しを試みる前に全ての壁ポリゴンと接触しなくなったということなのでループから抜ける
					break;
				}
			}
		}
	}

	// 床ポリゴンとの当たり判定
	if (floornum != 0)
	{
		// 床ポリゴンに当たったかどうかのフラグを倒しておく
		hitflag = 0;

		// 一番高い床ポリゴンにぶつける為の判定用変数を初期化
		maxY = 0.0;

		// 床ポリゴンの数だけ繰り返し
		for (int i = 0; i != floornum; ++i)
		{
			// i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
			poly = floorpoly[i];

			// 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
			lineresult = HitCheck_Line_Triangle(VAdd(area, VGet(0.0f, modelheight, 0.0f)), VAdd(area, VGet(0.0f, -1.0f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);

			// 当たっていなかったら何もしない
			if (lineresult.HitFlag == FALSE)
			{
				continue;
			}

			// 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
			if (hitflag == 1 && maxY > lineresult.Position.y)
			{
				continue;
			}
			// 接触したＹ座標を保存する
			maxY = lineresult.Position.y;

			// ポリゴンに当たったフラグを立てる
			hitflag = 1;
		}

			// 床ポリゴンに当たったかどうかで処理を分岐
		if (hitflag == 1)
		{
			// 当たった場合
			// 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
			area.y = maxY;
		}
	}

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(hitdim);
}

void Character::FaceShape()
{
	// 待機フレームが足らなかったら
	if (facewaitcount <= 300)
	{
		++facewaitcount;
		// ある程度足りたら
		if (facewaitcount == 295)
		{
			faceup = true;	// フェイスをさせる
		}
	}
	// フェイスをさせる
	if (faceup == true)
	{
		if (facecount <= 1)
		{
			facecount += 0.1f;
		}
		// 最大まで達したら
		if (facecount >= 1)
		{
			faceup = false;
			facedown = true;	// 下げる
		}
	}
	// フェイスをやめさせる
	if (facedown == true)
	{
		if (facecount >= 0)
		{
			facecount -= 0.1f;
		}
		// 最低まで達したら
		if (facecount <= 0)
		{
			faceup = false;
			facedown = false;
			facewaitcount = 0;
		}
	}
	
	if (playmodel == 0)
	{
		MV1SetShapeRate(charamodelhandle, MV1SearchShape(charamodelhandle, "顔1"), facecount);
		MV1SetShapeRate(charamodelhandle, MV1SearchShape(charamodelhandle, "顔2"), facecount);
	}
	else if (playmodel == 1)
	{
		MV1SetShapeRate(charamodelhandle, MV1SearchShape(charamodelhandle, "顔1"), facecount);
		MV1SetShapeRate(charamodelhandle, MV1SearchShape(charamodelhandle, "剣をしまう"), 1.0f);
	}
}

// ゲッター
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