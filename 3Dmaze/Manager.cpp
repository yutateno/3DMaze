#include "Manager.h"

Manager::Manager()
{
	preScene = STARTLOAD;	// ���O���X�^�[�g���[�h�ɂ���
	nowScene = STARTLOAD;	// �����X�^�[�g���[�h�ɂ���

	stagenum = 0;		// �ŏ��̃X�e�[�W�ԍ����O�ɂ���
	playmodel = 0;		// �K���ɏ�����
	cameratype = 0;		// �K���ɏ�����

	scene->SetScene(STARTLOAD);

	load = new Load(0, stagenum, playmodel);	// �X�^�[�g���[�h��new����
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
		nowScene = Scene::GetScene();	// ���̃V�[������肷��
		scene->Update();				// �X�V
		// ���O�̃V�[���ŏ�����������
		if (preScene == TITLE)
		{
			playmodel = scene->GetPlayModel();		// ���f���ԍ��ƃJ�����̃��[�h���擾����Q�[�����I���܂ł͕s��
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
	// ���O�ƍ��̃V�[�����������
	if (preScene != nowScene)
	{
		Manage();				// new delete������
		preScene = nowScene;	// ���O�̃V�[�������̃V�[���ɂ���
	}
}

void Manager::Manage()
{
	if (nowScene == STARTLOAD || nowScene == LOAD)
	{
		delete load;
		delete scene;		// ����
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