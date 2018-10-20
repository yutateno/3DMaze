#pragma once
#include "Character.h"
#include "Camera.h"
#include "Stage.h"
#include "Scene.h"

class Game : public Scene
{
private:
	// 実体関連
	Character* character;	// キャラクター
	Camera* camera;			// カメラ
	Stage* stage;			// ステージ

	int DStageHandle;	// 描画用ステージハンドル
	int CStageHandle;	// 判定用ステージハンドル
	int ModelHandle;	// プレイヤーモデルハンドル
	int bgm;			// BGMハンドル

	// ラストフェードアウト
	int gr_white;			// 白
	int gr_black;

	// エンディング
	int gr_end;				// エンディングの背景
	bool lastendflag;		// 終わるフラッグ
	int lastendcount;		// だんだん終わる
	int gr_lastsign;

	// 音楽
	int bgm_sub;			// 同時再生音楽
	int bgm_feedout;		// フェードアウト用BGM
	int se_door;			// 扉の開
	int feedsoundcount;		// クリア疑惑エリアでBGMを移りかえる
	int bgm_bloodwindow;	// 血窓
	int silencecount;
	int silenceflag;

	// 扉
	bool dooropenflag;		// 扉が開いた

	// 足音に関して
	VECTOR listen;		// ユーザー位置
	VECTOR prelisten;	// 直前のユーザー位置
	VECTOR sePos;		// 足音の位置
	int se_right;		// 右足のサウンドハンドル
	int se_left;		// 左足のサウンドハンドル
	bool footLR;		// 0:R 1:L
	int movespeed;		// 動くスピード
	int secount;		// 待機時間
	bool moveflag;		// 動いているかどうか
	bool sestart;		// 動き出し

	// ステージ番号（ステージエンドに必要
	int stagenum;		// ステージの番号

	void Draw();		// 描画
	void Process();		// プロセス
	void Sound();		// サウンド

	// 影生成-------------------------------------------------------
	// 影用の深度値を保存する描画可能グラフィック
	int DepthBufferGraphHandle;

	// 影用の深度記録画像を作成した際のカメラのビュー行列と射影行列
	MATRIX LightCamera_ViewMatrix;
	MATRIX LightCamera_ProjectionMatrix;

	// 深度記録画像への描画用の剛体メッシュ用頂点シェーダーと
	// スキニングメッシュ用の頂点シェーダー
	int Skin4_DepthShadow_Step1_VertexShader;
	int Normal_DepthShadow_Step1_VertexShader;

	// 深度記録画像への描画用のピクセルシェーダー
	int DepthShadow_Step1_PixelShader;

	// 深度記録画像を使ったディレクショナルライト一つ付きの描画用の剛体メッシュ用頂点シェーダーと
	// スキニングメッシュ用の頂点シェーダー
	int Skin4_DirLight_DepthShadow_Step2_VertexShader;
	int Normal_DirLight_DepthShadow_Step2_VertexShader;

	// 深度記録画像を使ったディレクショナルライト一つ付きの描画用のピクセルシェーダー
	int DirLight_DepthShadow_Step2_PixelShader;

	VECTOR LightDirection;
	VECTOR LightPosition;
	VECTOR LightTarget;
	// 影生成-------------------------------------------------------

public:
	Game(int ModeHandle, int CStageHandle, int DStageHanle, int bgm, int stagenum, bool cameratype, int playmodel);	// 引数たくさん持つコンストラクタ(.cppにて説明)
	~Game();																							// デストラクタ

	void Update();	// 更新
};