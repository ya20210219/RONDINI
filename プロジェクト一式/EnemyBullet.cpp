//=============================================================================
//
// 敵の弾処理 [Enemybullet.cpp]
// Author : 
//
//=============================================================================
#include "Enemybullet.h"
#include "MyDirect3D.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ENEMY_TEXTURE_BULLET		"data/TEXTURE/EnemyBullet.png"	// 読み込むテクスチャファイル名
#define	ENEMY_BULLET_WIDTH		(5.0f)							// 弾の幅
#define	ENEMY_BULLET_HEIGHT		(5.0f)							// 弾の高さ
#define	ENEMY_VALUE_MOVE_BULLET	(0.05f)							// 移動速度

#define ENEMY_BULLET_LIFE			(200)

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
	int				idxShadow;	// 影ID
	bool			bUse;		// 使用しているかどうかのフラグ

	SPHERE			col_Sphere;

} BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemyBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEnemyBullet(int nIdxEnemyBullet, float fWidth, float fHeight);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureEnemyBullet = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffEnemyBullet = NULL;	// 頂点バッファへのポインタ

static BULLET					g_EnemyBullet[MAX_ENEMY_BULLET];		// 弾ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// 頂点情報の作成
	MakeVertexEnemyBullet(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		ENEMY_TEXTURE_BULLET,			// ファイルの名前
		&g_pTextureEnemyBullet);		// 読み込むメモリー

// バレット配列の初期化
	for (int i = 0; i < MAX_ENEMY_BULLET; i++)
	{
		g_EnemyBullet[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyBullet[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyBullet[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_EnemyBullet[i].dir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_EnemyBullet[i].timer = 0;
		g_EnemyBullet[i].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemyBullet(void)
{
	if (g_pTextureEnemyBullet != NULL)
	{// テクスチャの開放
		g_pTextureEnemyBullet->Release();
		g_pTextureEnemyBullet = NULL;
	}

	if (g_pVtxBuffEnemyBullet != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffEnemyBullet->Release();
		g_pVtxBuffEnemyBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemyBullet(void)
{
	//弾の配列を全部調べる
	for (int i = 0; i < MAX_ENEMY_BULLET; i++)
	{
		//有効かフラグがONの物のみ処理を行う
		if (g_EnemyBullet[i].bUse)
		{
			// 移動
			g_EnemyBullet[i].pos.x += g_EnemyBullet[i].dir.x;
			g_EnemyBullet[i].pos.z += g_EnemyBullet[i].dir.z;

			g_EnemyBullet[i].col_Sphere.pos.x = g_EnemyBullet[i].pos.x;
			g_EnemyBullet[i].col_Sphere.pos.y = g_EnemyBullet[i].pos.y;
			g_EnemyBullet[i].col_Sphere.pos.z = g_EnemyBullet[i].pos.z;

			g_EnemyBullet[i].col_Sphere.r = 5.0f;

			// 生存フレームのタイマーを減らす
			g_EnemyBullet[i].timer--;

			// 生存フレームが0以下になったら消去する
			if (g_EnemyBullet[i].timer <= 0)
			{
				// 影の破棄
				ReleaseShadow(g_EnemyBullet[i].idxShadow);

				g_EnemyBullet[i].bUse = false;
			}
			else
			{
				// 影の位置設定
				SetPositionShadow(g_EnemyBullet[i].idxShadow, D3DXVECTOR3(g_EnemyBullet[i].pos.x, 0.1f, g_EnemyBullet[i].pos.z));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_ENEMY_BULLET; i++)
	{
		if (g_EnemyBullet[i].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_EnemyBullet[i].mtxWorld);

			D3DXMATRIX mtxView;

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

#if 0
			// 逆行列を求める
			D3DXMatrixInverse(&g_EnemyBullet[i].mtxWorld, NULL, &mtxView);

			// 平行移動の部分は無視する
			g_EnemyBullet[i].mtxWorld._41 = 0.0f;
			g_EnemyBullet[i].mtxWorld._42 = 0.0f;
			g_EnemyBullet[i].mtxWorld._43 = 0.0f;
#else
			//転置行列
			g_EnemyBullet[i].mtxWorld._11 = mtxView._11;
			g_EnemyBullet[i].mtxWorld._12 = mtxView._21;
			g_EnemyBullet[i].mtxWorld._13 = mtxView._31;
			g_EnemyBullet[i].mtxWorld._21 = mtxView._12;
			g_EnemyBullet[i].mtxWorld._22 = mtxView._22;
			g_EnemyBullet[i].mtxWorld._23 = mtxView._32;
			g_EnemyBullet[i].mtxWorld._31 = mtxView._13;
			g_EnemyBullet[i].mtxWorld._32 = mtxView._23;
			g_EnemyBullet[i].mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_EnemyBullet[i].scale.x,
				g_EnemyBullet[i].scale.y,
				g_EnemyBullet[i].scale.z);
			D3DXMatrixMultiply(&g_EnemyBullet[i].mtxWorld, &g_EnemyBullet[i].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_EnemyBullet[i].pos.x,
				g_EnemyBullet[i].pos.y,
				g_EnemyBullet[i].pos.z);
			D3DXMatrixMultiply(&g_EnemyBullet[i].mtxWorld, &g_EnemyBullet[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemyBullet[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffEnemyBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemyBullet);

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
HRESULT MakeVertexEnemyBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		FVF_VERTEX_3D,			// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffEnemyBullet,		// 頂点バッファインターフェースへのポインタ
		NULL)))					// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEnemyBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-ENEMY_BULLET_WIDTH / 2, ENEMY_BULLET_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENEMY_BULLET_WIDTH / 2, ENEMY_BULLET_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMY_BULLET_WIDTH / 2, -ENEMY_BULLET_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENEMY_BULLET_WIDTH / 2, -ENEMY_BULLET_HEIGHT / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pVtxBuffEnemyBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemyBullet(int nIdxEnemyBullet, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEnemyBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fWidth / 2, fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, -fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fWidth / 2, -fHeight / 2, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffEnemyBullet->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int nIdxEnemyBullet = -1;

	for (int i = 0; i < MAX_ENEMY_BULLET; i++)
	{
		if (!g_EnemyBullet[i].bUse)
		{
			g_EnemyBullet[i].pos = pos;
			g_EnemyBullet[i].pos.y = ENEMY_BULLET_HEIGHT / 2;
			g_EnemyBullet[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_EnemyBullet[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_EnemyBullet[i].dir = dir;
			g_EnemyBullet[i].timer = ENEMY_BULLET_LIFE;
			g_EnemyBullet[i].bUse = true;

			// 影の設定
			g_EnemyBullet[i].idxShadow = CreateShadow(pos, 8.0f, 8.0f);		// 影の設定

			nIdxEnemyBullet = i;

			break;
		}
	}

	return nIdxEnemyBullet;
}

BULLET* GetEnemyBullet()
{
	return g_EnemyBullet;
}

SPHERE* EnemyBullet_GetSphere(int index)
{
	return (&g_EnemyBullet[index].col_Sphere);
}

void EnemyBullet_Destroy(int index)
{
	g_EnemyBullet[index].bUse = false;
	ReleaseShadow(g_EnemyBullet[index].idxShadow);
}

bool EnemyBullet_IsEnable(int index)
{
	return g_EnemyBullet[index].bUse;
}
