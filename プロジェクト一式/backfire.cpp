//=============================================================================
//
// バックファイア処理 [BackFire.cpp]
// Author :  
//
//=============================================================================
#include "BackFire.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"

#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BackFire	"data/TEXTURE/BackFire.png"	// 読み込むテクスチャファイル名
#define	BackFire_WIDTH		(10.0f)							// バックファイアの幅
#define	BackFire_HEIGHT		(10.0f)							// バックファイアの高さ

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
	int				countH;		//カウント
	bool			bUse;		// 使用しているかどうかのフラグ

} BackFire;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBackFire(LPDIRECT3DDEVICE9 pDevice, int w, int h);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBackFire = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBackFire = NULL;	// 頂点バッファへのポインタ

static BackFire					g_BackFire;		// バックファイアワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBackFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();


	// 頂点情報の作成
	MakeVertexBackFire(pDevice, 0, 0);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_BackFire,			// ファイルの名前
		&g_pTextureBackFire);		// 読み込むメモリー

	// バックファイアの初期化

	PLAYER* pPlayer = GetPlayer();

	g_BackFire.pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
	g_BackFire.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_BackFire.timer = 0;
	g_BackFire.countW = 0;
	g_BackFire.countH = 0;
	g_BackFire.bUse = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBackFire(void)
{
	if (g_pTextureBackFire != NULL)
	{// テクスチャの開放
		g_pTextureBackFire->Release();
		g_pTextureBackFire = NULL;
	}

	if (g_pVtxBuffBackFire != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBackFire->Release();
		g_pVtxBuffBackFire = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBackFire(void)
{
	g_BackFire.scale = D3DXVECTOR3(0.9f, 0.9f, 0.9f);

	if (g_BackFire.timer <= 60)
	{
		if (g_BackFire.timer % 2 == 0)
		{
			g_BackFire.countW++;
			if(g_BackFire.countW > 5)
			{
				g_BackFire.countH++;
				if (g_BackFire.countH > 2)
				{
					g_BackFire.countH = 0;
				}
				g_BackFire.countW = 0;
			}
			LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
			MakeVertexBackFire(pDevice, g_BackFire.countW, g_BackFire.countH);
		}
		if (g_BackFire.timer == 60)
		{
			g_BackFire.timer = 0;
		}
	}

	PLAYER* pPlayer = GetPlayer();

	g_BackFire.pos.x = pPlayer->pos.x - sinf(pPlayer->rot.y + D3DX_PI) * 7.5f;
	g_BackFire.pos.y = pPlayer->pos.y;
	g_BackFire.pos.z = pPlayer->pos.z - cosf(pPlayer->rot.y + D3DX_PI) * 7.5f;

	if (Keyboard_IsPress(DIK_W))
	{
		g_BackFire.scale = D3DXVECTOR3(1.1f, 1.1f, 1.1f);
	}
	if (Keyboard_IsPress(DIK_S))
	{
		g_BackFire.scale = D3DXVECTOR3(0.75f, 0.75f, 0.75f);
	}
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBackFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (g_BackFire.bUse)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_BackFire.mtxWorld);

		D3DXMATRIX mtxView;

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
		// 逆行列を求める
		D3DXMatrixInverse(&g_BackFire.mtxWorld, NULL, &mtxView);

		// 平行移動の部分は無視する
		g_BackFire.mtxWorld._41 = 0.0f;
		g_BackFire.mtxWorld._42 = 0.0f;
		g_BackFire.mtxWorld._43 = 0.0f;
#else
		//転置行列
		g_BackFire.mtxWorld._11 = mtxView._11;
		g_BackFire.mtxWorld._12 = mtxView._21;
		g_BackFire.mtxWorld._13 = mtxView._31;
		g_BackFire.mtxWorld._21 = mtxView._12;
		g_BackFire.mtxWorld._22 = mtxView._22;
		g_BackFire.mtxWorld._23 = mtxView._32;
		g_BackFire.mtxWorld._31 = mtxView._13;
		g_BackFire.mtxWorld._32 = mtxView._23;
		g_BackFire.mtxWorld._33 = mtxView._33;
#endif

		// スケールを反映
		D3DXMatrixScaling(&mtxScale, g_BackFire.scale.x,
			g_BackFire.scale.y,
			g_BackFire.scale.z);
		D3DXMatrixMultiply(&g_BackFire.mtxWorld, &g_BackFire.mtxWorld, &mtxScale);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_BackFire.pos.x,
			g_BackFire.pos.y,
			g_BackFire.pos.z);
		D3DXMatrixMultiply(&g_BackFire.mtxWorld, &g_BackFire.mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_BackFire.mtxWorld);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffBackFire, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBackFire);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBackFire(LPDIRECT3DDEVICE9 pDevice, int w, int h)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		FVF_VERTEX_3D,			// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBackFire,		// 頂点バッファインターフェースへのポインタ
		NULL)))					// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBackFire->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BackFire_WIDTH / 2, BackFire_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BackFire_WIDTH / 2, BackFire_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BackFire_WIDTH / 2, -BackFire_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BackFire_WIDTH / 2, -BackFire_HEIGHT / 2, 0.0f);

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
		g_pVtxBuffBackFire->Unlock();
	}

	return S_OK;
}

BackFire* GetBackFire()
{
	return &g_BackFire;
}

void BackFire_Destroy()
{
	g_BackFire.bUse = false;
}

bool BackFire_IsEnable()
{
	return g_BackFire.bUse;
}
