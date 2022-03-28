//=============================================================================
//
// 弾処理 [Bomb.cpp]
// Author :
//
//=============================================================================
#include "AbilityBomb.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"
#include "Lockon.h"
#include "player.h"
#include "collision.h"
#include "enemy.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLET		"data/TEXTURE/AbilityBomb.png"	// 読み込むテクスチャファイル名
#define	BOMB_WIDTH		(50.0f)							// 弾の幅
#define	BOMB_HEIGHT		(50.0f)							// 弾の高さ
#define	VALUE_MOVE_BOMB	(0.08f)							// 移動速度
#define BOMB_LIFE			(300)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 回転
	D3DXVECTOR3		scale;		// スケール
	D3DXVECTOR3		dir;		// 移動量
	int				timer;		// 生存フレーム
	int				Life;		// ライフ
	int				idxShadow;	// 影ID
	bool			bUse;		// 使用しているかどうかのフラグ

	bool			BombUse;
	int				BombCoolDownCnt;
	bool			BombCoolDownFlg;
	int				countW;
	int				countH;

	SPHERE			col_Sphere;

} BOMB;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBomb(LPDIRECT3DDEVICE9 pDevice, int w, int h);
void SetVertexBomb(int nIdxBullet, float fWidth, float fHeight);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBomb = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBomb = NULL;	// 頂点バッファへのポインタ

static BOMB					g_Bomb[MAX_BOMB];		// 弾ワーク
static BOMBMGR				g_BombMgr;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// 頂点情報の作成
	MakeVertexBomb(pDevice, 0, 0);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_BULLET,			// ファイルの名前
		&g_pTextureBomb);		// 読み込むメモリー

