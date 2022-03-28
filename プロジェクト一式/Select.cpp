//=============================================================================
//
// プレイヤーセレクトシーン管理 [Select.cpp]
// Author : 
//
//=============================================================================
#include "input.h"
#include "Select.h"
#include "main.h"
#include "MyDirect3D.h"
#include "debugproc.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "sound.h"
#include "sprite.h"
#include "fade.h"
#include <time.h>
#include "light.h"
#include "meshfield.h"
#include "meshsky.h"
#include "player.h"
#include "UI.h"
#include "scene.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER1		"data/MODEL/Rondini_MK1(ver2).x"	// 読み込むモデル名
#define	MODEL_PLAYER2		"data/MODEL/Rondini_MK2(ver2).x"	// 読み込むモデル名
#define	MODEL_PLAYER3		"data/MODEL/Rondini_MK3(ver5).x"	// 読み込むモデル名

#define	VALUE_MOVE		(2.0f)					// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.01f)		// 回転量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPD3DXMESH			g_pMesh[MAX_PLAYER] = { NULL };		// メッシュ情報へのポインタ
static LPD3DXBUFFER			g_pBuffMat[MAX_PLAYER] = { NULL };	// マテリアル情報へのポインタ
static DWORD				g_nNumMat[MAX_PLAYER] = { 0 };		// マテリアル情報の数
static LPDIRECT3DTEXTURE9	g_pTexture = NULL;	// テクスチャ

