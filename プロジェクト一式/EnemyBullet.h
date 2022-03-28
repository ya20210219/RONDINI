//=============================================================================
//
// �G�̒e���� [Enemybullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "enemy.h"

#define	MAX_ENEMY_BULLET			(50 * MAX_ENEMY)							// �e�ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);

int SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
SPHERE* EnemyBullet_GetSphere(int index);

void EnemyBullet_Destroy(int index);
bool EnemyBullet_IsEnable(int index);
