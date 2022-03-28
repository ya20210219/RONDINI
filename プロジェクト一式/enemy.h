//=============================================================================
//
// �G�l�~�[�Ǘ� [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "enemy.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MAX_ENEMY		(100)				// �G�l�~�[�̐�
#define MAX_ENEMY_TYPE   (4)				//�G�l�~�[�̎�ނ̐�

struct ENEMY
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	float				NowRotY;
	float               TargetRotY;
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3         spd;
	float				g_Dist;
	int					life;
	int					type;
	float				PlusSpd;

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	D3DXVECTOR3			base_pos;			// �x�[�X�ƂȂ�ʒu

	int					idxEnemyShadow;

	bool				bUse;
	bool				LockEnable;
	D3DXVECTOR3         EnemyBulletBirthPos;		//�e�̐����ʒu
	int					EnemyBulletTimer;

	SPHERE  col_Sphere;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY* GetEnemy(int index);
void SetEnemy(D3DXVECTOR3 pos);
ENEMY* GetAllEnemy();
SPHERE* Enemy_GetSphere(int index);
void Enemy_Destroy(int index);
bool Enemy_IsEnable(int index);
bool EnemyLockOn_IsEnable(int index);

void SetEnemyLockOn_IsEnable(int index, bool enable);
void Enemy_Damage(int index, int damage);
bool Enemy_AllDestroy();
