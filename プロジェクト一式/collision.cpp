#include "collision.h"
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "EnemyBullet.h"
#include "Boss.h"
#include "Lockon.h"
#include "LockonBullet.h"
#include "AbilityThunder.h"
#include "BossBullet.h"
#include "AbilityBomb.h"
#include "Friend.h"
#include "sound.h"
#include "HitEffect.h"
#include "Item.h"
#include "BossBomb.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//bool HitCheckSphere(SPHERE* pA, SPHERE* pB);
//void Collision_Bullet_vs_Enemy(void);
//void Collision_Bullet_vs_Boss(void);
//void Collision_LockOnRange_vs_Enemy(void);

//=============================================================================
// 弾と敵の当たり判定
//=============================================================================
void Collision_Bullet_vs_Enemy()
{
	PLAYER* pPlayer = GetPlayer();
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Bullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Enemy_IsEnable(j))
				continue;

			if (HitCheckSphere(Bullet_GetSphere(i), Enemy_GetSphere(j)))
			{
				//ヒットしている
				if (pPlayer->Pattern != 2)
				{
					Enemy_Damage(j, 3);
				}
				if (pPlayer->Pattern == 2)
				{
					Enemy_Damage(j, 2);
				}

				SetHitEffect(GetEnemy(j)->pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				Bullet_Destroy(i);
			}
		}
	}
}

void Collision_LockonBullet_vs_Enemy()
{
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!LockonBullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Enemy_IsEnable(j))
				continue;

			if (HitCheckSphere(LockonBullet_GetSphere(i), Enemy_GetSphere(j)))
			{
				//ヒットしている
				Enemy_Damage(j, 15);
				SetHitEffect(GetEnemy(j)->pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
				LockonBullet_Destroy(i);
			}
		}
	}
}

void Collision_LockonBullet_vs_Boss()
{
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!LockonBullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_BOSS; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Boss_IsEnable(j))
				continue;

			if (HitCheckSphere(LockonBullet_GetSphere(i), Boss_GetSphere(j)))
			{
				//ヒットしている
				Boss_Damage(j, 3);
				LockonBullet_Destroy(i);
			}
		}
	}
}

void Collision_AbilityBomb_vs_Enemy()
{
	for (int i = 0; i < MAX_BOMB; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Bomb_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Enemy_IsEnable(j))
				continue;

			if (HitCheckSphere(Bomb_GetSphere(i), Enemy_GetSphere(j)))
			{
				//ヒットしている
				Enemy_Destroy(j);
				Bomb_LifeDown(i);
				SetHitEffect(GetEnemy(j)->pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
			}
		}
	}
}

void Collision_Bullet_vs_Boss()
{
	PLAYER* pPlayer = GetPlayer();
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Bullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_BOSS; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Boss_IsEnable(j))
				continue;

			if (HitCheckSphere(Bullet_GetSphere(i), Boss_GetSphere(j)))
			{
				//ヒットしている
				if (pPlayer->Pattern != 2)
				{
					Boss_Damage(j, 3);
				}
				if (pPlayer->Pattern == 2)
				{
					Boss_Damage(j, 2);
				}


				Bullet_Destroy(i);
			}
		}
	}
}

void Collision_AbilityBomb_vs_Boss()
{
	for (int i = 0; i < MAX_BOMB; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Bomb_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_BOSS; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Boss_IsEnable(j))
				continue;

			if (HitCheckSphere(Bomb_GetSphere(i), Boss_GetSphere(j)))
			{
				//ヒットしている
				Boss_Damage(j, 6);
				//Boss_Damage(j, 1000);
				Bomb_Destroy(i);
			}
		}
	}
}

void Collision_EnemyBullet_vs_Player()
{
	AbilityThunder* pAT = GetAbilityThunder();
	
	if (pAT->PlusThunder != true)
	{
		for (int i = 0; i < MAX_ENEMY_BULLET; i++)
		{
			//弾の有効フラグチェック(trueでない場合)
			if (!EnemyBullet_IsEnable(i))
				continue;

			//敵の有効フラグチェック(trueでない場合)
			if (!Player_IsEnable())
				continue;

			if (HitCheckSphere(EnemyBullet_GetSphere(i), Player_GetSphere()))
			{
				//ヒットしている
				Player_Damage(1);
				EnemyBullet_Destroy(i);
			}

		}
	}
}

