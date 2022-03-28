//=============================================================================
//
// エネミー管理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "enemy.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define MAX_ENEMY		(100)				// エネミーの数
#define MAX_ENEMY_TYPE   (4)				//エネミーの種類の数

struct ENEMY
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	float				NowRotY;
	float               TargetRotY;
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	D3DXVECTOR3         spd;
	float				g_Dist;
	int					life;
	int					type;
	float				PlusSpd;

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

	D3DXVECTOR3			base_pos;			// ベースとなる位置

	int					idxEnemyShadow;

	bool				bUse;
	bool				LockEnable;
	D3DXVECTOR3         EnemyBulletBirthPos;		//弾の生成位置
	int					EnemyBulletTimer;

	SPHERE  col_Sphere;
};

//*****************************************************************************
// プロトタイプ宣言
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
