//=============================================================================
//
// メッシュ空の処理 [meshsky.cpp]
// Author :
//
//=============================================================================
#include "meshsky.h"
#include "MyDirect3D.h"
#include "input.h"
#include "camera.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME1		"data/TEXTURE/sky002.jpg"		// 読み込むテクスチャファイル名 sky002
#define	TEXTURE_FILENAME2		"data/TEXTURE/sky004.jpg"		// 読み込むテクスチャファイル名 sky002
#define	VALUE_MOVE_SKY			(5.0f)							// 移動速度
#define	VALUE_ROTATE_SKY		(D3DX_PI * 0.001f)				// 回転速度

#define NUM_DIVIDE_H			(16)
#define NUM_DIVIDE_V			(8)
#define	NUM_VERTEX_CYLINDER		((NUM_DIVIDE_H + 1) * (NUM_DIVIDE_V + 1))						// 頂点数
#define	NUM_POLYGON_CYLINDER	(NUM_DIVIDE_H * (NUM_DIVIDE_V * 2) + (NUM_DIVIDE_V - 1) * 4)	// ポリゴン数
#define	NUM_VERTEX_INDEX		(NUM_DIVIDE_H * (NUM_DIVIDE_V * 2) + NUM_DIVIDE_V * 2 + (NUM_DIVIDE_V - 1) * 2)
#define	SKY_RADIUS				(30000.0f)	// 空ドームの半径	初期値8000
#define	SKY_HEIGHT_RATE			(2.0f)		// 空ドームの高さ係数

#define	TEX_COUNT_LOOP			(1)			// テクスチャの繰り返し回数


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSky;			// テクスチャ読み込み場所
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSky;		// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSky;		// インデックスバッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkyTop;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSkyTop;	// インデックスバッファへのポインタ

D3DXMATRIX g_mtxWorldSky;					// ワールドマトリックス
D3DXVECTOR3 g_posSky;						// ポリゴン表示位置の中心座標
D3DXVECTOR3 g_rotSky;						// ポリゴンの回転角

bool g_bTransParent;
bool g_bReverse;

