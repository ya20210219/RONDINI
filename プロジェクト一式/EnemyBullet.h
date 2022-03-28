//=============================================================================
//
// 敵の弾処理 [Enemybullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "enemy.h"

#define	MAX_ENEMY_BULLET			(50 * MAX_ENEMY)							// 弾最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);

int SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
SPHERE* EnemyBullet_GetSphere(int index);

void EnemyBullet_Destroy(int index);
bool EnemyBullet_IsEnable(int index);
