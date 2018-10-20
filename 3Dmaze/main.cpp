#include "Manager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	// Direct3D9Ex���g�p����
	SetUseDirect3DVersion(DX_DIRECT3D_9EX);

	// 1920x1080 32bit
	SetGraphMode(1920, 1080, 32);

	// XAudio���g�p����
	SetEnableXAudioFlag(TRUE);

	ChangeWindowMode(TRUE);
	
	DxLib_Init();

	 // �v���O���}�u���V�F�[�_�[���f���R�D�O���g�p�ł��Ȃ��ꍇ�̓G���[��\�����ďI��
	if (GetValidShaderVersion() < 300)
	{
		// �G���[�\��
		DrawString(0, 0, "�v���O���}�u���V�F�[�_�[�R�D�O���g�p�ł��Ȃ����̂��ߏI�����܂�", GetColor(255, 255, 255));

		// �L�[���͑҂�
		WaitKey();

		// �c�w���C�u�����̌�n��
		DxLib_End();

		// �\�t�g�I��
		return 0;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	Manager* manager = new Manager();

	PadData::SetPadNum();

	// �����L�[��������邩�E�C���h�E��������܂Ń��[�v
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

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}