int g_nNumBlockHSky, g_nNumBlockVSky;		// ブロック数
int g_nNumVertexSky;						// 総頂点数	
int g_nNumVertexIndexSky;					// 総インデックス数
int g_nNumPolygonSky;						// 総ポリゴン数
float g_fRadius;							// 半径

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius,
									int nNumBlockH, int nNumBlockV, bool bReverse)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	g_bTransParent = false;

	g_bReverse = bReverse;

	// ポリゴン表示位置の中心座標を設定
	g_posSky = pos;

	g_rotSky = rot;

	g_fRadius = fRadius;

	if (GetSceneNumber() != SCENE_INDEX_GAME4)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FILENAME1,		// ファイルの名前
			&g_pTextureSky);		// 読み込むメモリー
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME4)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FILENAME2,		// ファイルの名前
			&g_pTextureSky);		// 読み込むメモリー
	}


	// ブロック数の設定
	g_nNumBlockHSky = nNumBlockH;
	g_nNumBlockVSky = nNumBlockV;

	// 頂点数の設定
	g_nNumVertexSky = (nNumBlockH + 1) * (nNumBlockV + 1);

	// ポリゴン数の設定
	g_nNumPolygonSky = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);

	// インデックス数の設定
	g_nNumVertexIndexSky = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX_CYLINDER,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_3D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffSky,						// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (NUM_DIVIDE_H + 1),	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_3D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffSkyTop,					// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * NUM_VERTEX_INDEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												D3DFMT_INDEX16,				// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pIdxBuffSky,				// インデックスバッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * (NUM_DIVIDE_H + 2),	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												D3DFMT_INDEX16,				// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pIdxBuffSkyTop,			// インデックスバッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		const float ANGLE_H = (D3DX_PI * 2.0f) / NUM_DIVIDE_H;
		const float ANGLE_V = (D3DX_PI / 8.0f) / (NUM_DIVIDE_V + 1);
		const float WIDTH = 1.0f / NUM_DIVIDE_H;
		const float HEIGHT = 1.0f / NUM_DIVIDE_V;
		float fLengthXZ;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffSky->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntV = 0; nCntV < (NUM_DIVIDE_V + 1); nCntV++)
		{
			fLengthXZ = cosf(ANGLE_V * nCntV) * g_fRadius;

			for(int nCntH = 0; nCntH < (NUM_DIVIDE_H + 1); nCntH++, pVtx++)
			{
				// 頂点座標の設定
				pVtx->pos.x = sinf(ANGLE_H * nCntH) * fLengthXZ;
				if(g_bReverse)
				{
					pVtx->pos.y = -sinf(ANGLE_V * nCntV) * g_fRadius * SKY_HEIGHT_RATE;
				}
				else
				{
					pVtx->pos.y = sinf(ANGLE_V * nCntV) * g_fRadius * SKY_HEIGHT_RATE;
				}
				pVtx->pos.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

				// 法線の設定
				pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 頂点カラーの設定
				if(g_bReverse)
				{
					pVtx->col = D3DXCOLOR(0.0625f, 0.0625f, 0.375f, 0.5f);
					pVtx->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				}
				else
				{
					pVtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					//pVtx->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				}

				// テクスチャ座標の設定
				pVtx->tex.x = nCntH * WIDTH * TEX_COUNT_LOOP;

				if(nCntV == NUM_DIVIDE_V)
				{
					pVtx->tex.y = 0.01f;
				}
				else
				{
					pVtx->tex.y = 1.0f - nCntV * HEIGHT;
				}
			}
		}

		// 頂点データをアンロックする
		g_pVtxBuffSky->Unlock();

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffSkyTop->Lock( 0, 0, (void**)&pVtx, 0 );

		// 頂点座標の設定
		pVtx->pos.x = 0.0f;
		if(g_bReverse)
		{
			pVtx->pos.y = -sinf(ANGLE_V * (NUM_DIVIDE_V + 1)) * g_fRadius * SKY_HEIGHT_RATE;
		}
		else
		{
			pVtx->pos.y = sinf(ANGLE_V * (NUM_DIVIDE_V + 1)) * g_fRadius * SKY_HEIGHT_RATE;
		}
		pVtx->pos.z = 0.0f;

		// 法線の設定
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		if(g_bReverse)
		{
			pVtx->col = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
		}
		else
		{
			pVtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ座標の設定
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;

		pVtx++;

		fLengthXZ = cosf(ANGLE_V * NUM_DIVIDE_V) * g_fRadius;
		for(int nCntH = 0; nCntH < NUM_DIVIDE_H; nCntH++, pVtx++)
		{
			// 頂点座標の設定
			pVtx->pos.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
			if(g_bReverse)
			{
				pVtx->pos.y = -sinf(ANGLE_V * NUM_DIVIDE_V) * g_fRadius * SKY_HEIGHT_RATE;
			}
			else
			{
				pVtx->pos.y = sinf(ANGLE_V * NUM_DIVIDE_V) * g_fRadius * SKY_HEIGHT_RATE;
			}
			pVtx->pos.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

			// 法線の設定
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			if(g_bReverse)
			{
				pVtx->col = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
			}
			else
			{
				pVtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// テクスチャ座標の設定
			pVtx->tex.x = 0.01f;
			pVtx->tex.y = 0.01f;
		}

		// 頂点データをアンロックする
		g_pVtxBuffSkyTop->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		g_pIdxBuffSky->Lock( 0, 0, (void**)&pIdx, 0 );

		for(int nCntV = 0; nCntV < NUM_DIVIDE_V; nCntV++)
		{
			if(nCntV != 0)
			{
				*pIdx = nCntV * (NUM_DIVIDE_H + 1);
				pIdx++;
			}

			for(int nCntH = 0; nCntH < NUM_DIVIDE_H + 1; nCntH++)
			{
				*pIdx = nCntV * (NUM_DIVIDE_H + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (NUM_DIVIDE_H + 1) + nCntH;
				pIdx++;
			}

			if(nCntV != (NUM_DIVIDE_V - 1))
			{
				*pIdx = (nCntV + 1) * (NUM_DIVIDE_H + 1);
				pIdx++;
			}
		}

		// インデックスデータをアンロックする
		g_pIdxBuffSky->Unlock();

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		g_pIdxBuffSkyTop->Lock( 0, 0, (void**)&pIdx, 0 );

		for(int nCntH = 0; nCntH < NUM_DIVIDE_H + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// インデックスデータをアンロックする
		g_pIdxBuffSkyTop->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshSky(void)
{
	if(g_pVtxBuffSky)
	{// 頂点バッファの開放
		g_pVtxBuffSky->Release();
		g_pVtxBuffSky = NULL;
	}

	if(g_pIdxBuffSky)
	{// インデックスバッファの開放
		g_pIdxBuffSky->Release();
		g_pIdxBuffSky = NULL;
	}

	if(g_pVtxBuffSkyTop)
	{// 頂点バッファの開放
		g_pVtxBuffSkyTop->Release();
		g_pVtxBuffSkyTop = NULL;
	}

	if(g_pIdxBuffSkyTop)
	{// インデックスバッファの開放
		g_pIdxBuffSkyTop->Release();
		g_pIdxBuffSkyTop = NULL;
	}

	if(g_pTextureSky)
	{// テクスチャの開放
		g_pTextureSky->Release();
		g_pTextureSky = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshSky(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	for (int i = 0; i < 2; i++)
	{
		// ライティングを無効に
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldSky);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotSky.x, g_rotSky.y + ( D3DX_PI * i), g_rotSky.z);	//yxzの順で並んでいた
		D3DXMatrixMultiply(&g_mtxWorldSky, &g_mtxWorldSky, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_posSky.x, g_posSky.y, g_posSky.z);
		D3DXMatrixMultiply(&g_mtxWorldSky, &g_mtxWorldSky, &mtxTranslate);

		if (g_bTransParent)
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			// Zバッファを使用しない
		}
		else
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			// Zバッファを使用する
		}

		if (g_bReverse)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	// 表面をカリング
		}
		else
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSky);

		// 頂点バッファをレンダリングパイプラインに設定
		pDevice->SetStreamSource(0, g_pVtxBuffSky, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		pDevice->SetIndices(g_pIdxBuffSky);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSky);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_VERTEX_CYLINDER, 0, NUM_POLYGON_CYLINDER);


		// 頂点バッファをレンダリングパイプラインに設定
		pDevice->SetStreamSource(0, g_pVtxBuffSkyTop, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		pDevice->SetIndices(g_pIdxBuffSkyTop);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (NUM_DIVIDE_H + 2), 0, NUM_DIVIDE_H);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング

		// ライティングを有効に
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}
}

