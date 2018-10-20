#include "Manager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	// Direct3D9Exを使用する
	SetUseDirect3DVersion(DX_DIRECT3D_9EX);

	// 1920x1080 32bit
	SetGraphMode(1920, 1080, 32);

	// XAudioを使用する
	SetEnableXAudioFlag(TRUE);

	ChangeWindowMode(TRUE);
	
	DxLib_Init();

	 // プログラマブルシェーダーモデル３．０が使用できない場合はエラーを表示して終了
	if (GetValidShaderVersion() < 300)
	{
		// エラー表示
		DrawString(0, 0, "プログラマブルシェーダー３．０が使用できない環境のため終了します", GetColor(255, 255, 255));

		// キー入力待ち
		WaitKey();

		// ＤＸライブラリの後始末
		DxLib_End();

		// ソフト終了
		return 0;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	Manager* manager = new Manager();

	PadData::SetPadNum();

	// 何かキーが押されるかウインドウが閉じられるまでループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && KeyData::CheckEnd() && PadData::CheckEnd() && !Scene::GetEndFlag())
	{
		KeyData::UpDate();
		PadData::UpDate();
		manager->Update();
	}

	InitGraph();
	InitSoundMem();
	InitShader();

	delete manager;

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}