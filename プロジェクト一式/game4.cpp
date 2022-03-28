#include "Game4.h"
#include "main.h"
#include "MyDirect3D.h"
#include "input.h"
#include "sound.h"
#include <time.h>
#include "scene.h"
#include "fade.h"
#include "sprite.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "bullet.h"
#include "meshfield.h"
#include "collision.h"
#include "meshsky.h"
#include "backfire.h"
#include "EnemyBullet.h"
#include "UI.h"
#include "Boss.h"
#include "Lockon.h"
#include "LockonBullet.h"
#include "AbilityThunder.h"
#include "BossBullet.h"
#include "AbilityBomb.h"
#include "Friend.h"
#include "HitEffect.h"
#include "Item.h"
#include "Score.h"
#include "BossBomb.h"


static bool Game4_EndCheck(void);
double frand4(void);


enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_PLAYER_NORMAL,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_END,

	PHASE_INDEX_MAX
};


static PHASE_INDEX g_Phase;
static int g_NextSceneFlag = 0;
static int g_Game4ClearTimer = 0;
static bool g_Directing4Flg;

void Game4_Initialize(void)
{
	// �����_���V�[�h�̏�����
	srand((unsigned int)time(NULL));

	g_Game4ClearTimer = 0;
	g_Directing4Flg = false;
	g_NextSceneFlag = 0;
	g_Phase = PHASE_INDEX_FADE;

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �e�̏�����
	InitShadow();

	// �v���C���[�̏���������
	InitPlayer();

	// �o�b�N�t�@�C�A�̏���������
	InitBackFire();

	// �q�b�g�G�t�F�N�g�̏���������
	InitHitEffect();

	// Mk2�A�r���e�B�̏�����
	InitAbilityThunder();

	// Mk3�A�r���e�B�̏�����
	InitBomb();

	// �{�X�̏�����
	InitBoss();

	// �F�R�̏�����
	InitFriend();

	// �A�C�e���̏�����
	InitItem();

	// �e�̏���������
	InitBullet();

	// ���b�N�I���e�̏���������
	InitLockonBullet();

	// �{�X�̒e�̏���������
	InitBossBullet();

	// �{�X�̔��e�̏���������
	InitBossBomb();

	// UI�̏���������
	InitUI();

	// ���b�N�I���̏���������
	InitLockOn();

	// �X�R�A�̏���������
	InitScore();

	// ��̏���������
	InitMeshSky(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8000.0f, 16, 8);


	// �n�ʂ̏���������
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0), 100, 100, 50.0f, 50.0f);
}

void Game4_Finalize(void)
{
	StopSound();

	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// ��̏I������
	UninitMeshSky();

	// �n�ʂ̏I������
	UninitMeshField();

	// �e�̏I������
	UninitShadow();

	// �X�R�A�̏I������
	UninitScore();

	//���b�N�I���̏I������
	UninitLockOn();

	// UI�̏I������
	UninitUI();

	// �{�X�̔��e�̏I������
	UninitBossBomb();

	// �{�X�̒e�̏I������
	UninitBossBullet();

	// ���b�N�I���e�̏I������
	UninitLockonBullet();

	// �e�̏I������
	UninitBullet();

	// �A�C�e���̏I������
	UninitItem();

	// �F�R�̏I������
	UninitFriend();

	// �{�X�̏I������
	UninitBoss();

	// Mk3�A�r���e�B�̏I������
	UninitBomb();

	// MK2�A�r���e�B�̏I������
	UninitAbilityThunder();

	// �q�b�g�G�t�F�N�g�̏I������
	UninitHitEffect();

	//�o�b�N�t�@�C�A�̏I������
	UninitBackFire();

	// �v���C���[�̏I������
	UninitPlayer();

	g_Game4ClearTimer = 0;
}

