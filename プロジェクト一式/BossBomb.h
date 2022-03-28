//=============================================================================
//
// 弾処理 [BossBomb.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "Lockon.h"

#define	MAX_BOSS_BOMB			(1)							// 弾最大数	初期値128

typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 回転
	D3DXVECTOR3		scale;		// スケール
	D3DXVECTOR3		dir;		// 移動量
	int				countW;
	int				countH;
	float			PlayerY;
	int				timer;		// 生存フレーム
	int				idxShadow;	// 影ID
	bool			bUse;		// 使用しているかどうかのフラグ

	SPHERE			col_Sphere;

} ENEMYBOMB;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBossBomb(void);
void UninitBossBomb(void);
void UpdateBossBomb(void);
void DrawBossBomb(void);

int SetBossBomb(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
SPHERE* BossBomb_GetSphere(int index);

void BossBomb_Destroy(int index);
bool BossBomb_IsEnable(int index);
