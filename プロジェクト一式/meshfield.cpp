//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "meshfield.h"
#include "MyDirect3D.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	TEXTURE_FILENAME	"data/TEXTURE/field001.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_FILENAME	"data/TEXTURE/teppan02.png"		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9 g_pTextureField;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField;		// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9 g_pIdxBuffField;		// インデックスバッファへのポインタ

static D3DXMATRIX g_mtxWorldField;					// ワールドマトリックス
static D3DXVECTOR3 g_posField;						// ポリゴン表示位置の中心座標
static D3DXVECTOR3 g_rotField;						// ポリゴンの回転角

static DWORD g_nNumVertex;
static DWORD g_nNumVertexIndex;
static DWORD g_nNumPolygon;

static DWORD g_NumBlockX;
static DWORD g_NumBlockZ;
static float g_BlockSizeX;
static float g_BlockSizeZ;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int numBlockX, int numBlockZ, float sizeBlockX, float sizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	HRESULT hr;

	// 位置・向きの初期設定
	g_posField = pos;
	g_rotField = rot;

	g_NumBlockX = numBlockX;
	g_NumBlockZ = numBlockZ;
	g_BlockSizeX = sizeBlockX;
	g_BlockSizeZ = sizeBlockZ;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_FILENAME,	// ファイルの名前
								&g_pTextureField);	// 読み込むメモリー


	// 頂点情報の作成
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	if(g_pVtxBuffField)
	{// 頂点バッファの開放
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}

	if(g_pIdxBuffField)
	{// インデックスバッファの開放
		g_pIdxBuffField->Release();
		g_pIdxBuffField = NULL;
	}

	if(g_pTextureField)
	{// テクスチャの開放
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(g_pIdxBuffField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureField);

	// ポリゴンの描画
	// Type            : プリミティブの種類
	// BaseVertexIndex : 最初の頂点インデックスまでのオフセット
	// MinIndex        : 最小の頂点インデックス
	// NumVertices     : 頂点の数
	// StartIndex      : 読み取りを開始する位置
	// PrimitiveCount  : レンダリングするプリミティブの数
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertex, 0, g_nNumPolygon);

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//□（四角）の数
	int block_x = g_NumBlockX;
	int block_y = g_NumBlockZ;

	//頂点数
	g_nNumVertex = (block_x + 1) * (block_y + 1);
	//インデックス数
	g_nNumVertexIndex = (block_x + 1) * 2 * block_y + 2 * (block_y - 1);
	//ポリゴン数
	g_nNumPolygon = block_x * 2 * block_y + 4 * (block_y - 1);

	float size_x = g_BlockSizeX;
	float size_z = g_BlockSizeZ;

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffField,			// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumVertexIndex,	// インデックスデータ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// インデックスバッファの使用法　
												D3DFMT_INDEX16,				// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pIdxBuffField,			// インデックスバッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		const float fTexSizeX = 1.0f;
		const float fTexSizeZ = 1.0f;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		{
			for (int y = 0; y < (block_y + 1); y++)
			{
				for (int x = 0; x < (block_x + 1); x++)
				{
					// 頂点座標の設定
					pVtx[y*(block_x + 1) + x].pos.x = (-size_x / 2) * block_x + size_x * x;
					pVtx[y*(block_x + 1) + x].pos.y = 0.0f;
					pVtx[y*(block_x + 1) + x].pos.z = ( size_z / 2) * block_y - size_z * y;

					// 法線の設定
					pVtx[y*(block_x + 1) + x].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

					// 頂点カラーの設定
					pVtx[y*(block_x + 1) + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャ座標の設定
					pVtx[y*(block_x + 1) + x].tex.x = (float)x;
					pVtx[y*(block_x + 1) + x].tex.y = (float)y;
				}
			}
		}

		// 頂点データをアンロックする
		g_pVtxBuffField->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		int index = 0;

		//(block_y+1)にすると回りすぎてしまう
		for (int y = 0; y < block_y; y++)
		{
			//1行分の処理
			for (int x = 0; x < (block_x + 1); x++)
			{
				pIdx[index] = ((block_x + 1) + x) + ((block_x + 1)*y);
				index++;
				pIdx[index] = (0 + x) + ((block_x + 1)*y);
				index++;
			}

			//最後の行はこの処理を行わない
			if (y < (block_y-1))
			{
				//縮退ポリゴン分のインデックスの設定
				pIdx[index] = block_x + ((block_x + 1)*y);
				index++;
				pIdx[index] = ((block_x + 1) * 2) + ((block_x + 1)*y);
				index++;
			}
		}

		// インデックスデータをアンロックする
		g_pIdxBuffField->Unlock();
	}

	return S_OK;
}