// バレット配列の初期化
	for (int i = 0; i < MAX_BOMB; i++)
	{
		g_Bomb[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bomb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bomb[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bomb[i].dir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bomb[i].timer = 0;
		g_Bomb[i].Life = 0;
		g_Bomb[i].bUse = false;
	}

	g_BombMgr.BombUse = false;
	g_BombMgr.BombCoolDownCnt = BOMB_LIFE;
	g_BombMgr.BombCoolDownFlg = false;
	g_BombMgr.countW = 0;
	g_BombMgr.countH = 0;
	g_BombMgr.UseTimer = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBomb(void)
{
	if (g_pTextureBomb != NULL)
	{// テクスチャの開放
		g_pTextureBomb->Release();
		g_pTextureBomb = NULL;
	}

	if (g_pVtxBuffBomb != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBomb->Release();
		g_pVtxBuffBomb = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBomb(void)
{
	LOCKON* pLock = GetLockOn();
	PLAYER* pPlayer = GetPlayer();

	if (g_BombMgr.BombUse == true)
	{		
		g_BombMgr.UseTimer++;
		if (g_BombMgr.UseTimer > BOMB_LIFE)
		{
			g_BombMgr.BombCoolDownCnt = 0;
			g_BombMgr.BombUse = false;
			g_BombMgr.BombCoolDownFlg = true;
		}
		
		//両方の弾が存在しなければクールダウン開始
		if (g_Bomb[0].bUse == false && g_Bomb[1].bUse == false)
		{
			g_BombMgr.BombCoolDownCnt = 0;
			g_BombMgr.BombUse = false;
			g_BombMgr.BombCoolDownFlg = true;
		}

		//弾の配列を全部調べる
		for (int i = 0; i < MAX_BOMB; i++)
		{
			if (g_Bomb[i].Life < 1)
			{
				g_Bomb[i].bUse = false;
			}

			//有効かフラグがONの物のみ処理を行う
			if (g_Bomb[i].bUse)
			{
				g_Bomb[i].countW++;
				if (g_Bomb[i].countW > 5)
				{
					g_Bomb[i].countH++;
					if (g_Bomb[i].countH > 2)
					{
						g_Bomb[i].countH = 0;
					}
					g_Bomb[i].countW = 0;
				}
				LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
				MakeVertexBomb(pDevice, g_Bomb[i].countW, g_Bomb[i].countH);

				// 移動
				g_Bomb[i].pos.x += g_Bomb[i].dir.x;
				g_Bomb[i].pos.z += g_Bomb[i].dir.z;

				g_Bomb[i].col_Sphere.pos.x = g_Bomb[i].pos.x;
				g_Bomb[i].col_Sphere.pos.y = g_Bomb[i].pos.y;
				g_Bomb[i].col_Sphere.pos.z = g_Bomb[i].pos.z;

				g_Bomb[i].col_Sphere.r = BOMB_WIDTH / 2;
				//g_Bomb[i].col_Sphere.r = 10000.0f;

				// 生存フレームのタイマーを減らす
				g_Bomb[i].timer--;

				// 生存フレームが0以下になったら消去する
				if (g_Bomb[i].timer <= 0 || g_BombMgr.BombCoolDownFlg == true)
				{
					// 影の破棄
					ReleaseShadow(g_Bomb[i].idxShadow);
					g_Bomb[i].bUse = false;
				}
				else
				{
					// 影の位置設定
					SetPositionShadow(g_Bomb[i].idxShadow, D3DXVECTOR3(g_Bomb[i].pos.x, 0.1f, g_Bomb[i].pos.z));
				}
			}
		}	
	}	

	if (g_BombMgr.BombCoolDownFlg == true)
	{
		g_BombMgr.UseTimer = 0;
		g_BombMgr.BombCoolDownCnt++;
		if (g_BombMgr.BombCoolDownCnt > BOMB_LIFE)
		{
			g_BombMgr.BombCoolDownFlg = false;
			g_BombMgr.BombCoolDownCnt = BOMB_LIFE;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_BOMB; i++)
	{
		if (g_Bomb[i].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Bomb[i].mtxWorld);

			D3DXMATRIX mtxView;

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// 逆行列を求める
			D3DXMatrixInverse(&g_Bomb[i].mtxWorld, NULL, &mtxView);

			// 平行移動の部分は無視する
			g_Bomb[i].mtxWorld._41 = 0.0f;
			g_Bomb[i].mtxWorld._42 = 0.0f;
			g_Bomb[i].mtxWorld._43 = 0.0f;
#else
			//転置行列
			g_Bomb[i].mtxWorld._11 = mtxView._11;
			g_Bomb[i].mtxWorld._12 = mtxView._21;
			g_Bomb[i].mtxWorld._13 = mtxView._31;
			g_Bomb[i].mtxWorld._21 = mtxView._12;
			g_Bomb[i].mtxWorld._22 = mtxView._22;
			g_Bomb[i].mtxWorld._23 = mtxView._32;
			g_Bomb[i].mtxWorld._31 = mtxView._13;
			g_Bomb[i].mtxWorld._32 = mtxView._23;
			g_Bomb[i].mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_Bomb[i].scale.x,
				g_Bomb[i].scale.y,
				g_Bomb[i].scale.z);
			D3DXMatrixMultiply(&g_Bomb[i].mtxWorld, &g_Bomb[i].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Bomb[i].pos.x,
				g_Bomb[i].pos.y,
				g_Bomb[i].pos.z);
			D3DXMatrixMultiply(&g_Bomb[i].mtxWorld, &g_Bomb[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Bomb[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffBomb, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBomb);

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
HRESULT MakeVertexBomb(LPDIRECT3DDEVICE9 pDevice, int w, int h)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		FVF_VERTEX_3D,			// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBomb,		// 頂点バッファインターフェースへのポインタ
		NULL)))					// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBomb->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BOMB_WIDTH / 2, BOMB_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BOMB_WIDTH / 2, BOMB_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BOMB_WIDTH / 2, -BOMB_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BOMB_WIDTH / 2, -BOMB_HEIGHT / 2, 0.0f);

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
		g_pVtxBuffBomb->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBomb(int nIdxBomb, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBomb->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffBomb->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetBomb(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int nIdxBomb = -1;

	for (int i = 0; i < MAX_BOMB; i++)
	{
		if (!g_Bomb[i].bUse && g_BombMgr.UseTimer < 1)
		{
			g_Bomb[i].pos = pos;
			g_Bomb[i].pos.y = BOMB_HEIGHT / 6;
			g_Bomb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Bomb[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bomb[i].dir = dir;
			g_Bomb[i].Life = 5;
			g_Bomb[i].timer = BOMB_LIFE;
			g_Bomb[i].bUse = true;

			PlaySound(SOUND_LABEL_SE_BOMB01);

			// 影の設定
			g_Bomb[i].idxShadow = CreateShadow(pos, 8.0f, 8.0f);		// 影の設定

			nIdxBomb = i;

			break;
		}
	}
	return nIdxBomb;
}

BOMB* GetBomb()
{
	return g_Bomb;
}

BOMBMGR* GetBombMgr()
{
	return &g_BombMgr;
}

SPHERE* Bomb_GetSphere(int index)
{
	return (&g_Bomb[index].col_Sphere);
}

void Bomb_Destroy(int index)
{
	g_Bomb[index].bUse = false;
	ReleaseShadow(g_Bomb[index].idxShadow);
}

void Bomb_LifeDown(int index)
{
	g_Bomb[index].Life--;
}

bool Bomb_IsEnable(int index)
{
	return g_Bomb[index].bUse;
}
