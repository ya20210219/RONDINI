//=============================================================================
//
// ロックオン弾処理 [Lockonbullet.cpp]
// Author :	Y.Ariyoshi
//
//=============================================================================

#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "Lockon.h"
#include "LockonBullet.h"
#include "player.h"
#include "main.h"
#include "collision.h"
#include "enemy.h"
#include "radian.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLET		"data/TEXTURE/LockonBullet.png"		// 読み込むテクスチャファイル名
#define	LOCKBULLET_WIDTH		(10.0f)							// 弾の幅
#define	LOCKBULLET_HEIGHT		(10.0f)							// 弾の高さ
#define	LOCKBULLET_SPEED		(8.0f)							// 移動速度

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexLockonBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexLockonBullet(int nIdxLockonBullet, float fWidth, float fHeight);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureLockonBullet = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffLockonBullet = NULL;	// 頂点バッファへのポインタ

static LOCKONBULLET				g_LockonBullet[MAX_LOCKON];		// 弾ワーク
static float					g_T_rot;
static bool						g_LockFire;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLockonBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// 頂点情報の作成
	MakeVertexLockonBullet(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_BULLET,			// ファイルの名前
		&g_pTextureLockonBullet);		// 読み込むメモリー

	// バレット配列の初期化
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		g_LockonBullet[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockonBullet[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockonBullet[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_LockonBullet[i].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockonBullet[i].timer = 200;
		g_LockonBullet[i].bUse = false;
		g_LockonBullet[i].LockonUse = false;
	}

	g_LockFire = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLockonBullet(void)
{
	if (g_pTextureLockonBullet != NULL)
	{// テクスチャの開放
		g_pTextureLockonBullet->Release();
		g_pTextureLockonBullet = NULL;
	}

	if (g_pVtxBuffLockonBullet != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffLockonBullet->Release();
		g_pVtxBuffLockonBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLockonBullet(void)
{
	LOCKON* pLock = GetLockOn();
	PLAYER* pPlayer = GetPlayer();

	if (LockOnRange_IsEnable() && Keyboard_IsRelease(DIK_LSHIFT))
	{
		g_LockFire = true;
	}

	if (Keyboard_IsTrigger(DIK_LSHIFT))
	{
		g_LockFire = false;
	}

	//弾の配列を全部調べる
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		//有効かフラグがONの物のみ処理を行う
		if (g_LockonBullet[i].bUse /*&& (pLock + i)->bUse*/ && GetLockonRange()->LockonFreeFlg == false)
		{
			g_LockonBullet[i].TargetRotY = (atan2f(((pLock + i)->posL.z - g_LockonBullet[i].pos.z), ((pLock + i)->posL.x - g_LockonBullet[i].pos.x)) - (D3DX_PI / 2));

			g_T_rot = (float)LerpRadian(g_LockonBullet[i].rot.y, g_LockonBullet[i].TargetRotY, 0.75f);

			g_LockonBullet[i].rot.y = g_T_rot;
				 
			g_LockonBullet[i].dir.x = sinf(D3DX_PI * 0.0f - g_LockonBullet[i].rot.y) * LOCKBULLET_SPEED;
			g_LockonBullet[i].dir.z = cosf(D3DX_PI * 0.0f - g_LockonBullet[i].rot.y) * LOCKBULLET_SPEED;

			g_LockonBullet[i].col_Sphere.r = LOCKBULLET_WIDTH;
				
			// 移動
			g_LockonBullet[i].pos.x += g_LockonBullet[i].dir.x;
			g_LockonBullet[i].pos.z += g_LockonBullet[i].dir.z;

			g_LockonBullet[i].col_Sphere.pos.x = g_LockonBullet[i].pos.x;
			g_LockonBullet[i].col_Sphere.pos.y = g_LockonBullet[i].pos.y;
			g_LockonBullet[i].col_Sphere.pos.z = g_LockonBullet[i].pos.z;

			// 生存フレームのタイマーを減らす
			g_LockonBullet[i].timer--;

			// 生存フレームが0以下になったら消去する
			if (g_LockonBullet[i].timer <= 0)
			{
				// 影の破棄
				ReleaseShadow(g_LockonBullet[i].idxShadow);

				g_LockonBullet[i].bUse = false;
			}
			else
			{
				// 影の位置設定
				SetPositionShadow(g_LockonBullet[i].idxShadow, D3DXVECTOR3(g_LockonBullet[i].pos.x, 0.1f, g_LockonBullet[i].pos.z));
			}
		}
	}

	//弾の配列を全部調べる
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		if (g_LockonBullet[i].bUse && GetLockonRange()->LockonFreeFlg == true)
		{
			//g_LockonBullet[i].rot.y = pPlayer->rot.y;

			if (g_LockonBullet[i].bUse)
			{
				//移動
				g_LockonBullet[i].pos.x += g_LockonBullet[i].dir.x;
				g_LockonBullet[i].pos.z += g_LockonBullet[i].dir.z;

				g_LockonBullet[i].col_Sphere.pos.x = g_LockonBullet[i].pos.x;
				g_LockonBullet[i].col_Sphere.pos.y = g_LockonBullet[i].pos.y;
				g_LockonBullet[i].col_Sphere.pos.z = g_LockonBullet[i].pos.z;

				//生存フレームのタイマーを減らす
				g_LockonBullet[i].timer--;

				//生存フレームが0以下になったら消去する
				if (g_LockonBullet[i].timer <= 0)
				{
					//影の破棄
					ReleaseShadow(g_LockonBullet[i].idxShadow);

					g_LockonBullet[i].bUse = false;
				}
				else
				{
					//影の位置設定
					SetPositionShadow(g_LockonBullet[i].idxShadow, D3DXVECTOR3(g_LockonBullet[i].pos.x, 0.1f, g_LockonBullet[i].pos.z));
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLockonBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_LOCKON; i++)
	{
		if (g_LockonBullet[i].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_LockonBullet[i].mtxWorld);

			D3DXMATRIX mtxView;

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// 逆行列を求める
			D3DXMatrixInverse(&g_LockonBullet[i].mtxWorld, NULL, &mtxView);

			// 平行移動の部分は無視する
			g_LockonBullet[i].mtxWorld._41 = 0.0f;
			g_LockonBullet[i].mtxWorld._42 = 0.0f;
			g_LockonBullet[i].mtxWorld._43 = 0.0f;
#else
			//転置行列
			g_LockonBullet[i].mtxWorld._11 = mtxView._11;
			g_LockonBullet[i].mtxWorld._12 = mtxView._21;
			g_LockonBullet[i].mtxWorld._13 = mtxView._31;
			g_LockonBullet[i].mtxWorld._21 = mtxView._12;
			g_LockonBullet[i].mtxWorld._22 = mtxView._22;
			g_LockonBullet[i].mtxWorld._23 = mtxView._32;
			g_LockonBullet[i].mtxWorld._31 = mtxView._13;
			g_LockonBullet[i].mtxWorld._32 = mtxView._23;
			g_LockonBullet[i].mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_LockonBullet[i].scale.x,
				g_LockonBullet[i].scale.y,
				g_LockonBullet[i].scale.z);
			D3DXMatrixMultiply(&g_LockonBullet[i].mtxWorld, &g_LockonBullet[i].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_LockonBullet[i].pos.x,
				g_LockonBullet[i].pos.y,
				g_LockonBullet[i].pos.z);
			D3DXMatrixMultiply(&g_LockonBullet[i].mtxWorld, &g_LockonBullet[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_LockonBullet[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffLockonBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureLockonBullet);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexLockonBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		FVF_VERTEX_3D,			// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffLockonBullet,		// 頂点バッファインターフェースへのポインタ
		NULL)))					// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffLockonBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-LOCKBULLET_WIDTH / 2, LOCKBULLET_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(LOCKBULLET_WIDTH / 2, LOCKBULLET_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-LOCKBULLET_WIDTH / 2, -LOCKBULLET_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(LOCKBULLET_WIDTH / 2, -LOCKBULLET_HEIGHT / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pVtxBuffLockonBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexLockonBullet(int nIdxLockonBullet, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffLockonBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffLockonBullet->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetLockonBullet(D3DXVECTOR3 pos, float y)
{
	int nIdxLockonBullet = -1;

	for (int i = 0; i < MAX_LOCKON; i++)
	{
		if (!g_LockonBullet[i].bUse && GetLockonRange()->LockonCoolDownFlg == false)
		{
			g_LockonBullet[i].pos = pos;
			g_LockonBullet[i].pos.y = LOCKBULLET_HEIGHT / 2;
			g_LockonBullet[i].rot = D3DXVECTOR3(0.0f, y, 0.0f);
			g_LockonBullet[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_LockonBullet[i].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (GetLockonRange()->LockonFreeFlg == true)
			{
				g_LockonBullet[i].dir.x = -sinf(g_LockonBullet[i].rot.y - 0.1f + (0.1f * i)) * LOCKBULLET_SPEED;
				g_LockonBullet[i].dir.y = 0.0f;
				g_LockonBullet[i].dir.z = -cosf(g_LockonBullet[i].rot.y - 0.1f + (0.1f * i)) * LOCKBULLET_SPEED;
			}
			g_LockonBullet[i].timer = 200;
			g_LockonBullet[i].bUse = true;

			// 影の設定
			g_LockonBullet[i].idxShadow = CreateShadow(pos, 8.0f, 8.0f);		// 影の設定

			nIdxLockonBullet = i;

			PlaySound(SOUND_LABEL_SE_LOCKONBULLET);

			break;
		}
	}

	return nIdxLockonBullet;
}

LOCKONBULLET* GetLockonBullet()
{
	return g_LockonBullet;
}

SPHERE* LockonBullet_GetSphere(int index)
{
	return (&g_LockonBullet[index].col_Sphere);
}

void LockonBullet_Destroy(int index)
{
	LOCKON* pLock = GetLockOn();

	g_LockonBullet[index].bUse = false;
	g_LockonBullet[index].LockonUse = false;

	(pLock + index)->bUse = false;

	ReleaseShadow(g_LockonBullet[index].idxShadow);
}

bool LockonBullet_IsEnable(int index)
{
	return g_LockonBullet[index].bUse;
}
