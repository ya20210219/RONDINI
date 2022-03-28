#include "Game.h"
#include "main.h"
#include "MyDirect3D.h"
#include "input.h"
#include "sound.h"
#include <time.h>
#include "scene.h"
#include "fade.h"
#include "sprite.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "bullet.h"
#include "meshfield.h"
#include "collision.h"
#include "meshsky.h"
#include "backfire.h"
#include "EnemyBullet.h"
#include "UI.h"
#include "Boss.h"
#include "Lockon.h"
#include "LockonBullet.h"
#include "AbilityThunder.h"
#include "BossBullet.h"
#include "AbilityBomb.h"
#include "Friend.h"
#include "HitEffect.h"
#include "Item.h"
#include "Score.h"


static bool Game_EndCheck(void);
double frand(void);


enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_PLAYER_NORMAL,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_END,

	PHASE_INDEX_MAX
};


static PHASE_INDEX g_Phase;
static int g_NextSceneFlag = 0;
static int g_Game1ClearTimer = 0;
static bool g_Directing1Flg = false;

void Game_Initialize(void)
{
	// ランダムシードの初期化
	srand((unsigned int)time(NULL));

	g_Directing1Flg = false;
	g_Game1ClearTimer = 0;
	g_NextSceneFlag = 0;
	g_Phase = PHASE_INDEX_FADE;

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 影の初期化
	InitShadow();

	// プレイヤーの初期化処理
	InitPlayer();

	// バックファイアの初期化処理
	InitBackFire();

	// ヒットエフェクトの初期化処理
	InitHitEffect();

	// Mk2アビリティの初期化
	InitAbilityThunder();

	// Mk3アビリティの初期化
	InitBomb();

	// エネミーの初期化処理
	InitEnemy();

	// ボスの初期化
	InitBoss();

	// 友軍の初期化
	InitFriend();

	// アイテムの初期化
	InitItem();

	// 弾の初期化処理
	InitBullet();

	// ロックオン弾の初期化処理
	InitLockonBullet();

	// 敵の弾の初期化処理
	InitEnemyBullet();

	// ボスの弾の初期化処理
	InitBossBullet();

	// UIの初期化処理
	InitUI();

	// ロックオンの初期化処理
	InitLockOn();

	// スコアの初期化処理
	InitScore();

	// 空の初期化処理
	InitMeshSky(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8000.0f, 16, 8);


	// 地面の初期化処理
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0), 100, 100, 50.0f, 50.0f);
}

void Game_Finalize(void)
{
	StopSound();

	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// 空の終了処理
	UninitMeshSky();

	// 地面の終了処理
	UninitMeshField();

	// 影の終了処理
	UninitShadow();

	// スコアの終了処理
	UninitScore();

	//ロックオンの終了処理
	UninitLockOn();

	// UIの終了処理
	UninitUI();

	// ボスの弾の終了処理
	UninitBossBullet();

	// 敵の弾の終了処理
	UninitEnemyBullet();

	// ロックオン弾の終了処理
	UninitLockonBullet();

	// 弾の終了処理
	UninitBullet();

	// アイテムの終了処理
	UninitItem();

	// 友軍の終了処理
	UninitFriend();

	// ボスの終了処理
	UninitBoss();

	// エネミーの終了処理
	UninitEnemy();

	// Mk3アビリティの終了処理
	UninitBomb();

	// MK2アビリティの終了処理
	UninitAbilityThunder();

	// ヒットエフェクトの終了処理
	UninitHitEffect();

	//バックファイアの終了処理
	UninitBackFire();

	// プレイヤーの終了処理
	UninitPlayer();

	g_Game1ClearTimer = 0;
}

