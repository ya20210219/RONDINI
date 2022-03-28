//=============================================================================
//
// ボス管理 [Boss.h]
// Author : Y.Ariyoshi
//
//=============================================================================
#pragma once

#include "main.h"
#include "Boss.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define MAX_BOSS		(1)				// エネミーの数

struct BOSS
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	float				NowRotY;
	float               TargetRotY;
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	D3DXVECTOR3         spd;
	int					Life;
	int					MaxLife;
	int					HpColor;
	float				g_Dist;
	int					type;

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

	D3DXVECTOR3			base_pos;			// ベースとなる位置

	int					idxBossShadow;

	bool				bUse;
	D3DXVECTOR3         BossBulletBirthPos;		//弾の生成位置
	int					BossBulletTimer;

	SPHERE  col_Sphere;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

BOSS* GetBoss();
BOSS* GetAllBoss();
SPHERE* Boss_GetSphere(int index);
void Boss_Destroy(int index);
void Boss_Damage(int index, int damage);
bool Boss_IsEnable(int index);
bool Boss_AllDestroy();
