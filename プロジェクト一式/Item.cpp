//=============================================================================
//
// アイテム管理 [Item.cpp]
// Author : 
//
//=============================================================================

#include "MyDirect3D.h"
#include "shadow.h"
#include "scene.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "Item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ITEM1			"data/MODEL/Life001.x"	
#define	VALUE_ITEM_ROTATE	(D3DX_PI * 0.008f)		// 回転量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPD3DXMESH			g_pMesh = NULL;		// メッシュ情報へのポインタ
static LPD3DXBUFFER			g_pBuffMat = NULL;	// マテリアル情報へのポインタ
static DWORD				g_nNumMat = 0;		// マテリアル情報の数
static LPDIRECT3DTEXTURE9	g_pTexture = NULL;	// テクスチャ


static ITEM					g_Item[MAX_ITEM];	// アイテム

static int					g_ItemEnableCnt = MAX_ITEM;	//存在している敵のカウント
static bool					g_ItemAllDestroy = false;			//全滅フラグ
static int					g_BirthCnt;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	PLAYER *pPlayer = GetPlayer();

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_ITEM1,	// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
		pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
		NULL,									// 隣接性データを含むバッファへのポインタ
		&g_pBuffMat,							// マテリアルデータを含むバッファへのポインタ
		NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat,								// D3DXMATERIAL構造体の数
		&g_pMesh)))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}


	D3DXMATERIAL *pMat;
	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMat->GetBufferPointer();

	// マテリアルにテクスチャが登録されていれば読み込む
	if (pMat[0].pTextureFilename != NULL)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			pMat[0].pTextureFilename,			// ファイルの名前
			&g_pTexture);						// 読み込むメモリー
	}

	float interval = 75.0f;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		// 位置・回転・スケールの初期設定
		g_Item[i].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_Item[i].rot = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_Item[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		g_Item[i].bUse = false;

		g_Item[i].type = 1;

		g_Item[i].col_Sphere.pos.x = g_Item[i].pos.x;
		g_Item[i].col_Sphere.pos.y = g_Item[i].pos.y;
		g_Item[i].col_Sphere.pos.z = g_Item[i].pos.z;

		g_Item[i].col_Sphere.r = 10.0f;

		g_Item[i].idxItemShadow = CreateShadow(g_Item[i].pos, 30.0f, 30.0f);
	}

	g_ItemAllDestroy = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		g_Item[i].bUse = false;
	}

	if (g_pTexture != NULL)
	{// テクスチャの開放
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	if (g_pMesh != NULL)
	{// メッシュの開放
		g_pMesh->Release();
		g_pMesh = NULL;
	}

	if (g_pBuffMat != NULL)
	{// マテリアルの開放
		g_pBuffMat->Release();
		g_pBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	PLAYER *pPlayer = GetPlayer();
	CAMERA *cam = GetCamera();

	g_ItemEnableCnt = MAX_ITEM;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		
		if (!g_Item[i].bUse)
		{
			g_ItemEnableCnt--;
			continue;
		}

		g_Item[i].rot.y += VALUE_ITEM_ROTATE;

		if (g_Item[i].rot.y < 0)
		{
			g_Item[i].rot.y += D3DX_PI * 2.0f;
		}

		if (g_Item[i].rot.y > D3DX_PI * 2)
		{
			g_Item[i].rot.y -= D3DX_PI * 2.0f;
		}

		g_Item[i].col_Sphere.pos.x = g_Item[i].pos.x;
		g_Item[i].col_Sphere.pos.y = g_Item[i].pos.y;
		g_Item[i].col_Sphere.pos.z = g_Item[i].pos.z;
		g_Item[i].col_Sphere.r = 10.0f;

		SetPositionShadow(g_Item[i].idxItemShadow, D3DXVECTOR3(g_Item[i].pos.x, 0.0f, g_Item[i].pos.z));
	}

	if (g_ItemEnableCnt == 0)
	{
		g_ItemAllDestroy = true;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (!g_Item[i].bUse)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Item[i].mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Item[i].scl.x, g_Item[i].scl.y, g_Item[i].scl.z);
		D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[i].rot.y, g_Item[i].rot.x, g_Item[i].rot.z);
		D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Item[i].pos.x, g_Item[i].pos.y, g_Item[i].pos.z);
		D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Item[i].mtxWorld);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定(テクスチャの有るモデルを読み込むときは注意)
			pDevice->SetTexture(0, g_pTexture);

			// 描画
			g_pMesh->DrawSubset(nCntMat);
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

void SetItem(D3DXVECTOR3 pos)
{	
	if (GetSceneNumber() == SCENE_INDEX_TUTORIAL)
	{
		g_BirthCnt = rand() % 100 + 1;

		if (g_BirthCnt < 40)
		{ 
			for (int i = 0; i < MAX_ITEM; i++)
			{
				if (!g_Item[i].bUse)
				{
					g_Item[i].pos = pos;
					g_Item[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_Item[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					g_Item[i].bUse = true;

					g_Item[i].col_Sphere.pos.x = g_Item[i].pos.x;
					g_Item[i].col_Sphere.pos.y = g_Item[i].pos.y;
					g_Item[i].col_Sphere.pos.z = g_Item[i].pos.z;
					g_Item[i].col_Sphere.r = 10.0f;

					break;
				}
			}
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME)
	{
		g_BirthCnt = rand() % 100 + 1;

		if (g_BirthCnt < 10)
		{
			for (int i = 0; i < MAX_ITEM; i++)
			{
				if (!g_Item[i].bUse)
				{
					g_Item[i].pos = pos;
					g_Item[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_Item[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					g_Item[i].bUse = true;

					g_Item[i].col_Sphere.pos.x = g_Item[i].pos.x;
					g_Item[i].col_Sphere.pos.y = g_Item[i].pos.y;
					g_Item[i].col_Sphere.pos.z = g_Item[i].pos.z;
					g_Item[i].col_Sphere.r = 10.0f;

					break;
				}
			}
		}
	}
	
}

//=============================================================================
// アイテムの取得
//=============================================================================
ITEM* GetItem(int index)
{
	return &(g_Item[index]);
}

ITEM* GetAllItem()
{
	return g_Item;
}

SPHERE* Item_GetSphere(int index)
{
	return (&g_Item[index].col_Sphere);
}

void Item_Destroy(int index)
{
	g_Item[index].bUse = false;
	ReleaseShadow(g_Item[index].idxItemShadow);
}

bool Item_IsEnable(int index)
{
	return g_Item[index].bUse;
}

bool Item_AllDestroy()
{
	return g_ItemAllDestroy;
}