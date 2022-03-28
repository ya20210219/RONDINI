//=============================================================================
//
// エネミー管理 [Boss.cpp]
// Author : 
//
//=============================================================================
#include "Boss.h"
#include "MyDirect3D.h"
#include "shadow.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "BossBullet.h"
#include "radian.h"
#include "scene.h"
#include "Score.h"
#include "BossBomb.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_Boss1			"data/MODEL/Rondini_BossBattleShip_type05.x"
#define	MODEL_Boss2			"data/MODEL/Rondini_BossBattleShip_type06.x"
#define	MODEL_LastBoss		"data/MODEL/Rondini_LastBossBattleShip_Ver02.x"
//#define	MODEL_Boss		"data/MODEL/Rondini_Boss_type03.x"
#define	VALUE_Boss_ROTATE	(D3DX_PI * 0.008f)		// 回転量
#define BOSS_LIFE			(360)	//300	
#define BOSS_R				(120.0f)
#define BOSS_TYPE_NUM       (3)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPD3DXMESH			g_pMesh[BOSS_TYPE_NUM] = { NULL };		// メッシュ情報へのポインタ
static LPD3DXBUFFER			g_pBuffMat[BOSS_TYPE_NUM] = { NULL };	// マテリアル情報へのポインタ
static DWORD				g_nNumMat[BOSS_TYPE_NUM] = { 0 };		// マテリアル情報の数
static LPDIRECT3DTEXTURE9	g_pTexture = NULL;	// テクスチャ


static BOSS					g_Boss[MAX_BOSS];	// エネミー

