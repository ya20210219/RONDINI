#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include <d3d9.h>

#include "main.h"
#include "MyDirect3D.h"
#include "sound.h"
#include <time.h>
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
#include "Select.h"

static int g_TutorialEnemyCnt;

void Tutorial_Initialize(void)
{
	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 影の初期化
	//InitShadow();

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

	// 友軍の初期化処理
	InitFriend();

	// エネミーの初期化処理
	InitEnemy();

	// アイテムの初期化
	InitItem();

	// 弾の初期化処理
	InitBullet();

	// ロックオン弾の初期化処理
	InitLockonBullet();

	// 敵の弾の初期化処理
	InitEnemyBullet();

	// UIの初期化処理
	InitUI();

	// ロックオンの初期化処理
	InitLockOn();

	// 空の初期化処理
	InitMeshSky(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8000.0f, 16, 8);

	// 地面の初期化処理
	//InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0), 100, 100, 50.0f, 50.0f);

	//ループ用の初期値設定
	SetSelectPlayer(1);

	PlaySound(SOUND_LABEL_BGM002);
}

void Tutorial_Finalize(void)
{
	StopSound();

	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// 空の終了処理
	UninitMeshSky();

	// 地面の終了処理
	//UninitMeshField();

	// 影の終了処理
	//UninitShadow();

	//ロックオンの終了処理
	UninitLockOn();

	// UIの終了処理
	UninitUI();

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
}


void Tutorial_Update(void)
{
	PLAYER* pPlayer = GetPlayer();

	g_TutorialEnemyCnt++;
	if (g_TutorialEnemyCnt > 180)
	{
		g_TutorialEnemyCnt = 0;
		SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 2000.0f));
	}

	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	// 空処理の更新処理
	UpdateMeshSky();

	// 地面の更新処理
	//UpdateMeshField();

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

	// 弾の更新処理
	UpdateBullet();

	// ロックオン弾の更新処理
	UpdateLockonBullet();

	// 敵の弾の更新処理
	UpdateEnemyBullet();

	// UIの更新処理
	UpdateUI();

	// ロックオンの更新処理
	UpdateLockOn();

	// 影の更新処理
	//UpdateShadow();

	// 全ての更新処理が終わったら当たり判定を行う
	Collision_Bullet_vs_Boss();
	Collision_LockonBullet_vs_Enemy();
	Collision_LockonBullet_vs_Boss();
	Collision_Bullet_vs_Enemy();
	Collision_EnemyBullet_vs_Player();
	Collision_EnemyBullet_vs_Friend();
	Collision_LockOnRange_vs_Enemy();
	Collision_Enemy_vs_Player();
	Collision_Enemy_vs_Friend();
	Collision_EnemyBullet_vs_AbilityThunder();
	Collision_BossBullet_vs_AbilityThunder();
	Collision_Enemy_vs_AbilityThunder();
	Collision_AbilityBomb_vs_Enemy();
	Collision_AbilityBomb_vs_Boss();
	Collision_Item_vs_Player();


	//backspaceキーが押されたらシーン遷移する	
	if( Keyboard_IsTrigger(DIK_BACKSPACE)/* || pPlayer->life < 10*/)
	{
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			pPlayer->life = 1;
			pPlayer->bUse = true;
 			Fade_SceneTransition(SCENE_INDEX_SELECT);
			StopSound(SOUND_LABEL_BGM002);
		}
	}
}


void Tutorial_Draw(void)
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

	// UIの表示
	DrawUI();

	//一枚絵(tutorial)
	//Sprite_Draw(TEXTURE_INDEX_TUTORIAL, 0.0f, 0.0f, 0);
}
