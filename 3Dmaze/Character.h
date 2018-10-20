#pragma once
#include "DxLib.h"
#include "input.h"
#include <math.h>
#include <string>

using namespace std;

class Character
{
private:
	// それぞれの位置に関して
	VECTOR area;	// キャラ位置
	float angle;	// アングル

	// 動きに関して
	double speed;		// 回転スピード
	double walkspeed;	// 移動スピード
	double animspeed;	// モーションスピード

	// モデルのマテリアルに関して
	int materialnum;	// 輪郭線の数
	double dotwidth;	// 輪郭線の幅

	// モデルに関して
	int charamodelhandle;	// モデルのハンドル
	int playmodel;			// 操作モデル
	float modelheight;
	float modelwigth;

	// フレイム
	int framenum;		// フレイム数

	// モーションに関して
	int attachmotion;			// モーションのアタッチ
	int attachnum;				// 現在のモーション番号
	double totaltime;			// モーションの総合時間
	double nowplaytime;			// モーションの現在の時間
	double motionblendtime;		// ブレンド時間
	int preattach;				// 直前のモーションアタッチ
	double premotionplaytime;	// 直前のモーション時間

	// ステージ
	int stagenum;							// ステージの番号
	int collstagehandle;					// ステージハンドル
	int wallnum;							// 判定する壁の個数
	int floornum;							// 判定する床の個数
	bool hitflag;							// 当たっているフラッグ
	bool moveflag;							// 動いているフラッグ
	float maxY;								// 当たっている最大のy座標
	MV1_COLL_RESULT_POLY_DIM hitdim;		// プレイヤーの周囲にあるポリゴンを検出した結果が代入される当たり判定結果構造体
	MV1_COLL_RESULT_POLY* poly;				// ポリゴンの構造体にアクセスするために使用するポインタ( 使わなくても済ませられますがプログラムが長くなるので・・・ )
	MV1_COLL_RESULT_POLY* wallpoly[2048];	// 壁用のポリゴン構造体
	MV1_COLL_RESULT_POLY* floorpoly[2048];	// 床用のポリゴン構造体
	HITRESULT_LINE lineresult;				// 線分とポリゴンとの当たり判定の結果を代入する構造体
	void StageHit();						// 行う関数

	// モーフ、シェイプ、つまり表情などの動きに関わること
	float facecount;		// 移り変わりのカウント
	int facewaitcount;		// 通常の顔のを維持するカウント
	bool faceup;			// 表情の開き
	bool facedown;			// 票所の閉じ
	void FaceShape();		// 行う関数

	// コントローラー
	int inputx;		// 主に回転に関わる
	int inputy;		// 主に移動に関わる

	// シェーダー
	int charapshader;		// ピクセルシェーダ
	int charavshader;		// 頂点シェーダ

	MV1_COLL_RESULT_POLY_DIM ShadowHitResDim;
	MV1_COLL_RESULT_POLY *ShadowHitRes;
	VERTEX3D ShadowVertex[3];
	VECTOR ShadowSlideVec;
	int PLAYER_SHADOW_HEIGHT;
	float PLAYER_SHADOW_SIZE;
	int shadowhandle;
	void ShadowFoot();

	// エンディング
	float lastendcount;		// 最後のフェードアウト
	bool lastendflag;		// 最後に終える

public:
	Character(int ModelHandle, int StageHandle, int stagenum, int playmodel);	// モデルハンドルとステージハンドルとステージ番号を引数に取ったコンストラクタ
	~Character();																// デストラクタ

	void Draw();		// 描画
	void Process();		// プロセス

	void Player_PlayAnim(int attach);		// モーション変更
	void Player_AnimProcess();				// モーション動作

	// ゲッター
	VECTOR GetXYZ();	// キャラ位置
	double GetSpeed();	// モーションのスピード
	bool GetMove();		// 動いているかどうか
	// セッター
	void SetEnd(bool lastendflag);		// エンディングを終わるかどうか
};