static D3DXVECTOR3			g_Target;
static int					g_BossEnableCnt = MAX_BOSS;			//存在している敵のカウント
static bool					g_BossAllDestroy = false;			//全滅フラグ
static float				g_MoverotY;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	PLAYER *pPlayer = GetPlayer();

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_Boss2,	// 読み込むモデルファイル名(Xファイル)
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
	if (FAILED(D3DXLoadMeshFromX(MODEL_Boss1,	// 読み込むモデルファイル名(Xファイル)
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
	if (FAILED(D3DXLoadMeshFromX(MODEL_LastBoss,	// 読み込むモデルファイル名(Xファイル)
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



	D3DXMATERIAL *pMat;

	for (int i = 0; i < BOSS_TYPE_NUM; i++)
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

	if (GetSceneNumber() != SCENE_INDEX_TUTORIAL)
	{
		for (int i = 0; i < MAX_BOSS; i++)
		{
			// 位置・回転・スケールの初期設定
			g_Boss[i].pos = D3DXVECTOR3(0.0f, 50.0f, 3000.0f);
			g_Boss[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Boss[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Boss[i].spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Target = pPlayer->pos;

			g_Boss[i].NowRotY = 0.0f;
			g_Boss[i].TargetRotY = 0.0f;

			// ベース位置に保存する
			g_Boss[i].base_pos = g_Boss[i].pos;

			g_Boss[i].bUse = true;

			g_Boss[i].BossBulletTimer = 0;
			if (GetSceneNumber() == SCENE_INDEX_GAME)
			{
				g_Boss[i].type = 0;
				g_Boss[i].Life = 3;	//BOSS_LIFE
				g_Boss[i].Life = BOSS_LIFE;	//360
				g_Boss[i].MaxLife = g_Boss[i].Life;
			}
			if (GetSceneNumber() == SCENE_INDEX_GAME2)
			{
				g_Boss[i].type = 1;
				g_Boss[i].Life = 520;
				g_Boss[i].MaxLife = g_Boss[i].Life;
			}
			if (GetSceneNumber() == SCENE_INDEX_GAME4)
			{
				g_Boss[i].type = 2;
				g_Boss[i].Life = 720;
				g_Boss[i].MaxLife = g_Boss[i].Life;
			}
			g_Boss[i].col_Sphere.pos.x = g_Boss[i].pos.x;
			g_Boss[i].col_Sphere.pos.y = g_Boss[i].pos.y;
			g_Boss[i].col_Sphere.pos.z = g_Boss[i].pos.z;

			g_Boss[i].col_Sphere.r = BOSS_R;

			g_Boss[i].idxBossShadow = CreateShadow(g_Boss[i].pos, 60.0f, 60.0f);
		}
	}	

	g_BossAllDestroy = false;

	g_MoverotY = 0.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{
	if (g_pTexture != NULL)
	{// テクスチャの開放
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	for (int i = 0; i < BOSS_TYPE_NUM; i++)
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
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
	PLAYER *pPlayer = GetPlayer();
	CAMERA *cam = GetCamera();

	g_BossEnableCnt = MAX_BOSS;

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (!g_Boss[i].bUse)
		{
			g_BossEnableCnt--;
			continue;
		}

		if (g_Boss[i].Life < 0)
		{
			Boss_Destroy(i);
		}

		g_Boss[i].g_Dist = GetFloatDist(Boss_GetSphere(i), Player_GetSphere());

		g_Target = pPlayer->pos;

		if (g_Boss[i].rot.y < 0)
		{
			g_Boss[i].rot.y += D3DX_PI * 2.0f;
		}

		if (g_Boss[i].rot.y > D3DX_PI * 2)
		{
			g_Boss[i].rot.y -= D3DX_PI * 2.0f;
		}

		g_Boss[i].NowRotY = g_Boss[i].rot.y;

		g_Boss[i].TargetRotY = (-atan2f((g_Target.z - g_Boss[i].pos.z), (g_Target.x - g_Boss[i].pos.x)) - (D3DX_PI / 2));

		g_MoverotY = (float)LerpRadian(g_Boss[i].NowRotY, g_Boss[i].TargetRotY, 0.01f);	//0.2で8

		g_Boss[i].rot.y = g_MoverotY;

		if (GetSceneNumber() != SCENE_INDEX_GAME4)
		{
			g_Boss[i].spd.x = sinf(D3DX_PI * 0.0f - g_Boss[i].rot.y) * 0.8f;
			g_Boss[i].spd.z = cosf(D3DX_PI * 0.0f - g_Boss[i].rot.y) * 0.8f;
		}

		g_Boss[i].pos.x += g_Boss[i].spd.x;
		g_Boss[i].pos.z -= g_Boss[i].spd.z;

		g_Boss[i].BossBulletTimer++;

		//約1秒に1回攻撃	5列攻撃
		if (g_Boss[i].type == 0)
		{
			if (g_Boss[i].BossBulletTimer == 60)
			{
				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));


				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 1.0f) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletTimer = 0;
			}
		}

		//約1秒に1回攻撃	3列と左右斜め2列
		if (g_Boss[i].type == 1)
		{
			if (g_Boss[i].BossBulletTimer == 60)
			{
				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));


				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));


				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));


				g_Boss[i].BossBulletTimer = 0;
			}
		}
		
		//約0.5秒に1回攻撃	三列と左右斜め
		if (g_Boss[i].type == 2)
		{
			if (g_Boss[i].BossBulletTimer % 30 == 1)
			{
				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 2.0f) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 2.0f) * 60.0f;

				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - 1.0f) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - 1.0f) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + 1.0f) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + 1.0f) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 30.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * -30.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y - (D3DX_PI / 3)) * -90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y - (D3DX_PI / 3)) * 60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(-sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, -cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * 60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * 30.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * 90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y - 1.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				//==================================================================================================================================

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -30.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));

				g_Boss[i].BossBulletBirthPos.x = g_Boss[i].pos.x - sinf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -90.0f;
				g_Boss[i].BossBulletBirthPos.z = g_Boss[i].pos.z - cosf(g_Boss[i].rot.y + (D3DX_PI / 3)) * -60.0f;
				SetBossBullet(g_Boss[i].BossBulletBirthPos, D3DXVECTOR3(sinf(g_Boss[i].rot.y + 2.0f) * 4.0f, 0.0f, cosf(g_Boss[i].rot.y) * 4.0f));
				
			}

			if (g_Boss[i].BossBulletTimer > 180)
			{
				SetBossBomb(D3DXVECTOR3(g_Boss[i].pos.x, g_Boss[i].pos.y, g_Boss[i].pos.z - BOSS_R), g_Boss[i].rot);
				g_Boss[i].BossBulletTimer = 0;
			}
		}

		g_Boss[i].col_Sphere.pos.x = g_Boss[i].pos.x;
		g_Boss[i].col_Sphere.pos.y = g_Boss[i].pos.y;
		g_Boss[i].col_Sphere.pos.z = g_Boss[i].pos.z;
		g_Boss[i].col_Sphere.r = BOSS_R;

		SetPositionShadow(g_Boss[i].idxBossShadow, D3DXVECTOR3(g_Boss[i].pos.x, 0.0f, g_Boss[i].pos.z));

		if (g_Boss[i].Life > (g_Boss[i].MaxLife / 10 * 5))
		{
			g_Boss[i].HpColor = 3;
		}

		if (g_Boss[i].Life < (g_Boss[i].MaxLife / 10 * 5))
		{
			g_Boss[i].HpColor = 2;
		}

		if (g_Boss[i].Life < (g_Boss[i].MaxLife / 10 * 2))
		{
			g_Boss[i].HpColor = 1;
		}
	}
	   	 
	if (g_BossEnableCnt == 0)
	{
		g_BossAllDestroy = true;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (!g_Boss[i].bUse)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Boss[i].mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Boss[i].scl.x, g_Boss[i].scl.y, g_Boss[i].scl.z);
		D3DXMatrixMultiply(&g_Boss[i].mtxWorld, &g_Boss[i].mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Boss[i].rot.y, g_Boss[i].rot.x, g_Boss[i].rot.z);
		D3DXMatrixMultiply(&g_Boss[i].mtxWorld, &g_Boss[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Boss[i].pos.x, g_Boss[i].pos.y, g_Boss[i].pos.z);
		D3DXMatrixMultiply(&g_Boss[i].mtxWorld, &g_Boss[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss[i].mtxWorld);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMat[g_Boss[i].type]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMat[g_Boss[i].type]; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定(テクスチャの有るモデルを読み込むときは注意)
			pDevice->SetTexture(0, g_pTexture);

			// 描画
			g_pMesh[g_Boss[i].type]->DrawSubset(nCntMat);
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// エネミーの取得
//=============================================================================
BOSS* GetBoss()
{
	return &(g_Boss[0]);
}

BOSS* GetAllBoss()
{
	return g_Boss;
}

SPHERE* Boss_GetSphere(int index)
{
	return (&g_Boss[index].col_Sphere);
}

void Boss_Destroy(int index)
{
	PlaySound(SOUND_LABEL_SE_EXPLOSION);
	AddScore(1000);
	g_Boss[index].bUse = false;
	ReleaseShadow(g_Boss[index].idxBossShadow);
}

void Boss_Damage(int index, int damage)
{
	g_Boss[index].Life -= damage;
	if (g_Boss[index].Life > 1)
	{
		PlaySound(SOUND_LABEL_SE_DAMAGE);
	}
}

bool Boss_IsEnable(int index)
{
	return g_Boss[index].bUse;
}

bool Boss_AllDestroy()
{
	return g_BossAllDestroy;
}