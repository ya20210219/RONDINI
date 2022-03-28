#include "Game.h"
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


static bool Game_EndCheck(void);
double frand(void);


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
static int g_Game1ClearTimer = 0;
static bool g_Directing1Flg = false;

void Game_Initialize(void)
{
	// �����_���V�[�h�̏�����
	srand((unsigned int)time(NULL));

	g_Directing1Flg = false;
	g_Game1ClearTimer = 0;
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

	// �G�l�~�[�̏���������
	InitEnemy();

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

	// �G�̒e�̏���������
	InitEnemyBullet();

	// �{�X�̒e�̏���������
	InitBossBullet();

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

void Game_Finalize(void)
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

	// �{�X�̒e�̏I������
	UninitBossBullet();

	// �G�̒e�̏I������
	UninitEnemyBullet();

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

	// �G�l�~�[�̏I������
	UninitEnemy();

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

	g_Game1ClearTimer = 0;
}

void Game_Update(void)
{
	switch (g_Phase)
	{
	case PHASE_INDEX_FADE:
		//�t�F�[�h�̏I����҂�
		if (Fade_GetState()==FADE_STATE_NONE) {
			g_Phase = PHASE_INDEX_PLAYER_NORMAL;
			PlaySound(SOUND_LABEL_BGM000);
		}
		break;
	case PHASE_INDEX_PLAYER_NORMAL:

		if (g_Directing1Flg == false)
		{
			g_Game1ClearTimer++;
			if (g_Game1ClearTimer > 480)
			{
				g_Game1ClearTimer = 0;
				g_Directing1Flg = true;
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

		// �G�l�~�[�̍X�V����
		UpdateEnemy();

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

		// �G�̒e�̍X�V����
		UpdateEnemyBullet();

		// �{�X�̒e�̍X�V����
		UpdateBossBullet();

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
		Collision_LockonBullet_vs_Enemy();
		Collision_LockonBullet_vs_Boss();
		Collision_Bullet_vs_Enemy();
		Collision_EnemyBullet_vs_Player();
		Collision_EnemyBullet_vs_Friend();
		Collision_BossBullet_vs_Player();
		Collision_BossBullet_vs_Friend();
		Collision_BossBomb_vs_Player();
		Collision_BossBomb_vs_Friend();
		Collision_LockOnRange_vs_Enemy();
		Collision_Enemy_vs_Player();
		Collision_Enemy_vs_Friend();
		Collision_Boss_vs_Player();
		Collision_Boss_vs_Friend();
		Collision_EnemyBullet_vs_AbilityThunder();
		Collision_BossBullet_vs_AbilityThunder();
		Collision_Enemy_vs_AbilityThunder();
		Collision_AbilityBomb_vs_Enemy();
		Collision_AbilityBomb_vs_Boss();
		Collision_Item_vs_Player();
		
		if (Player_IsEnable() == false)
		{
			g_NextSceneFlag = 1;
		}

		if (Enemy_AllDestroy() == true && Boss_AllDestroy() == true)
		{
			g_NextSceneFlag = 1;
		}

		/*
		//�Q�[���̏I���`�F�b�N
		if (Game_EndCheck())
		{
			Fade_SceneTransition(SCENE_INDEX_SELECT);
			AddClearCnt();
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}
		*/
		if (Game_EndCheck())
		{
			g_Directing1Flg = true;
			g_Game1ClearTimer++;
			if (g_Game1ClearTimer == 300)
			{
				StopSound(SOUND_LABEL_BGM000);
				PlaySound(SOUND_LABEL_SE_CLEAR);
			} 
			
			if (g_Game1ClearTimer > 480)	//8�b
			{
				Fade_SceneTransition(SCENE_INDEX_SELECT);
				//Fade_SceneTransition(SCENE_INDEX_ENDING);
				AddClearCnt();
				g_Phase = PHASE_INDEX_STAGE_CLEAR;
			}
		}

		if (Game_EndCheck() && Player_IsEnable() == false)
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

void Game_Draw(void)
{
	// �J�����̐ݒ�
	SetCamera();

	// �󏈗��̕`�揈��
	DrawMeshSky();

	// �n�ʂ̕`�揈��
//	DrawMeshField();

	// �e�̕`�揈��
	//DrawShadow();

	// �G�̒e�̕`�揈��
	DrawEnemyBullet();

	// �{�X�̒e�̕`�揈��
	DrawBossBullet();

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

	// �G�l�~�[�̕`�揈��
	DrawEnemy();

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


bool Game_EndCheck(void)
{
	if (g_NextSceneFlag != 0)
		return true;

	return false;
}

double frand(void)
{
	return (double)rand() / RAND_MAX;
}

int GetGame1CleatTimer(void)
{
	return g_Game1ClearTimer;
}

bool GetGame1DirectingFlg()
{
	return g_Directing1Flg;
}