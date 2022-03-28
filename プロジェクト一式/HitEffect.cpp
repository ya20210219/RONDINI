//=============================================================================
//
// ヒットエフェクト処理 [HitEffect.cpp]
// Author :  
//
//=============================================================================
#include "HitEffect.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"

#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_HitEffect	"data/TEXTURE/HitEffect.png"	// 読み込むテクスチャファイル名
#define	HitEffect_WIDTH		(30.0f)							// バックファイアの幅
#define	HitEffect_HEIGHT	(30.0f)							// バックファイアの高さ
#define HITEffect_Timer		(24)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		scale;		// スケール
	int				timer;		// 生存フレーム
	int				countW;		//カウント
	bool			bUse;		// 使用しているかどうかのフラグ

} HitEffect;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexHitEffect(LPDIRECT3DDEVICE9 pDevice, int w);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureHitEffect = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffHitEffect = NULL;	// 頂点バッファへのポインタ

static HitEffect					g_HitEffect[MAX_HITEFFECT];		// バックファイアワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();


	// 頂点情報の作成
	MakeVertexHitEffect(pDevice, 0);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_HitEffect,			// ファイルの名前
		&g_pTextureHitEffect);		// 読み込むメモリー

	// ヒットエフェクトの初期化

	for (int i = 0; i < MAX_HITEFFECT; i++)
	{
		g_HitEffect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_HitEffect[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_HitEffect[i].timer = 0;
		g_HitEffect[i].countW = 0;
		g_HitEffect[i].bUse = false;

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHitEffect(void)
{
	if (g_pTextureHitEffect != NULL)
	{// テクスチャの開放
		g_pTextureHitEffect->Release();
		g_pTextureHitEffect = NULL;
	}

	if (g_pVtxBuffHitEffect != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffHitEffect->Release();
		g_pVtxBuffHitEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHitEffect(void)
{
	for (int i = 0; i < MAX_HITEFFECT; i++)
	{
		if (g_HitEffect[i].bUse)
		{
			g_HitEffect[i].timer++;

			if (g_HitEffect[i].timer <= HITEffect_Timer)
			{
				if (g_HitEffect[i].timer % 3 == 0)
				{
					g_HitEffect[i].countW++;
					if (g_HitEffect[i].countW > 8)
					{
						g_HitEffect[i].countW = 0;
					}
					LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
					MakeVertexHitEffect(pDevice, g_HitEffect[i].countW);
				}
				if (g_HitEffect[i].timer == HITEffect_Timer)
				{
					g_HitEffect[i].timer = 0;
					g_HitEffect[i].bUse = false;
				}
			}
		}
	}	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_HITEFFECT; i++)
	{
		if (g_HitEffect[i].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_HitEffect[i].mtxWorld);

			D3DXMATRIX mtxView;

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// 逆行列を求める
			D3DXMatrixInverse(&g_HitEffect[i].mtxWorld, NULL, &mtxView);

			// 平行移動の部分は無視する
			g_HitEffect[i].mtxWorld._41 = 0.0f;
			g_HitEffect[i].mtxWorld._42 = 0.0f;
			g_HitEffect[i].mtxWorld._43 = 0.0f;
#else
			//転置行列
			g_HitEffect[i].mtxWorld._11 = mtxView._11;
			g_HitEffect[i].mtxWorld._12 = mtxView._21;
			g_HitEffect[i].mtxWorld._13 = mtxView._31;
			g_HitEffect[i].mtxWorld._21 = mtxView._12;
			g_HitEffect[i].mtxWorld._22 = mtxView._22;
			g_HitEffect[i].mtxWorld._23 = mtxView._32;
			g_HitEffect[i].mtxWorld._31 = mtxView._13;
			g_HitEffect[i].mtxWorld._32 = mtxView._23;
			g_HitEffect[i].mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_HitEffect[i].scale.x,
				g_HitEffect[i].scale.y,
				g_HitEffect[i].scale.z);
			D3DXMatrixMultiply(&g_HitEffect[i].mtxWorld, &g_HitEffect[i].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_HitEffect[i].pos.x,
				g_HitEffect[i].pos.y,
				g_HitEffect[i].pos.z);
			D3DXMatrixMultiply(&g_HitEffect[i].mtxWorld, &g_HitEffect[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_HitEffect[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffHitEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureHitEffect);

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
HRESULT MakeVertexHitEffect(LPDIRECT3DDEVICE9 pDevice, int w)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		FVF_VERTEX_3D,			// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffHitEffect,		// 頂点バッファインターフェースへのポインタ
		NULL)))					// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffHitEffect->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-HitEffect_WIDTH / 2, HitEffect_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(HitEffect_WIDTH / 2, HitEffect_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-HitEffect_WIDTH / 2, -HitEffect_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(HitEffect_WIDTH / 2, -HitEffect_HEIGHT / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * w), 0.0f );
		pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f * (w + 1)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.125f * w), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f + (0.125f * (w + 1)), 1.0f);

		// 頂点データをアンロックする
		g_pVtxBuffHitEffect->Unlock();
	}

	return S_OK;
}

void SetHitEffect(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	for (int i = 0; i < MAX_HITEFFECT; i++)
	{
		if (!g_HitEffect[i].bUse)
		{
			g_HitEffect[i].pos = pos;
			g_HitEffect[i].scale = scale;
			g_HitEffect[i].timer = 0;
			g_HitEffect[i].bUse = true;

			break;
		}
	}
}

/*
HitEffect* GetHitEffect()
{
	return &g_HitEffect[i];
}

void HitEffect_Destroy()
{
	g_HitEffect[i].bUse = false;
}

bool HitEffect_IsEnable()
{
	return g_HitEffect[i].bUse;
}
*/
