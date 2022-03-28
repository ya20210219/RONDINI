//=============================================================================
//
// エネミー管理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "MyDirect3D.h"
#include "shadow.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "EnemyBullet.h"
#include "radian.h"
#include "Item.h"
#include "scene.h"
#include "Score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY1		"data/MODEL/Rondini_Enemy_type01.x"	
#define	MODEL_ENEMY2		"data/MODEL/Rondini_Enemy_type02.x"	
#define	MODEL_ENEMY3		"data/MODEL/Rondini_Enemy_type03.x"	
#define	MODEL_ENEMY4		"data/MODEL/Rondini_Enemy_type04.x"	

#define	VALUE_ENEMY_ROTATE	(D3DX_PI * 0.008f)		// 回転量
#define MAX_ENEMY_LIFE		(15)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPD3DXMESH			g_pMesh[MAX_ENEMY_TYPE] = { NULL };		// メッシュ情報へのポインタ
static LPD3DXBUFFER			g_pBuffMat[MAX_ENEMY_TYPE] = { NULL };	// マテリアル情報へのポインタ
static DWORD				g_nNumMat[MAX_ENEMY_TYPE] = { 0 };		// マテリアル情報の数
static LPDIRECT3DTEXTURE9	g_pTexture = NULL;	// テクスチャ


static ENEMY				g_Enemy[MAX_ENEMY];	// エネミー

