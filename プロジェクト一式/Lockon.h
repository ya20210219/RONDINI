//=============================================================================
//
// ロックオン機能処理 [Target.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

#define	MAX_LOCKON			(3)							// ロックオン最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		posL;		// 位置
	D3DXVECTOR3		rotL;		// 回転
	D3DXVECTOR3		scaleL;		// スケール
	bool			bUse;		// 使用しているかどうかのフラグ
	int				TargetNum;	//対象のナンバー
} LOCKON;

typedef struct
{
	D3DXVECTOR3		posR;		// 位置
	D3DXVECTOR3		rotR;		// 回転
	D3DXVECTOR3		scaleR;		// スケール
	bool			bUse;		// 使用しているかどうかのフラグ

	int				LockonCoolDownCnt;
	bool			LockonCoolDownFlg;
	int				UnLockonCnt;
	bool			LockonFreeFlg;

	SPHERE				col_Sphere;

} LOCKONRANGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLockOn(void);
void UninitLockOn(void);
void UpdateLockOn(void);
void DrawLockOn(void);

LOCKON* GetLockOn();
LOCKONRANGE* GetLockonRange();

void SetLockOn(int index, D3DXVECTOR3 pos, bool enable);
SPHERE* LockOnRange_GetSphere();

bool SetLockOnEnable(bool enable);
bool LockOnRange_IsEnable();
void LockOn_Destroy(int index);
bool LockOn_IsEnable(int index);
