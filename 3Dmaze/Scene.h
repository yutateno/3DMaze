#pragma once
#include "DxLib.h"
#include "input.h"

// �V�[��
enum eScene
{
	STARTLOAD, TITLE, LOAD, GAME
};

class Scene
{
protected:
	static bool endflag;	// �I���t���b�O

	static eScene scene;	// ���݂̃V�[��

	int playmodel;		// �v���C���[���f���̔ԍ�
	bool cameratype;	// �J�������[�h�̔ԍ�

	// �t�F�[�h�A�E�g�t�F�[�h�C���p
	bool start;		// �J�n�t���b�O
	bool end;		// �I���t���b�O
	int count;		// �t�F�[�h�C���t�F�[�h�A�E�g�p�J�E���g�ϐ�

public:
	Scene();					// �R���X�g���N�^
	virtual ~Scene() {};		// �f�X�g���N�^

	virtual void Update() = 0;	// �X�V

	static bool GetEndFlag();	// �I���Q�b�^�[
	static eScene GetScene();	// ���̃V�[���Q�b�^�[

	// �Z�b�^�[
	void SetScene(eScene scene);
	// �Q�b�^�[
	int GetPlayModel();		// �v���C���f���ԍ�
	bool GetCameraType();	// �J�����^�C�v
};