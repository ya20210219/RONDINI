//=============================================================================
//
// 敵の弾処理 [BossBomb.cpp]
// Author : 
//
//=============================================================================
#include "BossBomb.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "radian.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ENEMY_TEXTURE_BULLET_BULLET		"data/TEXTURE/BossBomb2.png"	// 読み込むテクスチャファイル名
#define	ENEMY_BOSS_BULLET_WIDTH			(75.0f)							// 弾の幅
#define	ENEMY_BOSS_BULLET_HEIGHT		(75.0f)							// 弾の高さ
#define	ENEMY_VALUE_MOVE_BOSS_BULLET	(3.0f)							// 移動速度

#define ENEMY_BOSS_BULLET_LIFE			(500)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBossBomb(LPDIRECT3DDEVICE9 pDevice, int w, int h);
void SetVertexBossBomb(int nIdxBossBomb, float fWidth, float fHeight);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBossBomb = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBossBomb = NULL;	// 頂点バッファへのポインタ

static ENEMYBOMB				g_BossBomb[MAX_BOSS_BOMB];		// 弾ワーク
static float					g_TargetY = 0.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBossBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// 頂点情報の作成
	MakeVertexBossBomb(pDevice, 0, 0);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		ENEMY_TEXTURE_BULLET_BULLET,			// ファイルの名前
		&g_pTextureBossBomb);		// 読み込むメモリー

