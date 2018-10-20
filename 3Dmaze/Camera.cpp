#include "Camera.h"

Camera::Camera(VECTOR xyz, int stagenum, int StageHandle, bool cameratype)
{
	collstagehandle = StageHandle;

	// モデル全体のコリジョン情報のセットアップ
	MV1SetupCollInfo(collstagehandle, -1);
	// ステージのモデルの座標を更新する
	MV1SetPosition(collstagehandle, VGet(0, 0, 0));
	// モデルハンドルが示すモデルに含まれるフレームの表示状態を変更する
	MV1SetFrameVisible(collstagehandle, -1, FALSE);
	// コリジョン情報を更新する
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
		camerazoom = 17;		// 階段開始なのでズームさせている状態にしとく
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
		view = VGet(0, 17, 0);	// stagenumが9のときだけ注視位置を変える
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

	if (this->cameratype == 0)	// 切り替え型
	{
		speed = DX_PI_F / 2;
	}
	else // 回転型（おまけステージの回転スピードは生成時にこれに指定）
	{
		speed = DX_PI_F / 90;
	}

	rightleft = false;	// 初期値は扱わないのでfalseにしとく

	// 完成まで残しとく
	//奥行0.1～1000までをカメラの描画範囲とする(これのnearの増加で髪とかが消えるの直せるもしくはZブラッシュを増やしてSetZBufferBitDepth( 24 ))
	//SetCameraNearFar(1.0f, 500.0f);

	//area + charaの視点からview + charaのターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(VAdd(area, chara), VAdd(view, chara));
}

Camera::~Camera()
{

}