void Collision_EnemyBullet_vs_Friend()
{
	for (int i = 0; i < MAX_ENEMY_BULLET; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!EnemyBullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(EnemyBullet_GetSphere(i), Friend_GetSphere(j)))
			{
				//ヒットしている
				Friend_Damage(j);
				EnemyBullet_Destroy(i);
			}
		}
	}	
}

void Collision_BossBullet_vs_Player()
{
	AbilityThunder* pAT = GetAbilityThunder();

	if (pAT->PlusThunder != true)
	{
		for (int i = 0; i < MAX_BOSS_BULLET; i++)
		{
			//弾の有効フラグチェック(trueでない場合)
			if (!BossBullet_IsEnable(i))
				continue;

			//敵の有効フラグチェック(trueでない場合)
			if (!Player_IsEnable())
				continue;

			if (HitCheckSphere(BossBullet_GetSphere(i), Player_GetSphere()))
			{
				//ヒットしている
				Player_Damage(1);
				BossBullet_Destroy(i);
			}

		}
	}
}

void Collision_BossBomb_vs_Player()
{
	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!BossBomb_IsEnable(i))
			continue;

		//敵の有効フラグチェック(trueでない場合)
		if (!Player_IsEnable())
			continue;

		if (HitCheckSphere(BossBomb_GetSphere(i), Player_GetSphere()))
		{
			//ヒットしている
			Player_Damage(5);
			BossBomb_Destroy(i);
		}

	}	
}

void Collision_BossBomb_vs_Friend()
{
	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!BossBomb_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(BossBomb_GetSphere(i), Friend_GetSphere(j)))
			{
				//ヒットしている
				Friend_SelectDamage(j, 5);
				BossBomb_Destroy(i);
			}
		}

	}
}

void Collision_BossBullet_vs_Friend()
{
	for (int i = 0; i < MAX_BOSS_BULLET; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!BossBullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(BossBullet_GetSphere(i), Friend_GetSphere(j)))
			{
				//ヒットしている
				Friend_Damage(j);
				BossBullet_Destroy(i);
			}
		}
	}
}

void Collision_Enemy_vs_Player()
{
	//AbilityThunder* pAT = GetAbilityThunder();

	//if (pAT->PlusThunder != true)
	//{
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			//弾の有効フラグチェック(trueでない場合)
			if (!Enemy_IsEnable(i))
				continue;

			//敵の有効フラグチェック(trueでない場合)
			if (!Player_IsEnable())
				continue;

			if (HitCheckSphere(Enemy_GetSphere(i), Player_GetSphere()))
			{
				//ヒットしている
				Player_Damage(2);
				PlaySound(SOUND_LABEL_SE_DAMAGE);
				Enemy_Damage(i, 6);

				SetHitEffect(GetEnemy(i)->pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			}

		}
	//}
}

void Collision_Boss_vs_Player()
{
	for (int i = 0; i < MAX_BOSS; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Boss_IsEnable(i))
			continue;

		//敵の有効フラグチェック(trueでない場合)
		if (!Player_IsEnable())
			continue;

		if (HitCheckSphere(Boss_GetSphere(i), Player_GetSphere()))
		{
			//ヒットしている
			Player_Damage(1);
			PlaySound(SOUND_LABEL_SE_DAMAGE);
			Boss_Damage(i, 1);
		}
	}
}


void Collision_Item_vs_Player()
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Item_IsEnable(i))
			continue;

		//敵の有効フラグチェック(trueでない場合)
		if (!Player_IsEnable())
			continue;

		if (HitCheckSphere(Item_GetSphere(i), Player_GetSphere()))
		{
			//ヒットしている
			Player_AddLife(5);
			Item_Destroy(i);
		}
	}
}

void Collision_Enemy_vs_Friend()
{
	/*
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Enemy_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(Enemy_GetSphere(i), Friend_GetSphere(j)))
			{
				//ヒットしている
				Friend_Destroy(j);
				Enemy_Destroy(i);
				SetHitEffect(GetEnemy(j)->pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
			}
		}
	}
	*/
}

