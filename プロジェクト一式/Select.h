//=============================================================================
//
// プレイヤーセレクトシーン管理 [Select.cpp]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct SELECT
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	int					life;
	int					spd;
	int					atk;

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

	int					idxShadow;			// 影の識別番号
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);
int SelectPlayer(void);
void SetSelectPlayer(int index);
SELECT* GetSelect();

