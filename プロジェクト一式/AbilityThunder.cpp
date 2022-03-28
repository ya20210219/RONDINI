//=============================================================================
//
// バックファイア処理 [AbilityThunder.cpp]
// Author : 
//
//=============================================================================
#include "AbilityThunder.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"

#include "player.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_AbilityThunder	"data/TEXTURE/AbilityThunder.png"	// 読み込むテクスチャファイル名
#define	AbilityThunder_WIDTH		(100.0f)							// バックファイアの幅
#define	AbilityThunder_HEIGHT		(100.0f)							// バックファイアの高さ

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexAbilityThunder(LPDIRECT3DDEVICE9 pDevice, int w, int h);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureAbilityThunder = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffAbilityThunder = NULL;	// 頂点バッファへのポインタ

static AbilityThunder					g_AbilityThunder;		// バックファイアワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAbilityThunder(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();


	// 頂点情報の作成
	MakeVertexAbilityThunder(pDevice, 0, 0);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_AbilityThunder,			// ファイルの名前
		&g_pTextureAbilityThunder);		// 読み込むメモリー

	// バックファイアの初期化

	PLAYER* pPlayer = GetPlayer();

	g_AbilityThunder.pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
	g_AbilityThunder.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_AbilityThunder.timer = 0;
	g_AbilityThunder.countW = 0;
	g_AbilityThunder.countH = 0;
	g_AbilityThunder.bUse = true;

	g_AbilityThunder.PlusThunderCnt = 300;
	g_AbilityThunder.PlusThunderUseCnt = 0;
	g_AbilityThunder.PlusThunder = false;
	g_AbilityThunder.PlusThunderCoolDownFlg = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAbilityThunder(void)
{
	if (g_pTextureAbilityThunder != NULL)
	{// テクスチャの開放
		g_pTextureAbilityThunder->Release();
		g_pTextureAbilityThunder = NULL;
	}

	if (g_pVtxBuffAbilityThunder != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffAbilityThunder->Release();
		g_pVtxBuffAbilityThunder = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAbilityThunder(void)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->Pattern != 1)
	{
		g_AbilityThunder.bUse = false;
	}

	if (g_AbilityThunder.bUse == true && g_AbilityThunder.PlusThunder == true)
	{
		if (g_AbilityThunder.timer <= 60)
		{
			if (g_AbilityThunder.timer % 2 == 0)
			{
				g_AbilityThunder.countW++;
				if (g_AbilityThunder.countW > 5)
				{
					g_AbilityThunder.countH++;
					if (g_AbilityThunder.countH > 2)
					{
						g_AbilityThunder.countH = 0;
					}
					g_AbilityThunder.countW = 0;
				}
				LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
				MakeVertexAbilityThunder(pDevice, g_AbilityThunder.countW, g_AbilityThunder.countH);
			}
			if (g_AbilityThunder.timer == 60)
			{
				g_AbilityThunder.timer = 0;
			}
		}

		pPlayer->PlayerSpd = 3.0f;

		g_AbilityThunder.pos.x = pPlayer->pos.x;
		g_AbilityThunder.pos.y = pPlayer->pos.y;
		g_AbilityThunder.pos.z = pPlayer->pos.z;

		g_AbilityThunder.col_Sphere.pos.x = pPlayer->pos.x;
		g_AbilityThunder.col_Sphere.pos.y = pPlayer->pos.y;
		g_AbilityThunder.col_Sphere.pos.z = pPlayer->pos.z;

		g_AbilityThunder.col_Sphere.r = 50.0f;

		g_AbilityThunder.PlusThunderUseCnt++;

		if (g_AbilityThunder.PlusThunderUseCnt == 1)
		{
			PlaySound(SOUND_LABEL_SE_THUNDER);
		}

		if (g_AbilityThunder.PlusThunderUseCnt >= 180)
		{
			g_AbilityThunder.PlusThunder = false;
			g_AbilityThunder.PlusThunderCoolDownFlg = true;
			g_AbilityThunder.PlusThunderUseCnt = 0;
			g_AbilityThunder.PlusThunderCnt = 0;
			StopSound(SOUND_LABEL_SE_THUNDER);
		}
	}

	if (g_AbilityThunder.PlusThunderCoolDownFlg == true)
	{
		pPlayer->PlayerSpd = 1.8f;

		g_AbilityThunder.PlusThunderCnt++;
		if (g_AbilityThunder.PlusThunderCnt > 300)
		{
			g_AbilityThunder.PlusThunderCoolDownFlg = false;
			g_AbilityThunder.PlusThunderCnt = 300;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAbilityThunder(void)
{
	if (g_AbilityThunder.PlusThunder == true)
	{
		LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
		D3DXMATRIX mtxScale, mtxTranslate;

		// ライティングを無効に
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// αテストを有効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		if (g_AbilityThunder.bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_AbilityThunder.mtxWorld);

			D3DXMATRIX mtxView;

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// 逆行列を求める
			D3DXMatrixInverse(&g_AbilityThunder.mtxWorld, NULL, &mtxView);

			// 平行移動の部分は無視する
			g_AbilityThunder.mtxWorld._41 = 0.0f;
			g_AbilityThunder.mtxWorld._42 = 0.0f;
			g_AbilityThunder.mtxWorld._43 = 0.0f;
#else
			//転置行列
			g_AbilityThunder.mtxWorld._11 = mtxView._11;
			g_AbilityThunder.mtxWorld._12 = mtxView._21;
			g_AbilityThunder.mtxWorld._13 = mtxView._31;
			g_AbilityThunder.mtxWorld._21 = mtxView._12;
			g_AbilityThunder.mtxWorld._22 = mtxView._22;
			g_AbilityThunder.mtxWorld._23 = mtxView._32;
			g_AbilityThunder.mtxWorld._31 = mtxView._13;
			g_AbilityThunder.mtxWorld._32 = mtxView._23;
			g_AbilityThunder.mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_AbilityThunder.scale.x,
				g_AbilityThunder.scale.y,
				g_AbilityThunder.scale.z);
			D3DXMatrixMultiply(&g_AbilityThunder.mtxWorld, &g_AbilityThunder.mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_AbilityThunder.pos.x,
				g_AbilityThunder.pos.y,
				g_AbilityThunder.pos.z);
			D3DXMatrixMultiply(&g_AbilityThunder.mtxWorld, &g_AbilityThunder.mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_AbilityThunder.mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffAbilityThunder, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureAbilityThunder);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}

		// ライティングを有効に
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// αテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexAbilityThunder(LPDIRECT3DDEVICE9 pDevice, int w, int h)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		FVF_VERTEX_3D,			// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffAbilityThunder,		// 頂点バッファインターフェースへのポインタ
		NULL)))					// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffAbilityThunder->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-AbilityThunder_WIDTH / 2, AbilityThunder_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(AbilityThunder_WIDTH / 2, AbilityThunder_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-AbilityThunder_WIDTH / 2, -AbilityThunder_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(AbilityThunder_WIDTH / 2, -AbilityThunder_HEIGHT / 2, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.2f * w), 0.0f + (0.5f * h));
		pVtx[1].tex = D3DXVECTOR2(0.0f + (0.2f * (w + 1)), 0.0f + (0.5f * h));
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.2f * w), 0.0f + (0.5f * (h + 1)));
		pVtx[3].tex = D3DXVECTOR2(0.0f + (0.2f * (w + 1)), 0.0f + (0.5f * (h + 1)));

		// 頂点データをアンロックする
		g_pVtxBuffAbilityThunder->Unlock();
	}

	return S_OK;
}

AbilityThunder* GetAbilityThunder()
{
	return &g_AbilityThunder;
}

SPHERE* GetAbilityThunderSphere()
{
	return &g_AbilityThunder.col_Sphere;
}

void AbilityThunder_Destroy()
{
	g_AbilityThunder.bUse = false;
}

bool AbilityThunder_IsEnable()
{
	return g_AbilityThunder.PlusThunder;
}