void Collision_Boss_vs_Friend()
{
	for (int i = 0; i < MAX_BOSS; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Boss_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//敵の有効フラグチェック(trueでない場合)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(Boss_GetSphere(i), Friend_GetSphere(j)))
			{
				//ヒットしている
				Friend_Destroy(j);
			}
		}
	}
}

void Collision_Enemy_vs_AbilityThunder()
{
	PLAYER* pPlayer = GetPlayer();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!Enemy_IsEnable(i))
			continue;

		//敵の有効フラグチェック(trueでない場合)
		if (!AbilityThunder_IsEnable())
			continue;

		if (HitCheckSphere(Enemy_GetSphere(i), GetAbilityThunderSphere()))
		{
			//ヒットしている
			PlaySound(SOUND_LABEL_SE_DAMAGE);

			//ヒットしている
			if (pPlayer->Pattern != 2)
			{
				Enemy_Damage(i, 3);
			}
			if (pPlayer->Pattern == 2)
			{
				Enemy_Damage(i, 2);
			}

			SetHitEffect(GetEnemy(i)->pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}	
}

void Collision_EnemyBullet_vs_AbilityThunder()
{
	for (int i = 0; i < MAX_ENEMY_BULLET; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!EnemyBullet_IsEnable(i))
			continue;

		//敵の有効フラグチェック(trueでない場合)
		if (!AbilityThunder_IsEnable())
			continue;

		if (HitCheckSphere(EnemyBullet_GetSphere(i), GetAbilityThunderSphere()))
		{
			//ヒットしている
			EnemyBullet_Destroy(i);
		}
	}	
}

void Collision_BossBullet_vs_AbilityThunder()
{
	for (int i = 0; i < MAX_BOSS_BULLET; i++)
	{
		//弾の有効フラグチェック(trueでない場合)
		if (!BossBullet_IsEnable(i))
			continue;

		//敵の有効フラグチェック(trueでない場合)
		if (!AbilityThunder_IsEnable())
			continue;

		if (HitCheckSphere(BossBullet_GetSphere(i), GetAbilityThunderSphere()))
		{
			//ヒットしている
			BossBullet_Destroy(i);
		}

	}

}

//サーチと敵の当たり判定(ロックオン用)
void Collision_LockOnRange_vs_Enemy()
{
	LOCKON* pLockon = GetLockOn();

	if (LockOnRange_IsEnable() == true)
	{
		for (int i = 0; i < MAX_LOCKON; i++)
		{
			//弾の有効フラグチェック(falseでない場合)
			if (LockOn_IsEnable(i))
				continue;

			for (int j = 0; j < MAX_ENEMY; j++)
			{
				//敵の有効フラグチェック(trueでない場合)
				if (!Enemy_IsEnable(j) || EnemyLockOn_IsEnable(j))
					continue;

				if (HitCheckSphere(LockOnRange_GetSphere(), Enemy_GetSphere(j)))
				{
					SetLockOn(i, GetEnemy(j)->pos, true);
					SetEnemyLockOn_IsEnable(j, true);
					(pLockon + i)->TargetNum = j;
					PlaySound(SOUND_LABEL_SE_LOCKON);

					break;
				}
			}
		}
	}
}

//=============================================================================
// 円と円の当たり判定
//=============================================================================
bool HitCheckSphere(SPHERE* pA, SPHERE* pB)
{
	D3DXVECTOR3 dist;

	//BからAに向かうベクトルを作る
	dist = pA->pos - pB->pos;

	//ベクトルdistの大きさを計算する
	float length = D3DXVec3Length(&dist);
	//pAの半径とpBの半径を足した値を計算する
	float size = pA->r + pB->r;

	//lengthとsizeの比較をしてsizeのほうが大きければ
	//ヒットしている
	if (length < size)
		return true;

	//ヒットしてない
	return false;
}

float GetFloatDist(SPHERE* cA, SPHERE* cB)
{
	//ベクトルを計算する
	D3DXVECTOR3 dist;	//距離

	//BからAに向かうベクトルを計算する
	dist = cA->pos - cB->pos;

	//ベクトルdistの大きさを計算する
	float length = D3DXVec3Length(&dist);

	return length;
}

