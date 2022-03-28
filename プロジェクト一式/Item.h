//=============================================================================
//
// エネミー管理 [Item.h]
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

#define MAX_ITEM		(MAX_ENEMY)				// エネミーの数

struct ITEM
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	int					type;

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

	int					idxItemShadow;

	bool				bUse;

	SPHERE  col_Sphere;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

ITEM* GetItem(int index);
ITEM* GetAllItem();
SPHERE* Item_GetSphere(int index);
void Item_Destroy(int index);
bool Item_IsEnable(int index);
void SetItem(D3DXVECTOR3 pos);
bool Item_AllDestroy();
