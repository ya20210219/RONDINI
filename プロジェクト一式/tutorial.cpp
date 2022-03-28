#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include <d3d9.h>

#include "main.h"
#include "MyDirect3D.h"
#include "sound.h"
#include <time.h>
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
#include "Select.h"

static int g_TutorialEnemyCnt;

void Tutorial_Initialize(void)
{
	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �e�̏�����
	//InitShadow();

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

	// �F�R�̏���������
	InitFriend();

	// �G�l�~�[�̏���������
	InitEnemy();

	// �A�C�e���̏�����
	InitItem();

	// �e�̏���������
	InitBullet();

	// ���b�N�I���e�̏���������
	InitLockonBullet();

	// �G�̒e�̏���������
	InitEnemyBullet();

	// UI�̏���������
	InitUI();

	// ���b�N�I���̏���������
	InitLockOn();

	// ��̏���������
	InitMeshSky(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8000.0f, 16, 8);

	// �n�ʂ̏���������
	//InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0), 100, 100, 50.0f, 50.0f);

	//���[�v�p�̏����l�ݒ�
	SetSelectPlayer(1);

	PlaySound(SOUND_LABEL_BGM002);
}

void Tutorial_Finalize(void)
{
	StopSound();

	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// ��̏I������
	UninitMeshSky();

	// �n�ʂ̏I������
	//UninitMeshField();

	// �e�̏I������
	//UninitShadow();

	//���b�N�I���̏I������
	UninitLockOn();

	// UI�̏I������
	UninitUI();

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
}


void Tutorial_Update(void)
{
	PLAYER* pPlayer = GetPlayer();

	g_TutorialEnemyCnt++;
	if (g_TutorialEnemyCnt > 180)
	{
		g_TutorialEnemyCnt = 0;
		SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 2000.0f));
	}

	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	// �󏈗��̍X�V����
	UpdateMeshSky();

	// �n�ʂ̍X�V����
	//UpdateMeshField();

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

	// �e�̍X�V����
	UpdateBullet();

	// ���b�N�I���e�̍X�V����
	UpdateLockonBullet();

	// �G�̒e�̍X�V����
	UpdateEnemyBullet();

	// UI�̍X�V����
	UpdateUI();

	// ���b�N�I���̍X�V����
	UpdateLockOn();

	// �e�̍X�V����
	//UpdateShadow();

	// �S�Ă̍X�V�������I������瓖���蔻����s��
	Collision_Bullet_vs_Boss();
	Collision_LockonBullet_vs_Enemy();
	Collision_LockonBullet_vs_Boss();
	Collision_Bullet_vs_Enemy();
	Collision_EnemyBullet_vs_Player();
	Collision_EnemyBullet_vs_Friend();
	Collision_LockOnRange_vs_Enemy();
	Collision_Enemy_vs_Player();
	Collision_Enemy_vs_Friend();
	Collision_EnemyBullet_vs_AbilityThunder();
	Collision_BossBullet_vs_AbilityThunder();
	Collision_Enemy_vs_AbilityThunder();
	Collision_AbilityBomb_vs_Enemy();
	Collision_AbilityBomb_vs_Boss();
	Collision_Item_vs_Player();


	//backspace�L�[�������ꂽ��V�[���J�ڂ���	
	if( Keyboard_IsTrigger(DIK_BACKSPACE)/* || pPlayer->life < 10*/)
	{
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			pPlayer->life = 1;
			pPlayer->bUse = true;
 			Fade_SceneTransition(SCENE_INDEX_SELECT);
			StopSound(SOUND_LABEL_BGM002);
		}
	}
}


void Tutorial_Draw(void)
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

	// UI�̕\��
	DrawUI();

	//�ꖇ�G(tutorial)
	//Sprite_Draw(TEXTURE_INDEX_TUTORIAL, 0.0f, 0.0f, 0);
}
