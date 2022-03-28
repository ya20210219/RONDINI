//=============================================================================
//
// Mk2アビリティ処理 [AbilityThunder.h]
// Author : Y.Ariyoshi
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

#define	MAX_THUNDERANIME			(10)

typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		scale;		// スケール
	int				timer;		// 生存フレーム
	int				countW;		//カウント
	int				countH;		//カウント
	bool			bUse;		// 使用しているかどうかのフラグ

	bool			PlusThunder;
	int				PlusThunderUseCnt;
	int				PlusThunderCnt;
	bool            PlusThunderCoolDownFlg;

	SPHERE          col_Sphere;

} AbilityThunder;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitAbilityThunder(void);
void UninitAbilityThunder(void);
void UpdateAbilityThunder(void);
void DrawAbilityThunder(void);

AbilityThunder* GetAbilityThunder();
SPHERE* GetAbilityThunderSphere();
void AbilityThunder_Destroy();
bool AbilityThunder_IsEnable();
