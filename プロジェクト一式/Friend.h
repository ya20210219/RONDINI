//=============================================================================
//
// 友軍管理 [Friend.h]
// Author : Y.Ariyoshi
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define MAX_FRIEND		(4)				// エネミーの数

struct FRIEND
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	float				rotP;
	float				NowRotY;
	float               TargetRotY;
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	D3DXVECTOR3         spd;
	float				g_Dist;
	int					life;
	float				FriendSpd;

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

	D3DXVECTOR3			base_pos;			// ベースとなる位置

	D3DXVECTOR3			Target;
	int					TargetNum;

	int					idxFriendShadow;

	bool				bUse;
	bool				LockEnable;
	D3DXVECTOR3         FriendBulletBirthPos;		//弾の生成位置
	int					FriendBulletTimer;

	SPHERE  col_Sphere;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFriend(void);
void UninitFriend(void);
void UpdateFriend(void);
void DrawFriend(void);

FRIEND* GetFriend(int index);
FRIEND* GetAllFriend();
SPHERE* Friend_GetSphere(int index);
void Friend_Destroy(int index);
bool Friend_IsEnable(int index);
bool FriendLockOn_IsEnable(int index);

void SetFriendLockOn_IsEnable(int index, bool enable);
void Friend_Damage(int index);
void Friend_SelectDamage(int index, int damage);
bool Friend_AllDestroy();

