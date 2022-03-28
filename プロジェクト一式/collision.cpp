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
// �v���g�^�C�v�錾
//*****************************************************************************
//bool HitCheckSphere(SPHERE* pA, SPHERE* pB);
//void Collision_Bullet_vs_Enemy(void);
//void Collision_Bullet_vs_Boss(void);
//void Collision_LockOnRange_vs_Enemy(void);

//=============================================================================
// �e�ƓG�̓����蔻��
//=============================================================================
void Collision_Bullet_vs_Enemy()
{
	PLAYER* pPlayer = GetPlayer();
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Bullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Enemy_IsEnable(j))
				continue;

			if (HitCheckSphere(Bullet_GetSphere(i), Enemy_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!LockonBullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Enemy_IsEnable(j))
				continue;

			if (HitCheckSphere(LockonBullet_GetSphere(i), Enemy_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!LockonBullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_BOSS; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Boss_IsEnable(j))
				continue;

			if (HitCheckSphere(LockonBullet_GetSphere(i), Boss_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Bomb_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Enemy_IsEnable(j))
				continue;

			if (HitCheckSphere(Bomb_GetSphere(i), Enemy_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Bullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_BOSS; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Boss_IsEnable(j))
				continue;

			if (HitCheckSphere(Bullet_GetSphere(i), Boss_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Bomb_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_BOSS; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Boss_IsEnable(j))
				continue;

			if (HitCheckSphere(Bomb_GetSphere(i), Boss_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
			//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!EnemyBullet_IsEnable(i))
				continue;

			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Player_IsEnable())
				continue;

			if (HitCheckSphere(EnemyBullet_GetSphere(i), Player_GetSphere()))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!EnemyBullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(EnemyBullet_GetSphere(i), Friend_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
			//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!BossBullet_IsEnable(i))
				continue;

			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Player_IsEnable())
				continue;

			if (HitCheckSphere(BossBullet_GetSphere(i), Player_GetSphere()))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!BossBomb_IsEnable(i))
			continue;

		//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Player_IsEnable())
			continue;

		if (HitCheckSphere(BossBomb_GetSphere(i), Player_GetSphere()))
		{
			//�q�b�g���Ă���
			Player_Damage(5);
			BossBomb_Destroy(i);
		}

	}	
}

void Collision_BossBomb_vs_Friend()
{
	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!BossBomb_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(BossBomb_GetSphere(i), Friend_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!BossBullet_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(BossBullet_GetSphere(i), Friend_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
			//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Enemy_IsEnable(i))
				continue;

			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Player_IsEnable())
				continue;

			if (HitCheckSphere(Enemy_GetSphere(i), Player_GetSphere()))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Boss_IsEnable(i))
			continue;

		//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Player_IsEnable())
			continue;

		if (HitCheckSphere(Boss_GetSphere(i), Player_GetSphere()))
		{
			//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Item_IsEnable(i))
			continue;

		//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Player_IsEnable())
			continue;

		if (HitCheckSphere(Item_GetSphere(i), Player_GetSphere()))
		{
			//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Enemy_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(Enemy_GetSphere(i), Friend_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Boss_IsEnable(i))
			continue;

		for (int j = 0; j < MAX_FRIEND; j++)
		{
			//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
			if (!Friend_IsEnable(j))
				continue;

			if (HitCheckSphere(Boss_GetSphere(i), Friend_GetSphere(j)))
			{
				//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!Enemy_IsEnable(i))
			continue;

		//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!AbilityThunder_IsEnable())
			continue;

		if (HitCheckSphere(Enemy_GetSphere(i), GetAbilityThunderSphere()))
		{
			//�q�b�g���Ă���
			PlaySound(SOUND_LABEL_SE_DAMAGE);

			//�q�b�g���Ă���
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
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!EnemyBullet_IsEnable(i))
			continue;

		//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!AbilityThunder_IsEnable())
			continue;

		if (HitCheckSphere(EnemyBullet_GetSphere(i), GetAbilityThunderSphere()))
		{
			//�q�b�g���Ă���
			EnemyBullet_Destroy(i);
		}
	}	
}

void Collision_BossBullet_vs_AbilityThunder()
{
	for (int i = 0; i < MAX_BOSS_BULLET; i++)
	{
		//�e�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!BossBullet_IsEnable(i))
			continue;

		//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
		if (!AbilityThunder_IsEnable())
			continue;

		if (HitCheckSphere(BossBullet_GetSphere(i), GetAbilityThunderSphere()))
		{
			//�q�b�g���Ă���
			BossBullet_Destroy(i);
		}

	}

}

//�T�[�`�ƓG�̓����蔻��(���b�N�I���p)
void Collision_LockOnRange_vs_Enemy()
{
	LOCKON* pLockon = GetLockOn();

	if (LockOnRange_IsEnable() == true)
	{
		for (int i = 0; i < MAX_LOCKON; i++)
		{
			//�e�̗L���t���O�`�F�b�N(false�łȂ��ꍇ)
			if (LockOn_IsEnable(i))
				continue;

			for (int j = 0; j < MAX_ENEMY; j++)
			{
				//�G�̗L���t���O�`�F�b�N(true�łȂ��ꍇ)
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
// �~�Ɖ~�̓����蔻��
//=============================================================================
bool HitCheckSphere(SPHERE* pA, SPHERE* pB)
{
	D3DXVECTOR3 dist;

	//B����A�Ɍ������x�N�g�������
	dist = pA->pos - pB->pos;

	//�x�N�g��dist�̑傫�����v�Z����
	float length = D3DXVec3Length(&dist);
	//pA�̔��a��pB�̔��a�𑫂����l���v�Z����
	float size = pA->r + pB->r;

	//length��size�̔�r������size�̂ق����傫�����
	//�q�b�g���Ă���
	if (length < size)
		return true;

	//�q�b�g���ĂȂ�
	return false;
}

float GetFloatDist(SPHERE* cA, SPHERE* cB)
{
	//�x�N�g�����v�Z����
	D3DXVECTOR3 dist;	//����

	//B����A�Ɍ������x�N�g�����v�Z����
	dist = cA->pos - cB->pos;

	//�x�N�g��dist�̑傫�����v�Z����
	float length = D3DXVec3Length(&dist);

	return length;
}