static D3DXVECTOR3			g_Target;
static int					g_EnemyEnableCnt = MAX_ENEMY;	//存在している敵のカウント
static bool					g_EnemyAllDestroy = false;			//全滅フラグ
static float				g_rotY1;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	PLAYER *pPlayer = GetPlayer();

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY1,	// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
		pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
		NULL,									// 隣接性データを含むバッファへのポインタ
		&g_pBuffMat[0],							// マテリアルデータを含むバッファへのポインタ
		NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat[0],								// D3DXMATERIAL構造体の数
		&g_pMesh[0])))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY2,	// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
		pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
		NULL,									// 隣接性データを含むバッファへのポインタ
		&g_pBuffMat[1],							// マテリアルデータを含むバッファへのポインタ
		NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat[1],								// D3DXMATERIAL構造体の数
		&g_pMesh[1])))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY3,	// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
		pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
		NULL,									// 隣接性データを含むバッファへのポインタ
		&g_pBuffMat[2],							// マテリアルデータを含むバッファへのポインタ
		NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat[2],								// D3DXMATERIAL構造体の数
		&g_pMesh[2])))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY4,	// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
		pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
		NULL,									// 隣接性データを含むバッファへのポインタ
		&g_pBuffMat[3],							// マテリアルデータを含むバッファへのポインタ
		NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat[3],								// D3DXMATERIAL構造体の数
		&g_pMesh[3])))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}


	D3DXMATERIAL *pMat;

	for (int i = 0; i < MAX_ENEMY_TYPE; i++)
	{
		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMat[i]->GetBufferPointer();

		// マテリアルにテクスチャが登録されていれば読み込む
		if (pMat[i].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
				pMat[i].pTextureFilename,			// ファイルの名前
				&g_pTexture);						// 読み込むメモリー
		}
	}


	float interval = 150.0f;

	//ステージ1の配置====================================================================================================================================================================================
	if (GetSceneNumber() == SCENE_INDEX_GAME)	
	{
		for (int i = 0; i < 15; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(15 - 1) + (interval * i), 10.0f, (1500.0f/* - (100 * i)*/));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE;
			g_Enemy[i].type = 0;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 15.0f;
		}

		for (int i = 15; i < 20; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(5 - 1) + (interval * (i - 15)), 10.0f, (1200.0f/* - (100 * i)*/));

			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE - 5;
			g_Enemy[i].type = 1;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 15.0f;
		}

		for (int i = 20; i < 24; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(4 - 1) + (interval * (i - 20)), 10.0f, (2200.0f/* - (100 * i)*/));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE * 2;
			g_Enemy[i].type = 2;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 15.0f;
		}
	}

	//ステージ2の配置====================================================================================================================================================================================
	if (GetSceneNumber() == SCENE_INDEX_GAME2)	
	{
		interval = 75.0f;
		for (int i = 0; i < 20; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(20 - 1) + (interval * i) - 1000, 10.0f, (1500.0f + (100 * i)));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE;
			g_Enemy[i].type = 0;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 15.0f;
		}

		for (int i = 20; i < 40; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(20 - 1) + (interval * (i - 20)) + 1000, 10.0f, (2500.0f - (100 * (i - 20))));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE;
			g_Enemy[i].type = 0;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 15.0f;
		}

		interval = 150.0f;

		for (int i = 40; i < 48; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(8 - 1) + (interval * (i - 40)), 10.0f, (2200.0f/* - (100 * i)*/));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE * 2;
			g_Enemy[i].type = 2;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 15.0f;
		}

		for (int i = 48; i < 60; i++)
		{
			interval = 300.0f;

			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(12 - 1) + (interval * (i - 48)), 10.0f, (2500.0f/* - (100 * i)*/));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE + 10;
			g_Enemy[i].type = 3;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 18.0f;
		}
	}

	//ステージ3の配置====================================================================================================================================================================================
	if (GetSceneNumber() == SCENE_INDEX_GAME3)
	{
		interval = 100.0f;

		for (int i = 0; i < 30; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(30 - 1) + (interval * (i % 4)) - 500.0f, 10.0f, (1500.0f + (200.0f * (i / 4))));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, (-D3DX_PI / 2), 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE + 10;
			g_Enemy[i].type = 3;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 18.0f;
		}

		for (int i = 30; i < 60; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(30 - 1) + (interval * ((i - 30) % 4)) + 2000.0f, 10.0f, (1500.0f + (100.0f * ((i - 30) / 4))));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, (D3DX_PI / 2), 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE + 10;
			g_Enemy[i].type = 3;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 18.0f;
		}

		for (int i = 60; i < 70; i++)
		{
			interval = 200.0f;

			// 位置・回転・スケールの初期設定
			g_Enemy[i].pos = D3DXVECTOR3(-(interval / 2)*(8 - 1) + (interval * (i - 60)), 10.0f, (3000.0f - (100 * ((i - 60) % 2))));
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE * 2;
			g_Enemy[i].type = 2;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;

			g_Enemy[i].col_Sphere.r = 15.0f;
		}
	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].bUse == true)
		{
			if (g_Enemy[i].type == 0)
			{
				g_Enemy[i].PlusSpd = 0.8f;
			}
			if (g_Enemy[i].type == 1)
			{
				g_Enemy[i].PlusSpd = 1.2f;
			}
			if (g_Enemy[i].type == 2)
			{
				g_Enemy[i].PlusSpd = 0.7f;
			}
			if (g_Enemy[i].type == 3)
			{
				g_Enemy[i].PlusSpd = 0.8f;
			}
			g_Enemy[i].idxEnemyShadow = CreateShadow(g_Enemy[i].pos, 30.0f, 30.0f);
		}
	}

	g_EnemyAllDestroy = false;

	g_rotY1 = 0.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_pTexture != NULL)
	{// テクスチャの開放
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	for (int i = 0; i < MAX_ENEMY_TYPE; i++)
	{
		if (g_pMesh != NULL)
		{// メッシュの開放
			g_pMesh[i]->Release();
			g_pMesh[i] = NULL;
		}

		if (g_pBuffMat != NULL)
		{// マテリアルの開放
			g_pBuffMat[i]->Release();
			g_pBuffMat[i] = NULL;
		}
	}

	for (int j = 0; j < MAX_ENEMY; j++)
	{
		g_Enemy[j].bUse = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	if (GetSceneNumber() == SCENE_INDEX_TUTORIAL || GetSceneNumber() == SCENE_INDEX_GAME ||
		 GetSceneNumber() == SCENE_INDEX_GAME2 || GetSceneNumber() == SCENE_INDEX_GAME3 ||
		GetSceneNumber() == SCENE_INDEX_GAME4)
	{
		PLAYER *pPlayer = GetPlayer();
		CAMERA *cam = GetCamera();

		g_EnemyEnableCnt = MAX_ENEMY;

		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (g_Enemy[i].life < 1 && g_Enemy[i].bUse == true)
			{
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
				Enemy_Destroy(i);
				//g_Enemy[i].bUse = false;
			}

			if (!g_Enemy[i].bUse)
			{
				g_EnemyEnableCnt--;
				continue;
			}

			g_Enemy[i].g_Dist = GetFloatDist(Enemy_GetSphere(i), Player_GetSphere());

			g_Target = pPlayer->pos;


			if (g_Enemy[i].rot.y < 0)
			{
				g_Enemy[i].rot.y += D3DX_PI * 2.0f;
			}

			if (g_Enemy[i].rot.y > D3DX_PI * 2)
			{
				g_Enemy[i].rot.y -= D3DX_PI * 2.0f;
			}

			g_Enemy[i].NowRotY = g_Enemy[i].rot.y;

			g_Enemy[i].TargetRotY = (-atan2f((g_Target.z - g_Enemy[i].pos.z), (g_Target.x - g_Enemy[i].pos.x)) - (D3DX_PI / 2));

			if (g_Enemy[i].TargetRotY < 0)
			{
				g_Enemy[i].TargetRotY += D3DX_PI * 2.0f;
			}

			if (g_Enemy[i].TargetRotY > D3DX_PI * 2.0f)
			{
				g_Enemy[i].TargetRotY -= D3DX_PI * 2.0f;
			}

			g_rotY1 = (float)LerpRadian(g_Enemy[i].NowRotY, g_Enemy[i].TargetRotY, 0.02f);	//0.2で8

			g_Enemy[i].rot.y = g_rotY1;

			g_Enemy[i].spd.x = sinf(D3DX_PI * 0.0f - g_Enemy[i].rot.y) * g_Enemy[i].PlusSpd;
			g_Enemy[i].spd.z = cosf(D3DX_PI * 0.0f - g_Enemy[i].rot.y) * g_Enemy[i].PlusSpd;

			//g_Enemy[i].spd = D3DXVECTOR3 (0.0f, 0.0f, 0.0f);

			g_Enemy[i].pos.x += g_Enemy[i].spd.x;
			g_Enemy[i].pos.z -= g_Enemy[i].spd.z;

			g_Enemy[i].EnemyBulletTimer++;

			if (g_Enemy[i].EnemyBulletTimer == 180)
			{
				g_Enemy[i].EnemyBulletBirthPos.x = g_Enemy[i].pos.x - sinf(g_Enemy[i].rot.y) * 18.0f;
				g_Enemy[i].EnemyBulletBirthPos.z = g_Enemy[i].pos.z - cosf(g_Enemy[i].rot.y) * 18.0f;

				if (g_Enemy[i].g_Dist < 700)
				{
					if (g_Enemy[i].type != 3)
					{
						SetEnemyBullet(g_Enemy[i].EnemyBulletBirthPos, D3DXVECTOR3(-sinf(g_Enemy[i].rot.y) * 4.0f, 0.0f, -cosf(g_Enemy[i].rot.y) * 4.0f));
					}
					if (g_Enemy[i].type == 3)
					{
						g_Enemy[i].EnemyBulletBirthPos.x = g_Enemy[i].pos.x - sinf(g_Enemy[i].rot.y - 0.2f) * 18.0f;
						g_Enemy[i].EnemyBulletBirthPos.z = g_Enemy[i].pos.z - cosf(g_Enemy[i].rot.y - 0.2f) * 18.0f;
						SetEnemyBullet(g_Enemy[i].EnemyBulletBirthPos, D3DXVECTOR3(-sinf(g_Enemy[i].rot.y) * 4.0f, 0.0f, -cosf(g_Enemy[i].rot.y) * 4.0f));
						g_Enemy[i].EnemyBulletBirthPos.x = g_Enemy[i].pos.x - sinf(g_Enemy[i].rot.y + 0.4f) * 18.0f;
						g_Enemy[i].EnemyBulletBirthPos.z = g_Enemy[i].pos.z - cosf(g_Enemy[i].rot.y + 0.4f) * 18.0f;
						SetEnemyBullet(g_Enemy[i].EnemyBulletBirthPos, D3DXVECTOR3(-sinf(g_Enemy[i].rot.y) * 4.0f, 0.0f, -cosf(g_Enemy[i].rot.y) * 4.0f));
					}
				}

				g_Enemy[i].EnemyBulletTimer = 0;
			}

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;
			g_Enemy[i].col_Sphere.r = 15.0f;

			SetPositionShadow(g_Enemy[i].idxEnemyShadow, D3DXVECTOR3(g_Enemy[i].pos.x, 0.0f, g_Enemy[i].pos.z));
		}

		if (g_EnemyEnableCnt == 0)
		{
			g_EnemyAllDestroy = true;
		}
		if (g_EnemyEnableCnt != 0)
		{
			g_EnemyAllDestroy = false;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!g_Enemy[i].bUse)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy[i].mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].rot.y, g_Enemy[i].rot.x, g_Enemy[i].rot.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].mtxWorld);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMat[g_Enemy[i].type]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMat[g_Enemy[i].type]; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定(テクスチャの有るモデルを読み込むときは注意)
			pDevice->SetTexture(0, g_pTexture);

			// 描画
			g_pMesh[g_Enemy[i].type]->DrawSubset(nCntMat);
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY* GetEnemy(int index)
{
	return &(g_Enemy[index]);
}