void Camera::Process(VECTOR xyz)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);	// コントローラーを取得
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

	// キャラの位置を更新し続ける
	chara = xyz;

	// ステージとのそれぞれのカメラのあたり判定（8方向）
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
	// ここまでカメラとステージのあたり判定

	// ステージ番号９（おまけステージ）じゃないとき
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
		// 切り替え型
		if (cameratype == 0)
		{
			// 前後変更 topcameraが埋まってなかったら実行
			if ((KeyData::Get(KEY_INPUT_RCONTROL) == 1
				|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
				&& topcamera == 0)
			{
				area = VGet(area.x*-1, area.y, area.z*-1);
			}
			// 左に回転中 leftcameraが埋まってなかったら実行
			if ((KeyData::Get(KEY_INPUT_LEFT) == 1
				|| inputleft == 1)
				&& leftcamera == 0)
			{
				area = RLrotate(speed);
			}
			// 右に回転中 rightcameraが埋まってなかったら実行
			if ((KeyData::Get(KEY_INPUT_RIGHT) == 1
				|| inputright == 1)
				&& rightcamera == 0)
			{
				area = RLrotate(-speed);
			}

			// 今のカメラが埋まっていたら
			if (nowcamera != 0)
			{
				// topcameraが埋まってなかったら
				if (topcamera == 0)
				{
					// ステージ番号３（クリア疑惑エリア）じゃなかったら
					if (stagenum != 3)
					{
						// キャラクターが階段を上っていなかったら（０でいいんですが一応0.5fにしています）
						if (chara.y <= 0.5f)
						{
							area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// ズームイン処理をさせる
							camerazoom++;
						}
						else
						{
							area = VGet(area.x*-1, area.y, area.z*-1);	// カメラを前後反対にする
						}
					}
					else
					{
						area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// ズームイン処理をさせる
						camerazoom++;
					}
				}
				else
				{
					// 左下カメラが埋まっていないが右下カメラが埋まっているとき
					if (lowerleftcamera == 0 && lowerrightcamera != 0)
					{
						area = RLrotate(speed);	// 左回転させる
					}
					// 左下カメラが埋まっているが右下カメラが埋まっていなかったら
					else if (lowerleftcamera != 0 && lowerrightcamera == 0)
					{
						area = RLrotate(-speed);	// 右回転させる
					}
					// 左下カメラと右下カメラが埋まっていた時
					else if (lowerleftcamera != 0 && lowerrightcamera != 0)
					{
						area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// ズームイン処理をさせる
						camerazoom++;
					}
				}
			}
			// カメラがズームされている状態で左下か右下が埋まっていなかったら
			if (camerazoom != 0 && (lowerleftcamera == 0 || lowerrightcamera == 0))
			{
				area = VAdd(area, VAdd(VGet(0, -0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), 1)));	// ズームアウト処理をさせる
				camerazoom--;
			}
		}
		// 回転型
		else
		{
			// 前後変更 topcameraが埋まっていなかったらさせる
			if ((KeyData::Get(KEY_INPUT_RCONTROL) == 1
				|| PadData::Get(XINPUT_BUTTON_A, 0) == 1)
				&& topcamera == 0 && moveflag == false)
			{
				area = VGet(area.x*-1, area.y, area.z*-1);
			}
			// 左に回転中 leftcameraが埋まっていなかったら処理させる
			if ((KeyData::Get(KEY_INPUT_LEFT) == 1
				|| inputleft == 1)
				&& leftcamera == 0 && moveflag == false)
			{
				rightleft = 1;
				moveflag = true;
			}
			// 右に回転中 rightcameraが埋まっていなかったら処理させる
			if ((KeyData::Get(KEY_INPUT_RIGHT) == 1
				|| inputright == 1)
				&& rightcamera == 0 && moveflag == false)
			{
				rightleft = 0;
				moveflag = true;
			}
			// 今のカメラが埋まっていたら
			if (nowcamera != 0 && moveflag == false)
			{
				// topcameraが埋まっていなかったら
				if (topcamera == 0)
				{
					// ステージ番号３（クリア疑惑エリア）じゃなかったら
					if (stagenum != 3)
					{
						// キャラクターが階段を上っていなかったら
						if (chara.y <= 0.5f)
						{
							area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// ズームイン処理をさせる
							camerazoom++;
						}
						else
						{
							area = VGet(area.x*-1, area.y, area.z*-1);	// カメラを前後反対にする
						}
					}
					else
					{
						area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// ズームイン処理をさせる
						camerazoom++;
					}
				}
				else
				{
					moveflag = true;	// カメラが回転してる最中にする

					// 左下が埋まっていなく右下が埋まっているとき
					if (lowerleftcamera == 0 && lowerrightcamera != 0)
					{
						rightleft = 1;	// 左回転として処理させる
					}
					// 左下が埋まってていて右下が埋まっていないとき
					else if (lowerleftcamera != 0 && lowerrightcamera == 0)
					{
						rightleft = 0;	// 右回転として処理する
					}
					// 左下が埋まっていて右下が埋まってるとき
					else if (lowerleftcamera != 0 && lowerrightcamera != 0)
					{
						area = VAdd(area, VAdd(VGet(0, 0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), -1)));	// ズームイン処理させる
						camerazoom++;
					}
				}
			}
			// 右回転
			if (rightleft == 0)
			{
				speed = -(DX_PI_F / 90);	// 回転向きを決める
			}
			else // 左回転
			{
				speed = DX_PI_F / 90;		// 回転向きを決める
			}
			// // 45フレーム回転させる(直して)
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
			// ズームしていて左下か右下が埋まっていなかったら
			if (camerazoom != 0 && (lowerleftcamera == 0 || lowerrightcamera == 0))
			{
				area = VAdd(area, VAdd(VGet(0, -0.2f, 0), VScale(VNorm(VGet(area.x, 0, area.z)), 1)));	// ズームアウト処理させる
				camerazoom--;
			}
		}
	}
	else if (stagenum == 4)
	{
		SetCameraNearFar(0.5f, 1500.0f);	// カメラの描画範囲を指定
											// 左に回転中
		if ((KeyData::Get(KEY_INPUT_LEFT) == 1
			|| input.Rx < 0))
		{
			area = RLrotate(speed);	// 回転処理
		}
		// 右に回転中
		if ((KeyData::Get(KEY_INPUT_RIGHT) == 1
			|| input.Rx > 0))
		{
			area = RLrotate(-speed);	// 回転処理
		}
		// 上キーが押されていたら
		if (KeyData::Get(KEY_INPUT_UP) > 0
			|| input.Ry < 0)
		{
			// 制限
			if (area.y > 10)
			{
				area = VAdd(area, VScale(VNorm(area), -1));
			}
		}

		// 下キーが押されていたら
		if (KeyData::Get(KEY_INPUT_DOWN) > 0
			|| input.Ry > 0)
		{
			// 制限
			if (area.y < 20)
			{
				area = VAdd(area, VScale(VNorm(area), 1));
			}
		}
	}
	else if(stagenum == 9)  // ステージ番号９（おまけステージ）だったら
	{
		SetCameraNearFar(1.0f, 3000.0f);	// カメラの描画範囲を指定
		// 左に回転中
		if ((KeyData::Get(KEY_INPUT_LEFT) == 1
			|| input.Rx < 0))
		{
			 area = RLrotate(speed);	// 回転処理
		}
		// 右に回転中
		if ((KeyData::Get(KEY_INPUT_RIGHT) == 1
			|| input.Rx > 0))
		{
			area = RLrotate(-speed);	// 回転処理
		}
		// 上キーが押されていたら
		if (KeyData::Get(KEY_INPUT_UP) > 0
			|| input.Ry < 0)
		{
			// 制限
			if (area.y > 10)
			{
				area = VAdd(area, VScale(VNorm(area), -1));
			}
		}

		// 下キーが押されていたら
		if (KeyData::Get(KEY_INPUT_DOWN) > 0
			|| input.Ry > 0)
		{
			// 制限
			if (area.y < 30)
			{
				area = VAdd(area, VScale(VNorm(area), 1));
			}
		}
	}

	// 第一引数の視点から第二引数のターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(VAdd(area, chara), VAdd(view, chara));

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(nowdim);
	MV1CollResultPolyDimTerminate(rightdim);
	MV1CollResultPolyDimTerminate(leftdim);
	MV1CollResultPolyDimTerminate(topdim);
	MV1CollResultPolyDimTerminate(lowerrightdim);
	MV1CollResultPolyDimTerminate(lowerleftdim);

	//// デバッグ-----------------------------------------------------------------------------------
	//DrawFormatString(50, 300, GetColor(255, 255, 255), "cameraspeed:%f", speed);
	//// -------------------------------------------------------------------------------------------
}

// ang角回転する
VECTOR Camera::RLrotate(double speed)
{
	VECTOR oxyz;
	oxyz = VGet(area.x, area.y, area.z);

	return VGet((float)(oxyz.x * cos(speed) + oxyz.z * sin(speed))
		, oxyz.y, (float)(-oxyz.x * sin(speed) + oxyz.z * cos(speed)));
}

// ゲッター
VECTOR Camera::GetXYZ()
{
	return area;
}