static SELECT				g_Select;			// プレイヤー
static float				g_Rotate;
static int					g_Pattern;
static D3DXVECTOR3			g_StandardPos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSelect(void)
{
	// ランダムシードの初期化
	srand((unsigned int)time(NULL));

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 影の初期化
	InitShadow();

	// UIの初期化
	InitUI();


	// 空の初期化処理
	InitMeshSky(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8000.0f, 16, 8);


	// 地面の初期化処理
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0), 100, 100, 50.0f, 50.0f);

	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER1,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&g_pBuffMat[0],			// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat[0],				// D3DXMATERIAL構造体の数
		&g_pMesh[0])))				// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER2,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&g_pBuffMat[1],			// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat[1],				// D3DXMATERIAL構造体の数
		&g_pMesh[1])))				// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER3,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&g_pBuffMat[2],			// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMat[2],				// D3DXMATERIAL構造体の数
		&g_pMesh[2])))				// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	D3DXMATERIAL *pMat;
	// マテリアル情報に対するポインタを取得
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		pMat = (D3DXMATERIAL*)g_pBuffMat[i]->GetBufferPointer();

		// マテリアルにテクスチャが登録されていれば読み込む
		if (pMat[i].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
				pMat[i].pTextureFilename,			// ファイルの名前
				&g_pTexture);						// 読み込むメモリー
		}
	}

	// 位置・回転・スケールの初期設定
	g_Select.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_Select.rot = D3DXVECTOR3(0.0f, D3DX_PI / 4	, 0.0f);
	g_Select.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Select.life = 0;
	g_Select.spd = 0;

	//影の初期化
	g_Select.idxShadow = CreateShadow(g_Select.pos, 30.0f, 30.0f);


	g_Pattern = 0;
	g_Rotate = 0.0f;


	g_Select.pos.x = g_StandardPos.x + (cosf(g_Rotate));  //回転する時の位置
	g_Select.pos.z = g_StandardPos.z + (sinf(g_Rotate));  //回転する時の位置
	

	CAMERA *cam = GetCamera();

	cam->posR.x = g_Select.pos.x;
	cam->posR.z = g_Select.pos.z;
	cam->posV.x = cam->posR.x - sinf(g_Select.rot.y - (D3DX_PI / 3)) * cam->fDistance;
	cam->posV.z = cam->posR.z - cosf(g_Select.rot.y - (D3DX_PI / 3)) * cam->fDistance;

	PlaySound(SOUND_LABEL_SELECTBGM);



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSelect(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// 空の終了処理
	UninitMeshSky();

	// 地面の終了処理
	UninitMeshField();

	// 影の終了処理
	UninitShadow();

	// UIの終了処理
	UninitShadow();

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (g_pTexture != NULL)
		{// テクスチャの開放
			g_pTexture->Release();
			g_pTexture = NULL;
		}

		if (g_pMesh != NULL)
		{// メッシュの開放
			g_pMesh[i]->Release();
			g_pMesh[i] = NULL;
		}

		if (g_pBuffMat != NULL)
		{// マテリアルの開放
			g_pBuffMat[i]->Release();
			g_pBuffMat[i] = NULL;
		}
	}

	StopSound(SOUND_LABEL_SELECTBGM);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSelect(void)
{

	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	// 空処理の更新処理
	UpdateMeshSky();

	// 地面の更新処理
	UpdateMeshField();

	// UIの更新処理
	UpdateUI();

	CAMERA *cam = GetCamera();
	   	 
	//切り替え
	if (Keyboard_IsTrigger(DIK_A))
	{
		g_Pattern -= 1;
		if (g_Pattern < 0)
		{
			g_Pattern = 2;
		}

	}
	if (Keyboard_IsTrigger(DIK_D))
	{
		g_Pattern += 1;
		if (g_Pattern > 2)
		{
			g_Pattern = 0;
		}
	}

	switch (g_Pattern)
	{
	case 0:
		g_Select.life = 40;
		g_Select.spd = 30;
		g_Select.atk = 30;
		break;
	case 1:
		g_Select.life = 30;
		g_Select.spd = 40;
		g_Select.atk = 30;
		break;
	case 2:
		g_Select.life = 50;
		g_Select.spd = 20;
		g_Select.atk = 40;
		break;
	default:
		break;
	}

	g_Select.rot.y += VALUE_ROTATE;
	   	   	 
	// 座標の更新
	//g_Select.rot.y = g_Select.rot.y + cam->rot.y;	

	// 影の座標を更新する
	D3DXVECTOR3 shadowPos = g_Select.pos;
	shadowPos.y = 0.0f;
	SetPositionShadow(g_Select.idxShadow, D3DXVECTOR3(g_Select.pos.x, 0.0f, g_Select.pos.z));
	SetScaleShadow(g_Select.idxShadow, D3DXVECTOR3(50.0f - (g_Select.pos.y / 2), 0.0f, 50.0f - (g_Select.pos.y / 2)));

	//バックスペースが押されたらシーン遷移する	
	if (Keyboard_IsTrigger(DIK_BACKSPACE))
	{
		//クリアカウント次第でフェードするシーンを決める
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			if (GetClearCnt() == 0)
			{
				Fade_SceneTransition(SCENE_INDEX_GAME);
			}
			if (GetClearCnt() == 1)
			{
				Fade_SceneTransition(SCENE_INDEX_GAME2);
			}
			if (GetClearCnt() == 2)
			{
				Fade_SceneTransition(SCENE_INDEX_GAME3);
			}
			if (GetClearCnt() == 3)
			{
				Fade_SceneTransition(SCENE_INDEX_GAME4);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSelect(void)
{
	// カメラの設定
	SetCamera();

	// 空処理の描画処理
	DrawMeshSky();

	// 地面の描画処理
	DrawMeshField();

	// 影の描画処理
	DrawShadow();

	// UIの描画処理
	DrawUI();

	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Select.mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_Select.scl.x, g_Select.scl.y, g_Select.scl.z);
	D3DXMatrixMultiply(&g_Select.mtxWorld, &g_Select.mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Select.rot.y, g_Select.rot.x, g_Select.rot.z);
	D3DXMatrixMultiply(&g_Select.mtxWorld, &g_Select.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_Select.pos.x, g_Select.pos.y, g_Select.pos.z);
	D3DXMatrixMultiply(&g_Select.mtxWorld, &g_Select.mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Select.mtxWorld);


	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMat[g_Pattern]->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMat[g_Pattern]; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定(テクスチャの有るモデルを読み込むときは注意)
		pDevice->SetTexture(0, g_pTexture);

		// 描画
		g_pMesh[g_Pattern]->DrawSubset(nCntMat);
	}
	
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

int SelectPlayer(void)
{
	return g_Pattern;
}

void SetSelectPlayer(int index)
{
	g_Pattern = index;
}

SELECT* GetSelect()
{
	return &g_Select;
}