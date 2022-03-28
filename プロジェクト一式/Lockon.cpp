//=============================================================================
//
// ロックオン処理 [Lockon.cpp]
// Author :
//
//=============================================================================
#include "Lockon.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "UI.h"
#include "enemy.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	TEXTURE_LOCKON		"data/TEXTURE/TargetMarker.png"	// 読み込むテクスチャファイル名	
#define	TEXTURE_LOCKON		"data/TEXTURE/LockUI.png"	// 読み込むテクスチャファイル名
#define	LockOn_WIDTH		(30.0f)							// ロックオンの幅
#define	LockOn_HEIGHT		(30.0f)							// ロックオンの高さ
#define LOCKONRANGE_R		(300.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexLockOn(LPDIRECT3DDEVICE9 pDevice);
void SetVertexLockOn(int nIdxLockOn, float fWidth, float fHeight);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureLockOn = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffLockOn = NULL;	// 頂点バッファへのポインタ

static LOCKON					g_LockOn[MAX_LOCKON];		// ロックオンワーク

static LOCKONRANGE				g_LockOnRange;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLockOn(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// 頂点情報の作成
	MakeVertexLockOn(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_LOCKON,			// ファイルの名前
		&g_pTextureLockOn);		// 読み込むメモリー

	// ロックオン配列の初期化
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		g_LockOn[i].posL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockOn[i].rotL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_LockOn[i].scaleL = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_LockOn[i].TargetNum = 0;
		g_LockOn[i].bUse = false;
	}

	g_LockOnRange.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_LockOnRange.rotR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_LockOnRange.scaleR = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_LockOnRange.col_Sphere.pos = g_LockOnRange.posR;
	g_LockOnRange.col_Sphere.r = LOCKONRANGE_R;
	g_LockOnRange.bUse = false;

	g_LockOnRange.LockonCoolDownCnt = 200;
	g_LockOnRange.LockonCoolDownFlg = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLockOn(void)
{
	if (g_pTextureLockOn != NULL)
	{// テクスチャの開放
		g_pTextureLockOn->Release();
		g_pTextureLockOn = NULL;
	}

	if (g_pVtxBuffLockOn != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffLockOn->Release();
		g_pVtxBuffLockOn = NULL;
	}

	g_LockOnRange.bUse = false;
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		g_LockOn[i].bUse = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLockOn(void)
{
	PLAYER* pPlayer = GetPlayer();
	ENEMY* pEnemy = GetAllEnemy();
	   
	g_LockOnRange.UnLockonCnt = 0;

	//ロックオンの配列を全部調べる
	for (int i = 0; i < MAX_LOCKON; i++)
	{
		//有効かフラグがONの物のみ処理を行う
		if (g_LockOn[i].bUse)
		{
			g_LockOnRange.LockonFreeFlg = false;
			g_LockOn[i].posL = (pEnemy + g_LockOn[i].TargetNum)->pos;
		
			if ((pEnemy + g_LockOn[i].TargetNum)->bUse == false)
			{
				g_LockOn[i].bUse = false;
			}
		}
		if (!g_LockOn[i].bUse)
		{
			g_LockOnRange.UnLockonCnt++;
		}
	}

	if (g_LockOnRange.UnLockonCnt == MAX_LOCKON)
	{
		g_LockOnRange.LockonFreeFlg = true;
	}

	if (Keyboard_IsRelease(DIK_LSHIFT) && g_LockOnRange.LockonCoolDownCnt == 200)
	{
		g_LockOnRange.LockonCoolDownFlg = true;
		g_LockOnRange.LockonCoolDownCnt = 0;
	}

	if (g_LockOnRange.LockonCoolDownFlg == true)
	{
		g_LockOnRange.LockonCoolDownCnt++;
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (!(pEnemy + i)->bUse || !(pEnemy + i)->LockEnable)
			{
				continue;
			}

			(pEnemy + i)->LockEnable = false;
		}

		if (g_LockOnRange.LockonCoolDownCnt > 200)
		{
			g_LockOnRange.LockonCoolDownCnt = 200;
			g_LockOnRange.LockonCoolDownFlg = false;
		}
	}

	g_LockOnRange.posR.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * LOCKONRANGE_R;
	g_LockOnRange.posR.y = pPlayer->pos.y;
	g_LockOnRange.posR.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * LOCKONRANGE_R;

	g_LockOnRange.col_Sphere.pos = g_LockOnRange.posR;
	g_LockOnRange.col_Sphere.r = LOCKONRANGE_R;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLockOn(void)
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
		if (g_LockOn[i].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_LockOn[i].mtxWorld);

			D3DXMATRIX mtxView;

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// 逆行列を求める
			D3DXMatrixInverse(&g_LockOn[i].mtxWorld, NULL, &mtxView);

			// 平行移動の部分は無視する
			g_LockOn[i].mtxWorld._41 = 0.0f;
			g_LockOn[i].mtxWorld._42 = 0.0f;
			g_LockOn[i].mtxWorld._43 = 0.0f;
#else
			//転置行列
			g_LockOn[i].mtxWorld._11 = mtxView._11;
			g_LockOn[i].mtxWorld._12 = mtxView._21;
			g_LockOn[i].mtxWorld._13 = mtxView._31;
			g_LockOn[i].mtxWorld._21 = mtxView._12;
			g_LockOn[i].mtxWorld._22 = mtxView._22;
			g_LockOn[i].mtxWorld._23 = mtxView._32;
			g_LockOn[i].mtxWorld._31 = mtxView._13;
			g_LockOn[i].mtxWorld._32 = mtxView._23;
			g_LockOn[i].mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_LockOn[i].scaleL.x,
				g_LockOn[i].scaleL.y,
				g_LockOn[i].scaleL.z);
			D3DXMatrixMultiply(&g_LockOn[i].mtxWorld, &g_LockOn[i].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_LockOn[i].posL.x,
				g_LockOn[i].posL.y,
				g_LockOn[i].posL.z);
			D3DXMatrixMultiply(&g_LockOn[i].mtxWorld, &g_LockOn[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_LockOn[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffLockOn, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureLockOn);	

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
HRESULT MakeVertexLockOn(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		FVF_VERTEX_3D,			// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffLockOn,		// 頂点バッファインターフェースへのポインタ
		NULL)))					// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffLockOn->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-LockOn_WIDTH / 2, LockOn_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(LockOn_WIDTH / 2, LockOn_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-LockOn_WIDTH / 2, -LockOn_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(LockOn_WIDTH / 2, -LockOn_HEIGHT / 2, 0.0f);

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
		g_pVtxBuffLockOn->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexLockOn(int nIdxLockOn, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffLockOn->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffLockOn->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetLockOn(int index, D3DXVECTOR3 pos, bool enable)
{
	g_LockOn[index].posL = pos;
	g_LockOn[index].rotL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_LockOn[index].scaleL = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_LockOn[index].bUse = enable;
}

LOCKON* GetLockOn()
{
	return g_LockOn;
}

LOCKONRANGE* GetLockonRange()
{
	return &g_LockOnRange;
}

void LockOn_Destroy(int index)
{
	g_LockOn[index].bUse = false;
}

bool LockOn_IsEnable(int index)
{
	return g_LockOn[index].bUse;
}

bool LockOnRange_IsEnable()
{
	return g_LockOnRange.bUse;
}

SPHERE* LockOnRange_GetSphere()
{
	return (&g_LockOnRange.col_Sphere);
}