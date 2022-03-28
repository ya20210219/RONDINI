//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author :
//
//=============================================================================
#include "camera.h"
#include "MyDirect3D.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_V_X				(0.0f)					// カメラの視点初期位置(X座標)
#define	CAM_POS_V_Y				(100.0f)				// カメラの視点初期位置(Y座標)
#define	CAM_POS_V_Z				(-200.0f)				// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X				(0.0f)					// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y				(60.0f)					// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z				(0.0f)					// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE				(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z				(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z				(100000.0f)										// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA		(2.0f)											// カメラの移動量
#define	VALUE_ROTATE_CAMERA		(D3DX_PI * 0.01f)								// カメラの回転量
#define	VALUE_ADJUST_DISTANCE	(2.0f)											// 視点と注視点の距離調整量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA g_camera;		// カメラ情報

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, 50.0f, CAM_POS_R_X);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fVecX,fVecZ;
	fVecX = g_camera.posV.x - g_camera.posR.x;
	fVecZ = g_camera.posV.z - g_camera.posR.z;
	g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	/*if(Keyboard_IsPress(DIK_Z))
	{// 視点旋回「左」
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if(g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if(Keyboard_IsPress(DIK_C))
	{// 視点旋回「右」
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if(g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}

		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if(Keyboard_IsPress(DIK_Y))
	{// 視点移動「上」
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if(Keyboard_IsPress(DIK_N))
	{// 視点移動「下」
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}


	if(Keyboard_IsPress(DIK_Q))
	{// 注視点旋回「左」
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if(g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if(Keyboard_IsPress(DIK_E))
	{// 注視点旋回「右」
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if(g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	*/

	if(Keyboard_IsPress(DIK_T))
	{// 注視点移動「上」
		g_camera.posR.y += VALUE_MOVE_CAMERA;
	}
	if(Keyboard_IsPress(DIK_B))
	{// 注視点移動「下」
		g_camera.posR.y -= VALUE_MOVE_CAMERA;
	}

	if(Keyboard_IsPress(DIK_U))
	{// 視点と注視点の距離縮める
		g_camera.fDistance -= VALUE_ADJUST_DISTANCE;
		if(g_camera.fDistance < 100.0f)
		{
			g_camera.fDistance = 100.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if(Keyboard_IsPress(DIK_M))
	{// 視点と注視点の距離伸ばす
		g_camera.fDistance += VALUE_ADJUST_DISTANCE;
		if(g_camera.fDistance > 300.0f)
		{
			g_camera.fDistance = 300.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	/*if(Keyboard_IsPress(DIK_RETURN))
	{// リセット
		g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
		g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float fVecX,fVecZ;
		fVecX = g_camera.posV.x - g_camera.posR.x;
		fVecZ = g_camera.posV.z - g_camera.posR.z;
		g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
	}*/

	/*
	PrintDebugProc("[カメラの視点  ：(%f : %f : %f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	PrintDebugProc("[カメラの注視点：(%f : %f : %f)]\n", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z);
	PrintDebugProc("[カメラの向き  ：(%f)]\n", g_camera.rot.y);
	PrintDebugProc("\n");

	PrintDebugProc("*** 注視点操作 ***\n");
	PrintDebugProc("上移動 : T\n");
	PrintDebugProc("下移動 : B\n");
	PrintDebugProc("\n");

	PrintDebugProc("視点と注視点の距離調整 : U / M\n");
	PrintDebugProc("後ろを向く             : C\n");

	PrintDebugProc("カメラの視点・注視点リセット ： ENTER\n");
	PrintDebugProc("\n");

	PrintDebugProc("*** プレイヤー操作 ***\n");
	PrintDebugProc("スピードアップ : W\n");
	PrintDebugProc("減速           : S\n");
	PrintDebugProc("左             : A\n");
	PrintDebugProc("右             : D\n");
	PrintDebugProc("弾発射         : P\n");
	PrintDebugProc("ジャンプ       : SPACE\n");
	PrintDebugProc("\n");
	*/
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice(); 

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,			// ビュー平面の視野角
								VIEW_ASPECT,		// ビュー平面のアスペクト比
								VIEW_NEAR_Z,		// ビュー平面のNearZ値
								VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// カメラの視点
						&g_camera.posR,		// カメラの注視点
						&g_camera.vecU);		// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void)
{
	return &g_camera;
}

