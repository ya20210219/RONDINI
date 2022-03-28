//=============================================================================
//
// 当たり判定管理 [collision.h]
// Author :
//
//=============================================================================
#pragma once


#include <d3dx9.h>


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float		r;		//半径

}SPHERE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void UpdateCollision(void);
void Collision_Bullet_vs_Enemy();
void Collision_LockonBullet_vs_Enemy();
void Collision_LockonBullet_vs_Boss();
void Collision_Bullet_vs_Boss();
void Collision_AbilityBomb_vs_Boss();
void Collision_EnemyBullet_vs_Player();
void Collision_EnemyBullet_vs_Friend();
void Collision_BossBullet_vs_Player();
void Collision_BossBullet_vs_Friend();
void Collision_BossBomb_vs_Player();
void Collision_BossBomb_vs_Friend();
void Collision_Enemy_vs_Player();
void Collision_Boss_vs_Player();
void Collision_Boss_vs_Friend();
void Collision_Enemy_vs_Friend();
void Collision_LockOnRange_vs_Enemy();
void Collision_AbilityBomb_vs_Enemy();
void Collision_EnemyBullet_vs_AbilityThunder();
void Collision_BossBullet_vs_AbilityThunder();
void Collision_Enemy_vs_AbilityThunder();
void Collision_Item_vs_Player();
bool HitCheckSphere(SPHERE* cA, SPHERE* cB);
float GetFloatDist(SPHERE* cA, SPHERE* cB);