void Game4_Update(void)
{
	switch (g_Phase)
	{
	case PHASE_INDEX_FADE:
		//�t�F�[�h�̏I����҂�
		if (Fade_GetState() == FADE_STATE_NONE) {
			g_Phase = PHASE_INDEX_PLAYER_NORMAL;
			PlaySound(SOUND_LABEL_BGM001);
		}
		break;
	case PHASE_INDEX_PLAYER_NORMAL:

		if (g_Directing4Flg == false)
		{
			g_Game4ClearTimer++;
			if (g_Game4ClearTimer > 480)
			{
				g_Game4ClearTimer = 0;
				g_Directing4Flg = true;
			}
		}

		// �J�����̍X�V����
		UpdateCamera();

		// ���C�g�̍X�V����
		UpdateLight();

		// �󏈗��̍X�V����
		UpdateMeshSky();

		// �n�ʂ̍X�V����
		UpdateMeshField();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �o�b�N�t�@�C�A�̍X�V����
		UpdateBackFire();

		// MK2�A�r���e�B�̍X�V����
		UpdateAbilityThunder();

		// Mk3�A�r���e�B�̍X�V����
		UpdateBomb();

		// �F�R�̍X�V����
		UpdateFriend();

		// �q�b�g�G�t�F�N�g�̍X�V����
		UpdateHitEffect();

		// �A�C�e���̍X�V����
		UpdateItem();

		// �{�X�̍X�V����
		UpdateBoss();

		// �e�̍X�V����
		UpdateBullet();

		// ���b�N�I���e�̍X�V����
		UpdateLockonBullet();

		// �{�X�̒e�̍X�V����
		UpdateBossBullet();

		// �{�X�̔��e�̍X�V����
		UpdateBossBomb();

		// UI�̍X�V����
		UpdateUI();

		// ���b�N�I���̍X�V����
		UpdateLockOn();

		// �X�R�A�̍X�V����
		UpdateScore();

		// �e�̍X�V����
		UpdateShadow();

		// �S�Ă̍X�V�������I������瓖���蔻����s��
		Collision_Bullet_vs_Boss();
		Collision_LockonBullet_vs_Boss();
		Collision_BossBullet_vs_Player();
		Collision_BossBullet_vs_Friend();
		Collision_BossBomb_vs_Player();
		Collision_BossBomb_vs_Friend();
		Collision_Boss_vs_Player();
		Collision_Boss_vs_Friend();
		Collision_BossBullet_vs_AbilityThunder();
		Collision_AbilityBomb_vs_Boss();
		Collision_Item_vs_Player();

		if (Player_IsEnable() == false)
		{
			g_NextSceneFlag = 1;
		}

		if (Boss_AllDestroy() == true)
		{
			g_NextSceneFlag = 1;
		}

		//�Q�[���̏I���`�F�b�N
		if (Game4_EndCheck())
		{
			g_Directing4Flg = true;
			g_Game4ClearTimer++;
			if (g_Game4ClearTimer == 300)
			{
				StopSound(SOUND_LABEL_BGM001);
				PlaySound(SOUND_LABEL_SE_CLEAR);
			}

			if (g_Game4ClearTimer > 480)
			{
				Fade_SceneTransition(SCENE_INDEX_ENDING);
				g_Phase = PHASE_INDEX_STAGE_CLEAR;
			}

		}

		if (Game4_EndCheck() && Player_IsEnable() == false)
		{
			Fade_SceneTransition(SCENE_INDEX_RESULT);
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}

		break;
	case PHASE_INDEX_STAGE_CLEAR:
		g_Phase = PHASE_INDEX_END;
		break;
	case PHASE_INDEX_END:
		break;
	}
}

void Game4_Draw(void)
{
	// �J�����̐ݒ�
	SetCamera();

	// �󏈗��̕`�揈��
	DrawMeshSky();

	// �n�ʂ̕`�揈��
//	DrawMeshField();

	// �e�̕`�揈��
	//DrawShadow();

	// �{�X�̒e�̕`�揈��
	DrawBossBullet();

	// �{�X�̔��e�̕`�揈��
	DrawBossBomb();

	// ���b�N�I���e�̕`�揈��
	DrawLockonBullet();

	// �e�̕`�揈��
	DrawBullet();

	// �A�C�e���̕`�揈��
	DrawItem();

	// �F�R�̕`�揈��
	DrawFriend();

	// �{�X�̕`�揈��
	DrawBoss();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �o�b�N�t�@�C�A�̕`�揈��
	DrawBackFire();

	// �q�b�g�G�t�F�N�g�̕`�揈��
	DrawHitEffect();

	// MK2�A�r���e�B�̕`�揈��
	DrawAbilityThunder();

	// Mk3�A�r���e�B�̕`�揈��
	DrawBomb();

	//���b�N�I���̕`�揈��
	DrawLockOn();

	// �X�R�A�̕`�揈��
	DrawScore();

	// UI�̕\��
	DrawUI();
}


bool Game4_EndCheck(void)
{
	if (g_NextSceneFlag != 0)
		return true;

	return false;
}

double frand4(void)
{
	return (double)rand() / RAND_MAX;
}

int GetGame4CleatTimer(void)
{
	return g_Game4ClearTimer;
}

bool GetGame4DirectingFlg()
{
	return g_Directing4Flg;
}