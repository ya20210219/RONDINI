//=============================================================================
//
// プレイヤー管理 [player.cpp]
// Author :
//
//=============================================================================
#include "input.h"
#include "player.h"
#include "MyDirect3D.h"
#include "debugproc.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "sound.h"
#include "sprite.h"
#include "Select.h"
#include "Lockon.h"
#include "LockonBullet.h"
#include "AbilityThunder.h"
#include "AbilityBomb.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER1		"data/MODEL/Rondini_MK1(ver2).x"	// 読み込むモデル名
#define	MODEL_PLAYER2		"data/MODEL/Rondini_MK2(ver2).x"	// 読み込むモデル名
#define	MODEL_PLAYER3		"data/MODEL/Rondini_MK3(ver5).x"	// 読み込むモデル名

#define	VALUE_MOVE		(2.0f)					// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.01f)		// 回転量
#define JUMP_FORCE		(5.0f)
#define GRAVITY			(0.5f)
#define MAX_LIFE		(50)

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

static PLAYER				g_Player;			// プレイヤー
static int					g_BulletCnt;
static int					g_FullLife;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(MODEL_PLAYER1,				// 読み込むモデルファイル名(Xファイル)
								D3DXMESH_SYSTEMMEM,			// メッシュの作成オプションを指定
								pDevice,					// IDirect3DDevice9インターフェイスへのポインタ
								NULL,						// 隣接性データを含むバッファへのポインタ
								&g_pBuffMat[0],				// マテリアルデータを含むバッファへのポインタ
								NULL,						// エフェクトインスタンスの配列を含むバッファへのポインタ
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
	g_Player.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_Player.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Player.life = MAX_LIFE;
	g_Player.bUse = true;

	g_Player.Pattern = SelectPlayer();
	
	if (GetSceneNumber() == SCENE_INDEX_TUTORIAL)
	{
		g_Player.Pattern = 0;
	}

	if (g_Player.Pattern == 0)
	{
		g_Player.PlayerSpd = 1.5f;
	}
	if (g_Player.Pattern == 1)
	{
		g_Player.PlayerSpd = 1.8f;
		g_Player.life = g_Player.life / 5 * 4;	//40
	}
	if (g_Player.Pattern == 2)
	{
		g_Player.PlayerSpd = 1.2f;
		g_Player.life = g_Player.life / 5 * 6;	//60
	}

	g_FullLife = g_Player.life;

	g_Player.PlayerSpd = 1.5f;
	g_BulletCnt = 0;

	//影の初期化
	g_Player.idxShadow = CreateShadow(g_Player.pos, 30.0f, 30.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	g_Player.bUse = false;

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

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	LOCKONRANGE* pLockOnRange = GetLockonRange();
	LOCKON* pLock = GetLockOn();
	LOCKONBULLET* pLBullet = GetLockonBullet();
	AbilityThunder * pAT = GetAbilityThunder();
	BOMBMGR* pAB = GetBombMgr();

	if (g_Player.life < 1)
	{
		g_Player.bUse = false;
	}

	if (g_Player.bUse == true)
	{
		g_Player.speed.x = sinf(D3DX_PI * 0.0f - g_Player.rot.y) * g_Player.PlayerSpd;
		g_Player.speed.z = cosf(D3DX_PI * 0.0f - g_Player.rot.y) * g_Player.PlayerSpd;
	
		if (Keyboard_IsPress(DIK_A))
		{
			if (Keyboard_IsPress(DIK_W))
			{
				g_Player.speed.x = sinf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);	//g_PlayerSpd / 3 * 4
				g_Player.speed.z = cosf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);

				/*
				g_Player.pos.x += g_Player.speed.x;
				g_Player.pos.z -= g_Player.speed.z;
				*/
			}

			g_Player.rot.y -= VALUE_ROTATE;
			g_Player.rot.z -= VALUE_ROTATE;
			if (g_Player.rot.y < -D3DX_PI)
			{
				g_Player.rot.y += D3DX_PI * 2.0f;
			}

			if (g_Player.rot.z < -D3DX_PI / 4)
			{
				g_Player.rot.z = -D3DX_PI / 4;
			}
		}

		else if (Keyboard_IsPress(DIK_D))
		{
			if (Keyboard_IsPress(DIK_W))
			{
				g_Player.speed.x = sinf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);
				g_Player.speed.z = cosf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);
			}
				
			g_Player.rot.y += VALUE_ROTATE;
			g_Player.rot.z += VALUE_ROTATE;

			if (g_Player.rot.y > D3DX_PI)
			{
				g_Player.rot.y -= D3DX_PI * 2.0f;
			}
			if (g_Player.rot.z > D3DX_PI / 4)
			{
				g_Player.rot.z = D3DX_PI / 4;
			}
		}

		if (Keyboard_IsPress(DIK_W))
		{
			g_Player.speed.x = sinf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);
			g_Player.speed.z = cosf(D3DX_PI * 0.0f - g_Player.rot.y) * (g_Player.PlayerSpd / 3 * 4);
		}

		if (Keyboard_IsPress(DIK_S))
		{
			g_Player.speed.x *= 0.75f;
			g_Player.speed.z *= 0.75f;
		}

		if (g_Player.Pattern == 0)
		{
			if (Keyboard_IsPress(DIK_LSHIFT) && GetLockonRange()->LockonCoolDownFlg == false)
			{
				pLockOnRange->bUse = true;
			}

			if (Keyboard_IsRelease(DIK_LSHIFT))
			{
				pLockOnRange->bUse = false;
				//ロックオンしている場合
				for (int i = 0; i < MAX_LOCKON; i++)
				{
					if ((pLock + i)->bUse == true)
					{
						SetLockonBullet(D3DXVECTOR3(g_Player.pos.x - sinf(g_Player.rot.y - 0.5f) * 18.0f,
							0.5f,
							g_Player.pos.z - cosf(g_Player.rot.y - 0.5f) * 18.0f), g_Player.rot.y);
						//PlaySound(SOUND_LABEL_SE_LOCKONBULLET);
					}
				}

				//ロックオンしていない場合
				if (pLockOnRange->LockonFreeFlg)
				{
					for (int i = 0; i < MAX_LOCKON; i++)
					{
						SetLockonBullet(D3DXVECTOR3(g_Player.pos.x - sinf(g_Player.rot.y - 0.5f) * 18.0f,
							0.5f,
							g_Player.pos.z - cosf(g_Player.rot.y - 0.5f) * 18.0f), g_Player.rot.y);
					}
				}
			}			
		}

		//機体パターン1の場合
		if (g_Player.Pattern == 1)
		{
			if (Keyboard_IsPress(DIK_LSHIFT) && pAT->PlusThunderCoolDownFlg == false)
			{
				//アビリティ発動
				pAT->PlusThunder = true;
			}
		}

		if (g_Player.Pattern == 2)
		{

			//ボム2発発射
			if (Keyboard_IsPress(DIK_LSHIFT) && pAB->BombCoolDownFlg == false)
			{
				g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y - 1.0f) * 18.0f;
				g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y - 1.0f) * 18.0f;

				SetBomb(g_Player.BulletBirthPos, D3DXVECTOR3(-sinf(g_Player.rot.y - 0.1f) * 4.0f, 0.0f, -cosf(g_Player.rot.y - 0.1f) * 4.0f));

				g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y + 2.0f) * 18.0f;
				g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y + 2.0f) * 18.0f;

				SetBomb(g_Player.BulletBirthPos, D3DXVECTOR3(-sinf(g_Player.rot.y + 0.1f) * 4.0f, 0.0f, -cosf(g_Player.rot.y + 0.1f) * 4.0f));
				pAB->BombUse = true;
			}
		}
	   
		if (g_Player.rot.z > 0 && !Keyboard_IsPress(DIK_D))
		{
			g_Player.rot.z -= (VALUE_ROTATE / 2);
		}
		if (g_Player.rot.z < 0 && !Keyboard_IsPress(DIK_A))
		{
			g_Player.rot.z += (VALUE_ROTATE / 2);
		}


		// 座標の更新
		g_Player.rot.y = g_Player.rot.y + cam->rot.y;
		g_Player.pos.x += g_Player.speed.x;
		g_Player.pos.z -= g_Player.speed.z;

		g_Player.col_Sphere.pos.x = g_Player.pos.x;
		g_Player.col_Sphere.pos.y = g_Player.pos.y;
		g_Player.col_Sphere.pos.z = g_Player.pos.z;

		g_Player.col_Sphere.r = 25.0f;

		g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y) * 18.0f;
		g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y) * 18.0f;
		
		if (g_Player.Pattern == 2)
		{
			g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y - 0.5f) * 18.0f;
			g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y - 0.5f) * 18.0f;
		}


		// カメラの注視点と視点を主人公に追従させる
		cam->posR.x = g_Player.pos.x;
		cam->posR.z = g_Player.pos.z;
		cam->posV.x = cam->posR.x - sinf(g_Player.rot.y + D3DX_PI) * cam->fDistance;
		cam->posV.z = cam->posR.z - cosf(g_Player.rot.y + D3DX_PI) * cam->fDistance;

		if (Keyboard_IsPress(DIK_C))
		{
			cam->posV.x = cam->posR.x - sinf(g_Player.rot.y) * cam->fDistance;
			cam->posV.z = cam->posR.z - cosf(g_Player.rot.y) * cam->fDistance;
		}

		if (Keyboard_IsPress(DIK_RSHIFT))
		{
			g_BulletCnt++;
			if (g_BulletCnt > 9)
			{
				g_BulletCnt = 0;
			}
		}
		if (!Keyboard_IsPress(DIK_RSHIFT))
		{
			g_BulletCnt = 0;			
		}

		if (g_BulletCnt % 10 == 9 || Keyboard_IsTrigger(DIK_RSHIFT))
		{
			PlaySound(SOUND_LABEL_SE_SHOT);
			SetBullet(g_Player.BulletBirthPos, D3DXVECTOR3(-sinf(g_Player.rot.y) * 5.0f, 0.0f, -cosf(g_Player.rot.y) * 5.0f));
			if (g_Player.Pattern == 2)
			{
				g_Player.BulletBirthPos.x = g_Player.pos.x - sinf(g_Player.rot.y + 0.5f) * 18.0f;
				g_Player.BulletBirthPos.z = g_Player.pos.z - cosf(g_Player.rot.y + 0.5f) * 18.0f;

				SetBullet(g_Player.BulletBirthPos, D3DXVECTOR3(-sinf(g_Player.rot.y) * 5.0f, 0.0f, -cosf(g_Player.rot.y) * 5.0f));
			}
		}

		// 影の座標を更新する
		D3DXVECTOR3 shadowPos = g_Player.pos;
		shadowPos.y = 0.0f;
		SetPositionShadow(g_Player.idxShadow, D3DXVECTOR3(g_Player.pos.x, 0.0f, g_Player.pos.z));
		SetScaleShadow(g_Player.idxShadow, D3DXVECTOR3(50.0f - (g_Player.pos.y / 2), 0.0f, 50.0f - (g_Player.pos.y / 2)));
	
		if (g_Player.life > (g_FullLife / 10 * 5))
		{
			g_Player.HpColor = 3;
		}

		if (g_Player.life < (g_FullLife / 10 * 5))
		{
			g_Player.HpColor = 2;
		}

		if (g_Player.life < (g_FullLife / 10 * 2))
		{
			g_Player.HpColor = 1;
		}

		//=======デバッグ用=======================
		/*
		if (Keyboard_IsTrigger(DIK_1))
		{
			g_Player.Pattern = 0;
		}
		if (Keyboard_IsTrigger(DIK_2))
		{
			g_Player.Pattern = 1;
		}
		if (Keyboard_IsTrigger(DIK_3))
		{
			g_Player.Pattern = 2;
		}
		*/
		//========================================
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyD3D_GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	if (g_Player.bUse == true)
	{

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player.mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);


		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMat[g_Player.Pattern]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMat[g_Player.Pattern]; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定(テクスチャの有るモデルを読み込むときは注意)
			pDevice->SetTexture(0, g_pTexture);

			// 描画
			g_pMesh[g_Player.Pattern]->DrawSubset(nCntMat);
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	/*
	//==============HPバー===========================================================================
	Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
		SCREEN_WIDTH - 300.0f,											//描画座標x
		10.0f,															//描画座標y
		0, 0,															//テクスチャ切り取り座標x,y
		10 * g_Player.life, 10,											//テクスチャ切り取り幅x,y
		10.0f * 0.5f,													//中心座標x
		10.0f * 0.5f,													//中心座標y
		3.0f,															//拡大率x方向
		3.0f,															//拡大率y方向
		0.0f,															//回転角度(ラジアン)
		g_Player.HpColor);												//描画する色
		*/
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
PLAYER* GetPlayer()
{
	return &(g_Player);
}

SPHERE* Player_GetSphere(void)
{
	return (&g_Player.col_Sphere);
}
void Player_Destroy(void)
{
	g_Player.bUse = false;
	ReleaseShadow(g_Player.idxShadow);
}
void Player_Damage(int damage)
{
	g_Player.life -= damage;
}

void Player_AddLife(int life)
{
	g_Player.life += life;
	if (g_Player.life > g_FullLife)
	{
		g_Player.life = g_FullLife;
	}
}

bool Player_IsEnable(void)
{
	return g_Player.bUse;
}
