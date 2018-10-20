#pragma once
#include "DxLib.h"
#include "input.h"
#include <math.h>

class Camera
{
private:
	// それぞれの位置に関して
	VECTOR area;		// カメラの位置
	VECTOR view;		// 注視する方向
	VECTOR chara;		// キャラ位置

	// ステージに関して
	int collstagehandle;	// ステージハンドル
	int stagenum;			// ステージの番号

	// 八方向のカメラのステージのあたり判定に関して
	int nowcamera;							// 今のカメラ
	int rightcamera;						// 右
	int leftcamera;							// 左
	int topcamera;							// 前後反対
	int lowerrightcamera;					// 右下
	int lowerleftcamera;					// 左下
	MV1_COLL_RESULT_POLY_DIM nowdim;		// 今のカメラのコリジョン判定
	MV1_COLL_RESULT_POLY_DIM rightdim;		// 右
	MV1_COLL_RESULT_POLY_DIM leftdim;		// 左
	MV1_COLL_RESULT_POLY_DIM topdim;		// 前後反対
	MV1_COLL_RESULT_POLY_DIM lowerrightdim;	// 右下
	MV1_COLL_RESULT_POLY_DIM lowerleftdim;	// 左下
	double hitturn;							// 四方向のカメラの位置決定用
	double lowerturn;						// 八方向のlowerカメラの位置決定用
	double upperturn;						// 八方向のupperカメラの位置決定用
	int moveframe;

	// コントローラー
	DINPUT_JOYSTATE input;		// 主に回転に関わる
	int inputright;
	int inputleft;

	// カメラのズームや回転の動きに関して
	double speed;					// 回転スピード
	int camerazoom;					// どのくらいズームしたかどうか
	bool cameratype;				// 0:回転型か 1:切り替え型か
	bool rightleft;					// 0:右回転か 1:左回転か
	bool moveflag;					// カメラ動き中かどうか
	VECTOR RLrotate(double speed);	// 回転を行う関数

public:
	Camera(VECTOR xyz, int stagenum, int StageHandle, bool cameratype);		// キャラの位置とステージ番号とステージハンドルとカメラタイプを引数に取ったコンストラクタ
	~Camera();																// デストラクタ

	void Process(VECTOR xyz);		// キャラの位置を引数に取ったプロセス

	// ゲッター
	VECTOR GetXYZ();				// カメラの位置
};