void Game_Update(void)
{
	switch (g_Phase)
	{
	case PHASE_INDEX_FADE:
		//フェードの終了を待つ
		if (Fade_GetState()==FADE_STATE_NONE) {
			g_Phase = PHASE_INDEX_PLAYER_NORMAL;
			PlaySound(SOUND_LABEL_BGM000);
		}
		break;
	case PHASE_INDEX_PLAYER_NORMAL:

		if (g_Directing1Flg == false)
		{
			g_Game1ClearTimer++;
			if (g_Game1ClearTimer > 480)
			{
				g_Game1ClearTimer = 0;
				g_Directing1Flg = true;
			}
		}

		// カメラの更新処理
		UpdateCamera();

		// ライトの更新処理
		UpdateLight();

		// 空処理の更新処理
		UpdateMeshSky();

		// 地面の更新処理
		UpdateMeshField();

		// プレイヤーの更新処理
		UpdatePlayer();

		// バックファイアの更新処理
		UpdateBackFire();

		// MK2アビリティの更新処理
		UpdateAbilityThunder();

		// Mk3アビリティの更新処理
		UpdateBomb();

		// 友軍の更新処理
		UpdateFriend();

		// エネミーの更新処理
		UpdateEnemy();

		// ヒットエフェクトの更新処理
		UpdateHitEffect();

		// アイテムの更新処理
		UpdateItem();

		// ボスの更新処理
		UpdateBoss();

		// 弾の更新処理
		UpdateBullet();

		// ロックオン弾の更新処理
		UpdateLockonBullet();

		// 敵の弾の更新処理
		UpdateEnemyBullet();

		// ボスの弾の更新処理
		UpdateBossBullet();

		// UIの更新処理
		UpdateUI();

		// ロックオンの更新処理
		UpdateLockOn();

		// スコアの更新処理
		UpdateScore();

		// 影の更新処理
		UpdateShadow();

		// 全ての更新処理が終わったら当たり判定を行う
		Collision_Bullet_vs_Boss();
		Collision_LockonBullet_vs_Enemy();
		Collision_LockonBullet_vs_Boss();
		Collision_Bullet_vs_Enemy();
		Collision_EnemyBullet_vs_Player();
		Collision_EnemyBullet_vs_Friend();
		Collision_BossBullet_vs_Player();
		Collision_BossBullet_vs_Friend();
		Collision_BossBomb_vs_Player();
		Collision_BossBomb_vs_Friend();
		Collision_LockOnRange_vs_Enemy();
		Collision_Enemy_vs_Player();
		Collision_Enemy_vs_Friend();
		Collision_Boss_vs_Player();
		Collision_Boss_vs_Friend();
		Collision_EnemyBullet_vs_AbilityThunder();
		Collision_BossBullet_vs_AbilityThunder();
		Collision_Enemy_vs_AbilityThunder();
		Collision_AbilityBomb_vs_Enemy();
		Collision_AbilityBomb_vs_Boss();
		Collision_Item_vs_Player();
		
		if (Player_IsEnable() == false)
		{
			g_NextSceneFlag = 1;
		}

		if (Enemy_AllDestroy() == true && Boss_AllDestroy() == true)
		{
			g_NextSceneFlag = 1;
		}

		/*
		//ゲームの終了チェック
		if (Game_EndCheck())
		{
			Fade_SceneTransition(SCENE_INDEX_SELECT);
			AddClearCnt();
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}
		*/
		if (Game_EndCheck())
		{
			g_Directing1Flg = true;
			g_Game1ClearTimer++;
			if (g_Game1ClearTimer == 300)
			{
				StopSound(SOUND_LABEL_BGM000);
				PlaySound(SOUND_LABEL_SE_CLEAR);
			} 
			
			if (g_Game1ClearTimer > 480)	//8秒
			{
				Fade_SceneTransition(SCENE_INDEX_SELECT);
				//Fade_SceneTransition(SCENE_INDEX_ENDING);
				AddClearCnt();
				g_Phase = PHASE_INDEX_STAGE_CLEAR;
			}
		}

		if (Game_EndCheck() && Player_IsEnable() == false)
		{
			Fade_SceneTransition(SCENE_INDEX_RESULT);
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}
		break;
	case PHASE_INDEX_STAGE_CLEAR:
		g_Phase = PHASE_INDEX_END;
		break;
	case PHASE_INDEX_END:
		break;
	}
}

void Game_Draw(void)
{
	// カメラの設定
	SetCamera();

	// 空処理の描画処理
	DrawMeshSky();

	// 地面の描画処理
//	DrawMeshField();

	// 影の描画処理
	//DrawShadow();

	// 敵の弾の描画処理
	DrawEnemyBullet();

	// ボスの弾の描画処理
	DrawBossBullet();

	// ロックオン弾の描画処理
	DrawLockonBullet();

	// 弾の描画処理
	DrawBullet();

	// アイテムの描画処理
	DrawItem();

	// 友軍の描画処理
	DrawFriend();

	// ボスの描画処理
	DrawBoss();

	// エネミーの描画処理
	DrawEnemy();

	// プレイヤーの描画処理
	DrawPlayer();

	// バックファイアの描画処理
	DrawBackFire();

	// ヒットエフェクトの描画処理
	DrawHitEffect();

	// MK2アビリティの描画処理
	DrawAbilityThunder();

	// Mk3アビリティの描画処理
	DrawBomb();

	//ロックオンの描画処理
	DrawLockOn();

	// スコアの描画処理
	DrawScore();

	// UIの表示
	DrawUI();
}


bool Game_EndCheck(void)
{
	if (g_NextSceneFlag != 0)
		return true;

	return false;
}

double frand(void)
{
	return (double)rand() / RAND_MAX;
}

int GetGame1CleatTimer(void)
{
	return g_Game1ClearTimer;
}

bool GetGame1DirectingFlg()
{
	return g_Directing1Flg;
}