// バレット配列の初期化
	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		g_BossBomb[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BossBomb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BossBomb[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_BossBomb[i].dir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_BossBomb[i].timer = 0;
		g_BossBomb[i].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossBomb(void)
{
	if (g_pTextureBossBomb != NULL)
	{// テクスチャの開放
		g_pTextureBossBomb->Release();
		g_pTextureBossBomb = NULL;
	}

	if (g_pVtxBuffBossBomb != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBossBomb->Release();
		g_pVtxBuffBossBomb = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossBomb(void)
{
	PLAYER* pPlayer = GetPlayer();

	//弾の配列を全部調べる
	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		//有効かフラグがONの物のみ処理を行う
		if (g_BossBomb[i].bUse)
		{
			g_BossBomb[i].countW++;
			if (g_BossBomb[i].countW > 5)
			{
				g_BossBomb[i].countH++;
				if (g_BossBomb[i].countH > 2)
				{
					g_BossBomb[i].countH = 0;
				}
				g_BossBomb[i].countW = 0;
			}
			LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
			MakeVertexBossBomb(pDevice, g_BossBomb[i].countW, g_BossBomb[i].countH);


			if (g_BossBomb[i].rot.y < 0)
			{
				g_BossBomb[i].rot.y += D3DX_PI * 2.0f;
			}

			if (g_BossBomb[i].rot.y > D3DX_PI * 2)
			{
				g_BossBomb[i].rot.y -= D3DX_PI * 2.0f;
			}

			g_BossBomb[i].PlayerY = (-atan2f((pPlayer->pos.z - g_BossBomb[i].pos.z), (pPlayer->pos.x - g_BossBomb[i].pos.x)) - (D3DX_PI / 2));

			if (g_BossBomb[i].PlayerY < 0)
			{
				g_BossBomb[i].PlayerY += D3DX_PI * 2.0f;
			}

			if (g_BossBomb[i].PlayerY > D3DX_PI * 2.0f)
			{
				g_BossBomb[i].PlayerY -= D3DX_PI * 2.0f;
			}

			g_TargetY = (float)LerpRadian(g_BossBomb[i].rot.y, g_BossBomb[i].PlayerY, 0.01f);
			
			g_BossBomb[i].rot.y = g_TargetY;

			g_BossBomb[i].dir.x = sinf(D3DX_PI * 0.0f - g_BossBomb[i].rot.y) * ENEMY_VALUE_MOVE_BOSS_BULLET;
			g_BossBomb[i].dir.z = cosf(D3DX_PI * 0.0f - g_BossBomb[i].rot.y) * ENEMY_VALUE_MOVE_BOSS_BULLET;

			// 移動
			g_BossBomb[i].pos.x += g_BossBomb[i].dir.x;
			g_BossBomb[i].pos.z -= g_BossBomb[i].dir.z;

			g_BossBomb[i].col_Sphere.pos.x = g_BossBomb[i].pos.x;
			g_BossBomb[i].col_Sphere.pos.y = g_BossBomb[i].pos.y;
			g_BossBomb[i].col_Sphere.pos.z = g_BossBomb[i].pos.z;

			g_BossBomb[i].col_Sphere.r = 60.0f;

			// 生存フレームのタイマーを減らす
			g_BossBomb[i].timer--;

			// 生存フレームが0以下になったら消去する
			if (g_BossBomb[i].timer <= 0)
			{
				// 影の破棄
				ReleaseShadow(g_BossBomb[i].idxShadow);

				g_BossBomb[i].bUse = false;
			}
			else
			{
				// 影の位置設定
				SetPositionShadow(g_BossBomb[i].idxShadow, D3DXVECTOR3(g_BossBomb[i].pos.x, 0.1f, g_BossBomb[i].pos.z));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		if (g_BossBomb[i].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_BossBomb[i].mtxWorld);

			D3DXMATRIX mtxView;

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// 逆行列を求める
			D3DXMatrixInverse(&g_BossBomb[i].mtxWorld, NULL, &mtxView);

			// 平行移動の部分は無視する
			g_BossBomb[i].mtxWorld._41 = 0.0f;
			g_BossBomb[i].mtxWorld._42 = 0.0f;
			g_BossBomb[i].mtxWorld._43 = 0.0f;
#else
			//転置行列
			g_BossBomb[i].mtxWorld._11 = mtxView._11;
			g_BossBomb[i].mtxWorld._12 = mtxView._21;
			g_BossBomb[i].mtxWorld._13 = mtxView._31;
			g_BossBomb[i].mtxWorld._21 = mtxView._12;
			g_BossBomb[i].mtxWorld._22 = mtxView._22;
			g_BossBomb[i].mtxWorld._23 = mtxView._32;
			g_BossBomb[i].mtxWorld._31 = mtxView._13;
			g_BossBomb[i].mtxWorld._32 = mtxView._23;
			g_BossBomb[i].mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_BossBomb[i].scale.x,
				g_BossBomb[i].scale.y,
				g_BossBomb[i].scale.z);
			D3DXMatrixMultiply(&g_BossBomb[i].mtxWorld, &g_BossBomb[i].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_BossBomb[i].pos.x,
				g_BossBomb[i].pos.y,
				g_BossBomb[i].pos.z);
			D3DXMatrixMultiply(&g_BossBomb[i].mtxWorld, &g_BossBomb[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_BossBomb[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffBossBomb, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBossBomb);

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
HRESULT MakeVertexBossBomb(LPDIRECT3DDEVICE9 pDevice, int w, int h)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		FVF_VERTEX_3D,			// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBossBomb,		// 頂点バッファインターフェースへのポインタ
		NULL)))					// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBossBomb->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-ENEMY_BOSS_BULLET_WIDTH / 2, ENEMY_BOSS_BULLET_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENEMY_BOSS_BULLET_WIDTH / 2, ENEMY_BOSS_BULLET_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMY_BOSS_BULLET_WIDTH / 2, -ENEMY_BOSS_BULLET_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENEMY_BOSS_BULLET_WIDTH / 2, -ENEMY_BOSS_BULLET_HEIGHT / 2, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.2f * w), 0.0f + (0.5f * h));
		pVtx[1].tex = D3DXVECTOR2(0.0f + (0.2f * (w + 1)), 0.0f + (0.5f * h));
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.2f * w), 0.0f + (0.5f * (h + 1)));
		pVtx[3].tex = D3DXVECTOR2(0.0f + (0.2f * (w + 1)), 0.0f + (0.5f * (h + 1)));

		// 頂点データをアンロックする
		g_pVtxBuffBossBomb->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBossBomb(int nIdxBossBomb, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBossBomb->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffBossBomb->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetBossBomb(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nIdxBossBomb = -1;

	for (int i = 0; i < MAX_BOSS_BOMB; i++)
	{
		if (!g_BossBomb[i].bUse)
		{
			g_BossBomb[i].pos = pos;
			g_BossBomb[i].pos.y = ENEMY_BOSS_BULLET_HEIGHT / 2;
			g_BossBomb[i].rot = rot;
			g_BossBomb[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_BossBomb[i].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BossBomb[i].timer = ENEMY_BOSS_BULLET_LIFE;
			g_BossBomb[i].bUse = true;

			PlaySound(SOUND_LABEL_SE_BOMB01);

			nIdxBossBomb = i;

			break;
		}
	}

	return nIdxBossBomb;
}

ENEMYBOMB* GetBossBomb()
{
	return g_BossBomb;
}

SPHERE* BossBomb_GetSphere(int index)
{
	return (&g_BossBomb[index].col_Sphere);
}

void BossBomb_Destroy(int index)
{
	g_BossBomb[index].timer = 0;
	g_BossBomb[index].bUse = false;
	ReleaseShadow(g_BossBomb[index].idxShadow);
}

bool BossBomb_IsEnable(int index)
{
	return g_BossBomb[index].bUse;
}