void SetEnemy(D3DXVECTOR3 pos)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!g_Enemy[i].bUse)
		{
			g_Enemy[i].pos = pos;
			g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].bUse = true;

			g_Enemy[i].EnemyBulletTimer = 0;

			g_Enemy[i].life = MAX_ENEMY_LIFE;
			g_Enemy[i].type = 0;
			g_Enemy[i].PlusSpd = 0.8f;

			g_Enemy[i].col_Sphere.pos.x = g_Enemy[i].pos.x;
			g_Enemy[i].col_Sphere.pos.y = g_Enemy[i].pos.y;
			g_Enemy[i].col_Sphere.pos.z = g_Enemy[i].pos.z;
			g_Enemy[i].col_Sphere.r = 15.0f;

			g_Enemy[i].NowRotY = 0.0f;
			g_Enemy[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Enemy[i].base_pos = g_Enemy[i].pos;

			g_Enemy[i].bUse = true;
			g_Enemy[i].LockEnable = false;

			break;
		}
	}
}

ENEMY* GetAllEnemy()
{
	return g_Enemy;
}

SPHERE* Enemy_GetSphere(int index)
{
	return (&g_Enemy[index].col_Sphere);
}

void Enemy_Destroy(int index)
{
	PlaySound(SOUND_LABEL_SE_EXPLOSION);
	if (g_Enemy[index].type == 0 || g_Enemy[index].type	== 1)
	{
		AddScore(50);
	}

	if (g_Enemy[index].type == 2 || g_Enemy[index].type == 3)
	{
		AddScore(75);
	}
	g_Enemy[index].bUse = false;
	g_Enemy[index].LockEnable = false;
	SetItem(g_Enemy[index].pos);
	ReleaseShadow(g_Enemy[index].idxEnemyShadow);
}

bool Enemy_IsEnable(int index)
{
	return g_Enemy[index].bUse;
}

bool EnemyLockOn_IsEnable(int index)
{
	return g_Enemy[index].LockEnable;
}

void SetEnemyLockOn_IsEnable(int index, bool enable)
{
	g_Enemy[index].LockEnable = enable;
}

void Enemy_Damage(int index, int damage)
{
	g_Enemy[index].life -= damage;
	if (g_Enemy[index].life >= 1)
	{
		PlaySound(SOUND_LABEL_SE_DAMAGE);
	}
}

bool Enemy_AllDestroy()
{
	return g_EnemyAllDestroy;
}