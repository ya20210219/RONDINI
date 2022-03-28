//=============================================================================
//
// プレイヤー管理 [player.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

#define MAX_PLAYER		(3)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYER
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	D3DXVECTOR3			speed;				// 移動スピード
	int					life;				//ライフ

	D3DXVECTOR3         BulletBirthPos;		//弾の生成位置

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス


	float				PlayerSpd;
	int					idxShadow;			// 影の識別番号
	bool				bUse;
	int					HpColor;
	int					Pattern;

	SPHERE				col_Sphere;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);
SPHERE* Player_GetSphere(void);
void Player_Destroy(void);
void Player_Damage(int damage);
void Player_AddLife(int life);
bool Player_IsEnable(void);
