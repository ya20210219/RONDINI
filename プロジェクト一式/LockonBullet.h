//=============================================================================
//
// ロックオン弾処理 [bullet.h]
// Author : Y.Ariyoshi
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"


typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 回転
	D3DXVECTOR3		scale;		// スケール
	D3DXVECTOR3		dir;		// 移動量
	int				timer;		// 生存フレーム
	int				idxShadow;	// 影ID
	float			TargetRotY;
	bool			bUse;		// 使用しているかどうかのフラグ
	bool			LockonUse;

	SPHERE			col_Sphere;

} LOCKONBULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLockonBullet(void);
void UninitLockonBullet(void);
void UpdateLockonBullet(void);
void DrawLockonBullet(void);

LOCKONBULLET* GetLockonBullet();
int SetLockonBullet(D3DXVECTOR3 pos, float y);
SPHERE* LockonBullet_GetSphere(int index);

void LockonBullet_Destroy(int index);
bool LockonBullet_IsEnable(int index);
