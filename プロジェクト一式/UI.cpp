//=============================================================================
//
// UI処理 [UI.cpp]
// Author :	Y.Ariyoshi
//
//=============================================================================

#include "sprite.h"
#include "scene.h"
#include "player.h"
#include "MyDirect3D.h"
#include "enemy.h"
#include "camera.h"
#include "scene.h"
#include "Select.h"
#include "Boss.h"
#include "Lockon.h"
#include "AbilityThunder.h"
#include "AbilityBomb.h"
#include "Friend.h"
#include "game.h"
#include "game2.h"
#include "game3.h"
#include "game4.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	TEXTURE_RADAR		"data/TEXTURE/HPbar.png"		// 読み込むテクスチャファイル名
#define TEXTURE_RADAR_MAIN	"data/TEXTURE/Radar_main.png"	// 読み込むテクスチャファイル名
#define	RADAR_WIDTH					(5.0f)					// レーダーの点の幅
#define	RADAR_HEIGHT				(5.0f)					// レーダーの点の高さ
#define RADAR_MAIN_WIDHT			(250.0f)
#define RADAR_MAIN_HEIGHT			(250.0f)
#define MAX_RADAR					(MAX_ENEMY + MAX_BOSS + MAX_FRIEND)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR2		pos;		// 位置
	float			rot;		// 回転
	D3DXVECTOR2		scale;		// スケール
	D3DXVECTOR2		dir;		// 移動量

	bool			bUse;		// 使用しているかどうかのフラグ

} RADAR;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

static RADAR					g_Radar[MAX_RADAR];			//レーダーワーク
static D3DXVECTOR2				g_BirthPos;
static float					g_range;
static int						g_DrawCnt = 0;
static int						g_DrawNum = 0;

void InitUI(void)
{
	//  レーダー配列の初期化
	for (int i = 0; i < MAX_RADAR; i++)
	{
		g_Radar[i].pos = D3DXVECTOR2(0.0f, 0.0f);
		g_Radar[i].rot = 0.0f;
		g_Radar[i].scale = D3DXVECTOR2(1.0f, 1.0f);
		g_Radar[i].dir = D3DXVECTOR2(1.0f, 1.0f);
		g_Radar[i].bUse = false;
	}

	g_range = 900.0f;
	g_DrawCnt = 0;
	g_BirthPos = D3DXVECTOR2(1025, 470);
}

void UninitUI(void)
{
	for (int i = 0; i < MAX_RADAR; i++)
	{
		g_Radar[i].bUse = false;
	}
}

void UpdateUI(void)
{
	ENEMY* pEnemy = GetAllEnemy();
	CAMERA* pCam = GetCamera();
	PLAYER* pPlayer = GetPlayer();
	BOSS* pBoss = GetBoss();
	FRIEND* pFriend = GetAllFriend();

	if (GetSceneNumber() == SCENE_INDEX_TUTORIAL)
	{
		if (g_DrawCnt != -1)
		{
			g_DrawCnt++;
			if (g_DrawCnt % 300 == 299)
			{
				g_DrawNum++;
			}		
		}
		if (g_DrawCnt > 900)
		{
			g_DrawCnt = 0;
			g_DrawNum = 0;
		}
	}

	if (GetSceneNumber() != SCENE_INDEX_TUTORIAL)
	{
		if (g_DrawCnt != -1)
		{
			g_DrawCnt++;
		}
		if (g_DrawCnt > 180)
		{
			g_DrawCnt = -1;
		}
	}

	//エネミー用
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Radar[i].pos.x = (g_BirthPos.x + (RADAR_MAIN_WIDHT / 2)) - sinf(-(pEnemy + i)->rot.y + (pPlayer->rot.y + D3DX_PI)) * ((pEnemy + i)->g_Dist / 8);
		g_Radar[i].pos.y = (g_BirthPos.y + (RADAR_MAIN_HEIGHT / 2)) - cosf(-(pEnemy + i)->rot.y + (pPlayer->rot.y + D3DX_PI)) * ((pEnemy + i)->g_Dist / 8);
		g_Radar[i].rot = (pEnemy + i)->rot.y;
		g_Radar[i].scale = D3DXVECTOR2(1.0f, 1.0f);
		g_Radar[i].dir = D3DXVECTOR2(1.0f, 1.0f);

		if (!(pEnemy + i)->bUse || (pEnemy + i)->g_Dist > g_range)
		{
			g_Radar[i].bUse = false;
		}

		else if ((pEnemy + i)->bUse || (pEnemy + i)->g_Dist < g_range)
		{
			g_Radar[i].bUse = true;
		}
	}

	//ボス用
	for (int i = MAX_ENEMY; i < MAX_ENEMY + MAX_BOSS; i++)
	{
		g_Radar[i].pos.x = (g_BirthPos.x + (RADAR_MAIN_WIDHT / 2)) - sinf(-(pBoss + (i - MAX_ENEMY))->rot.y + (pPlayer->rot.y + D3DX_PI)) * ((pBoss + (i - MAX_ENEMY))->g_Dist / 8);
		g_Radar[i].pos.y = (g_BirthPos.y + (RADAR_MAIN_HEIGHT / 2)) - cosf(-(pBoss + (i - MAX_ENEMY))->rot.y + (pPlayer->rot.y + D3DX_PI)) * ((pBoss + (i - MAX_ENEMY))->g_Dist / 8);
		g_Radar[i].rot = (pBoss + (i - MAX_ENEMY))->rot.y;
		g_Radar[i].scale = D3DXVECTOR2(1.0f, 1.0f);
		g_Radar[i].dir = D3DXVECTOR2(1.0f, 1.0f);

		if (!(pBoss + (i - MAX_ENEMY))->bUse || (pBoss + (i - MAX_ENEMY))->g_Dist > g_range)
		{
			g_Radar[i].bUse = false;
		}

		else if ((pBoss + (i - MAX_ENEMY))->bUse || (pBoss + (i - MAX_ENEMY))->g_Dist < g_range)
		{
			g_Radar[i].bUse = true;
		}
	}

	//友軍用
	for (int i = MAX_ENEMY + MAX_BOSS; i < MAX_RADAR; i++)
	{
		g_Radar[i].pos.x = (g_BirthPos.x + (RADAR_MAIN_WIDHT / 2)) - sinf(-(pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->rotP + (pPlayer->rot.y + D3DX_PI)) * ((pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->g_Dist / 8);
		g_Radar[i].pos.y = (g_BirthPos.y + (RADAR_MAIN_HEIGHT / 2)) - cosf(-(pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->rotP + (pPlayer->rot.y + D3DX_PI)) * ((pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->g_Dist / 8);
		g_Radar[i].rot = (pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->rotP;
		g_Radar[i].scale = D3DXVECTOR2(1.0f, 1.0f);
		g_Radar[i].dir = D3DXVECTOR2(1.0f, 1.0f);

		if (!(pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->bUse || (pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->g_Dist > g_range)
		{
			g_Radar[i].bUse = false;
		}

		else if ((pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->bUse || (pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->g_Dist < g_range)
		{
			g_Radar[i].bUse = true;
		}
	}
}

void DrawUI(void)
{
	PLAYER* pPlayer = GetPlayer();
	ENEMY* pEnemy = GetAllEnemy();
	SELECT* pSelect = GetSelect();
	BOSS* pBoss = GetBoss();
	LOCKONRANGE* pLockon = GetLockonRange();
	AbilityThunder* pAT = GetAbilityThunder();
	BOMBMGR* pAB = GetBombMgr();
	FRIEND* pFriend = GetAllFriend();

	if (GetSceneNumber() == SCENE_INDEX_TUTORIAL)
	{
		int TTtw = Texture_GetWidth(TEXTURE_INDEX_RONDINI_TUTORIAL_TEXT);
		int TTth = Texture_GetHeight(TEXTURE_INDEX_RONDINI_TUTORIAL_TEXT);

		Sprite_Draw(TEXTURE_INDEX_RONDINI_TUTORIAL_TEXT,					//テクスチャ管理番号
			0.0f,															//描画座標x
			0.0f,															//描画座標y
			(int)0.0f, (int)(TTth / 4 * g_DrawNum),							//テクスチャ切り取り座標x,y
			TTtw, TTth / 4,													//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.75f,															//拡大率x方向
			0.75f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色

		Sprite_Draw(TEXTURE_INDEX_RONDINI_TUTORIAL_TEXT,					//テクスチャ管理番号
			(float)SCREEN_WIDTH / 2 - (TTtw / 4),									//描画座標x
			(float)SCREEN_HEIGHT - (TTth / 4 / 2),									//描画座標y
			(int)0.0f, (int)(TTth / 4 * 3),									//テクスチャ切り取り座標x,y
			TTtw, TTth / 4,													//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.5f,															//拡大率x方向
			0.5f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色

		int NTtw = Texture_GetWidth(TEXTURE_INDEX_RONDINI_NEXT_TEXT);
		int NTth = Texture_GetHeight(TEXTURE_INDEX_RONDINI_NEXT_TEXT);

		//次へのメッセージ
		Sprite_Draw(TEXTURE_INDEX_RONDINI_NEXT_TEXT,						//テクスチャ管理番号
			(float)SCREEN_WIDTH - (NTtw / 2),										//描画座標x
			0.0f,															//描画座標y
			(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
			NTtw, NTth,														//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.5f,															//拡大率x方向
			0.5f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色
	}

	if (GetSceneNumber() == SCENE_INDEX_SELECT)
	{
		int TEXTtw = Texture_GetWidth(TEXTURE_INDEX_RONDINI_TEXT);
		int TEXTth = Texture_GetHeight(TEXTURE_INDEX_RONDINI_TEXT);

		int STATUStw = Texture_GetWidth(TEXTURE_INDEX_RONDINI_STATUS_TEXT);
		int STATUSth = Texture_GetHeight(TEXTURE_INDEX_RONDINI_STATUS_TEXT);

		Sprite_Draw(TEXTURE_INDEX_RONDINI_TEXT,								//テクスチャ管理番号
			0.0f,															//描画座標x
			0.0f,															//描画座標y
			(int)0.0f, (int)(TEXTth / 3 * SelectPlayer()),					//テクスチャ切り取り座標x,y
			TEXTtw, TEXTth / 3,												//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.75f,															//拡大率x方向
			0.75f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色


		Sprite_Draw(TEXTURE_INDEX_RONDINI_SKILL_TEXT,						//テクスチャ管理番号
			0.0f,															//描画座標x
			200.0f,															//描画座標y
			(int)0.0f, (int)(TEXTth / 3 * SelectPlayer()),					//テクスチャ切り取り座標x,y
			TEXTtw, TEXTth / 3,												//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.75f,															//拡大率x方向
			0.75f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色

		Sprite_Draw(TEXTURE_INDEX_RONDINI_STATUS_TEXT,						//テクスチャ管理番号
			0.0f,															//描画座標x
			SCREEN_HEIGHT - 300,											//描画座標y
			(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
			STATUStw, STATUSth,												//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			1.0f,															//拡大率x方向
			1.0f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色

		//HP
		Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
			120.0f,															//描画座標x
			SCREEN_HEIGHT - 185,											//描画座標y
			(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
			10 * pSelect->life / 4, 10,										//テクスチャ切り取り幅x,y
			10.0f * 0.5f,													//中心座標x
			10.0f * 0.5f,													//中心座標y
			2.0f,															//拡大率x方向
			2.0f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			1);																//描画する色

		//ATK
		Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
			120.0f,															//描画座標x
			SCREEN_HEIGHT - 125,											//描画座標y
			(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
			10 * pSelect->atk / 3, 10,										//テクスチャ切り取り幅x,y
			10.0f * 0.5f,													//中心座標x
			10.0f * 0.5f,													//中心座標y
			2.0f,															//拡大率x方向
			2.0f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			3);																//描画する色

		//SPD
		Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
			120.0f,															//描画座標x
			SCREEN_HEIGHT - 65,												//描画座標y
			(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
			10 * pSelect->spd / 3, 10,										//テクスチャ切り取り幅x,y
			10.0f * 0.5f,													//中心座標x
			10.0f * 0.5f,													//中心座標y
			2.0f,															//拡大率x方向
			2.0f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			5);																//描画する色

		//ミッション概要
		Sprite_Draw(TEXTURE_INDEX_RONDINI_STAGE_TEXT,						//テクスチャ管理番号
			SCREEN_WIDTH - 620,												//描画座標x
			0.0f,															//描画座標y
			(int)0.0f, (int)(870 / 4 * GetClearCnt()),						//テクスチャ切り取り座標x,y
			820, (870 / 4),													//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.75f,															//拡大率x方向
			0.75f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色

		//ADでキャラ変換させる
		Sprite_Draw(TEXTURE_INDEX_AD,										//テクスチャ管理番号
			SCREEN_WIDTH / 2,												//描画座標x
			SCREEN_HEIGHT - 45,												//描画座標y
			(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
			330, 180,														//テクスチャ切り取り幅x,y
			330 / 2,														//中心座標x
			180 / 2,														//中心座標y
			0.5f,															//拡大率x方向
			0.5f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色

		//次へのメッセージ
		Sprite_Draw(TEXTURE_INDEX_RONDINI_NEXT_TEXT,						//テクスチャ管理番号
			(float)SCREEN_WIDTH - 300,										//描画座標x
			SCREEN_HEIGHT - 50,												//描画座標y
			(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
			600, 100,														//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.5f,															//拡大率x方向
			0.5f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色
	}

	if (pPlayer->bUse == true)
	{ 
		//次へのメッセージ
		Sprite_Draw(TEXTURE_INDEX_PLAYERGAUGE,								//テクスチャ管理番号
			0.0f,															//描画座標x
			SCREEN_HEIGHT - 100,												//描画座標y
			(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
			800, 200,														//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.5f,															//拡大率x方向
			0.5f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色

		//HPバー
		Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
			60.0f,															//描画座標x
			SCREEN_HEIGHT - 31.0f,											//描画座標y
			0, 0,															//テクスチャ切り取り座標x,y
			10 * pPlayer->life, 10,											//テクスチャ切り取り幅x,y
			10.0f * 0.5f,													//中心座標x
			10.0f * 0.5f,													//中心座標y
			0.5f,															//拡大率x方向
			2.0f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			pPlayer->HpColor);												//描画する色

		if (g_DrawCnt == -1)
		{
			if (GetSceneNumber() != SCENE_INDEX_GAME3)
			{
				//ミッションネーム
				Sprite_Draw(TEXTURE_INDEX_BOSSHP,									//テクスチャ管理番号
					SCREEN_WIDTH / 2,												//描画座標x
					25.0f,															//描画座標y
					0, 0,															//テクスチャ切り取り座標x,y
					700, 50,														//テクスチャ切り取り幅x,y
					700 * 0.5f,														//中心座標x
					50 * 0.5f,														//中心座標y
					1.0f,															//拡大率x方向
					1.0f,															//拡大率y方向
					0.0f,															//回転角度(ラジアン)
					0);																//描画する色
			}


			for (int i = 0; i < MAX_BOSS; i++)
			{
				if (GetSceneNumber() == SCENE_INDEX_GAME)
				{
					//ボスHPバー
					Sprite_Draw(TEXTURE_INDEX_HPBAR,															//テクスチャ管理番号
						(float)(SCREEN_WIDTH / 3 + 50) - ((pBoss + i)->MaxLife / ((pBoss + i)->MaxLife) / 36),	//描画座標x
						15.0f,																					//描画座標y
						0, 0,																					//テクスチャ切り取り座標x,y
						//(10 * pBoss->Life / (pBoss->MaxLife / 50)), 10,										//テクスチャ切り取り幅x,y
						(10 * pBoss->Life / 10), 10,															//テクスチャ切り取り幅x,y
						0.0f,																					//中心座標x
						0.0f,																					//中心座標y
						0.5f,																					//拡大率x方向
						2.0f,																					//拡大率y方向
						0.0f,																					//回転角度(ラジアン)
						(pBoss + i)->HpColor);																	//描画する色
				}
				if (GetSceneNumber() == SCENE_INDEX_GAME2)
				{
					//ボスHPバー
					Sprite_Draw(TEXTURE_INDEX_HPBAR,															//テクスチャ管理番号
						(float)(SCREEN_WIDTH / 3 + 50) - ((pBoss + i)->MaxLife / ((pBoss + i)->MaxLife) / 50),	//描画座標x
						15.0f,																					//描画座標y
						0, 0,																					//テクスチャ切り取り座標x,y
						//(10 * ((pBoss + i)->Life / (pBoss->MaxLife / 50))), 10,								//テクスチャ切り取り幅x,y
						(10 * pBoss->Life / 10), 10,															//テクスチャ切り取り幅x,y
						0.0f,																					//中心座標x
						0.0f,																					//中心座標y
						0.5f,																					//拡大率x方向
						2.0f,																					//拡大率y方向
						0.0f,																					//回転角度(ラジアン)
						(pBoss + i)->HpColor);																	//描画する色
				}
				if (GetSceneNumber() == SCENE_INDEX_GAME4)
				{
					//ボスHPバー																				
					Sprite_Draw(TEXTURE_INDEX_HPBAR,															//テクスチャ管理番号
						(float)(SCREEN_WIDTH / 3 + 50) - ((pBoss + i)->MaxLife / ((pBoss + i)->MaxLife) / 72),		//描画座標x
						15.0f,																					//描画座標y
						0, 0,																					//テクスチャ切り取り座標x,y
						//(10 * ((pBoss + i)->Life / (pBoss->MaxLife / 50))), 10,								//テクスチャ切り取り幅x,y
						(10 * pBoss->Life / 10), 10,															//テクスチャ切り取り幅x,y
						0.0f,																					//中心座標x
						0.0f,																					//中心座標y
						0.5f,																					//拡大率x方向
						2.0f,																					//拡大率y方向
						0.0f,																					//回転角度(ラジアン)
						(pBoss + i)->HpColor);																	//描画する色
				}
			}
		}


		//アビリティバー
		if (pPlayer->Pattern == 0)
		{
			//アビリティバー(赤)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
				60.0f,															//描画座標x
				SCREEN_HEIGHT - 65.0f,											//描画座標y
				0, 0,															//テクスチャ切り取り座標x,y
				10 * 33, 10,													//テクスチャ切り取り幅x,y
				10.0f * 0.5f,													//中心座標x
				10.0f * 0.5f,													//中心座標y
				0.5f,															//拡大率x方向
				2.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				1);

			//アビリティバー(水色)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
				60.0f,															//描画座標x
				SCREEN_HEIGHT - 65.0f,											//描画座標y
				0, 0,															//テクスチャ切り取り座標x,y
				10 * (pLockon->LockonCoolDownCnt / 6), 10,							//テクスチャ切り取り幅x,y
				10.0f * 0.5f,													//中心座標x
				10.0f * 0.5f,													//中心座標y
				0.5f,															//拡大率x方向
				2.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				6);
		}

		//アビリティバー
		if (pPlayer->Pattern == 1)
		{
			//アビリティバー(赤)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
				60.0f,															//描画座標x
				SCREEN_HEIGHT - 65.0f,											//描画座標y
				0, 0,															//テクスチャ切り取り座標x,y
				10 * 50, 10,													//テクスチャ切り取り幅x,y
				10.0f * 0.5f,													//中心座標x
				10.0f * 0.5f,													//中心座標y
				0.5f,															//拡大率x方向
				2.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				1);

			//アビリティバー(水色)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
				60.0f,															//描画座標x
				SCREEN_HEIGHT - 65.0f,											//描画座標y
				0, 0,															//テクスチャ切り取り座標x,y
				10 * (pAT->PlusThunderCnt / 6), 10,							//テクスチャ切り取り幅x,y
				10.0f * 0.5f,													//中心座標x
				10.0f * 0.5f,													//中心座標y
				0.5f,															//拡大率x方向
				2.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				6);
		}		

		//アビリティバー
		if (pPlayer->Pattern == 2)
		{
			//アビリティバー(赤)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
				60.0f,															//描画座標x
				SCREEN_HEIGHT - 65.0f,											//描画座標y
				0, 0,															//テクスチャ切り取り座標x,y
				10 * 60, 10,													//テクスチャ切り取り幅x,y
				10.0f * 0.5f,													//中心座標x
				10.0f * 0.5f,													//中心座標y
				0.5f,															//拡大率x方向
				2.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				1);

			//アビリティバー(水色)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
				60.0f,															//描画座標x
				SCREEN_HEIGHT - 65.0f,											//描画座標y
				0, 0,															//テクスチャ切り取り座標x,y
				10 * (pAB->BombCoolDownCnt / 5), 10,							//テクスチャ切り取り幅x,y
				10.0f * 0.5f,													//中心座標x
				10.0f * 0.5f,													//中心座標y
				0.5f,															//拡大率x方向
				2.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				6);
		}
	}

	if (GetSceneNumber() != SCENE_INDEX_SELECT)
	{
		//レーダー
		Sprite_Draw(TEXTURE_INDEX_RADAR,									//テクスチャ管理番号
			g_BirthPos.x,													//描画座標x
			g_BirthPos.y,													//描画座標y
			0, 0,															//テクスチャ切り取り座標x,y
			250, 250,														//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			1.0f,															//拡大率x方向
			1.0f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			4);																//描画する色
	}

	if (g_DrawCnt != -1 && GetSceneNumber() != SCENE_INDEX_RESULT && GetSceneNumber() != SCENE_INDEX_SELECT && GetSceneNumber() != SCENE_INDEX_TUTORIAL)
	{
		if (GetSceneNumber() == SCENE_INDEX_GAME)
		{
			//ミッションネーム
			Sprite_Draw(TEXTURE_INDEX_RONDINI_MISSION_TEXT,						//テクスチャ管理番号
				SCREEN_WIDTH / 2,												//描画座標x
				25.0f,															//描画座標y
				0, 0,															//テクスチャ切り取り座標x,y
				700, 50,														//テクスチャ切り取り幅x,y
				700 * 0.5f,														//中心座標x
				50 * 0.5f,														//中心座標y
				1.0f,															//拡大率x方向
				1.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetSceneNumber() == SCENE_INDEX_GAME2)
		{
			//ミッションネーム
			Sprite_Draw(TEXTURE_INDEX_RONDINI_MISSION_TEXT,						//テクスチャ管理番号
				SCREEN_WIDTH / 2,												//描画座標x
				25.0f,															//描画座標y
				0, 50,															//テクスチャ切り取り座標x,y
				700, 50,														//テクスチャ切り取り幅x,y
				700 * 0.5f,														//中心座標x
				50 * 0.5f,														//中心座標y
				1.0f,															//拡大率x方向
				1.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetSceneNumber() == SCENE_INDEX_GAME3)
		{
			//ミッションネーム
			Sprite_Draw(TEXTURE_INDEX_RONDINI_MISSION_TEXT,						//テクスチャ管理番号
				SCREEN_WIDTH / 2,												//描画座標x
				25.0f,															//描画座標y
				0, 100,															//テクスチャ切り取り座標x,y
				700, 50,														//テクスチャ切り取り幅x,y
				700 * 0.5f,														//中心座標x
				50 * 0.5f,														//中心座標y
				1.0f,															//拡大率x方向
				1.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetSceneNumber() == SCENE_INDEX_GAME4)
		{
			//ミッションネーム
			Sprite_Draw(TEXTURE_INDEX_RONDINI_MISSION_TEXT,						//テクスチャ管理番号
				SCREEN_WIDTH / 2,												//描画座標x
				25.0f,															//描画座標y
				0, 150,															//テクスチャ切り取り座標x,y
				700, 50,														//テクスチャ切り取り幅x,y
				700 * 0.5f,														//中心座標x
				50 * 0.5f,														//中心座標y
				1.0f,															//拡大率x方向
				1.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}
	}		

	if (GetSceneNumber() != SCENE_INDEX_SELECT)
	{
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if ((pEnemy + i)->bUse == true && g_Radar[i].bUse == true)
			{
				Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
					g_Radar[i].pos.x,												//描画座標x
					g_Radar[i].pos.y,												//描画座標y
					0, 0,															//テクスチャ切り取り座標x,y
					(int)RADAR_WIDTH, (int)RADAR_HEIGHT,							//テクスチャ切り取り幅x,y
					10.0f * 0.5f,													//中心座標x
					10.0f * 0.5f,													//中心座標y
					1.0f,															//拡大率x方向
					1.0f,															//拡大率y方向
					0.0f,															//回転角度(ラジアン)
					1);																//描画する色
			}
		}

		for (int i = MAX_ENEMY; i < MAX_RADAR; i++)
		{
			if ((pBoss + (i - MAX_ENEMY))->bUse == true && g_Radar[i].bUse == true)
			{
				Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
					g_Radar[i].pos.x,												//描画座標x
					g_Radar[i].pos.y,												//描画座標y
					0, 0,															//テクスチャ切り取り座標x,y
					(int)RADAR_WIDTH, (int)RADAR_HEIGHT,							//テクスチャ切り取り幅x,y
					10.0f * 0.5f,													//中心座標x
					10.0f * 0.5f,													//中心座標y
					1.5f,															//拡大率x方向
					1.5f,															//拡大率y方向
					0.0f,															//回転角度(ラジアン)
					1);																//描画する色
			}
		}

		for (int i = MAX_ENEMY + MAX_BOSS; i < MAX_RADAR; i++)
		{
			if ((pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->bUse == true && g_Radar[i].bUse == true)
			{
				Sprite_Draw(TEXTURE_INDEX_HPBAR,									//テクスチャ管理番号
					g_Radar[i].pos.x,												//描画座標x
					g_Radar[i].pos.y,												//描画座標y
					0, 0,															//テクスチャ切り取り座標x,y
					(int)RADAR_WIDTH, (int)RADAR_HEIGHT,							//テクスチャ切り取り幅x,y
					10.0f * 0.5f,													//中心座標x
					10.0f * 0.5f,													//中心座標y
					1.5f,															//拡大率x方向
					1.5f,															//拡大率y方向
					0.0f,															//回転角度(ラジアン)
					3);																//描画する色
			}
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME)
	{
		if (GetGame1CleatTimer() > 1 && GetGame1CleatTimer() < 300 && GetGame1DirectingFlg() == true)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//テクスチャ管理番号
				0.0f,															//描画座標x
				60.0f,															//描画座標y
				(int)0.0f, (int)(870 / 4 * 0),									//テクスチャ切り取り座標x,y
				820, (870 / 4),													//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				0.75f,															//拡大率x方向
				0.75f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetGame1CleatTimer() > 181 && GetGame1CleatTimer() < 480 && GetGame1DirectingFlg() == false)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//テクスチャ管理番号
				0.0f,															//描画座標x
				60.0f,															//描画座標y
				(int)0.0f, (int)(870 / 4 * 3),									//テクスチャ切り取り座標x,y
				820, (870 / 4),													//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				0.75f,															//拡大率x方向
				0.75f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetGame1CleatTimer() > 301 && GetGame1CleatTimer() < 480 && GetGame1DirectingFlg() == true)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_CLEAR_TEXT,						//テクスチャ管理番号
				(SCREEN_WIDTH / 2) - 250,										//描画座標x
				(SCREEN_HEIGHT / 2) - 25,										//描画座標y
				(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
				500, 50,														//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				1.0f,															//拡大率x方向
				1.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME2)
	{
		if (GetGame2CleatTimer() > 1 && GetGame2CleatTimer() < 480)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//テクスチャ管理番号
				0.0f,															//描画座標x
				60.0f,															//描画座標y
				(int)0.0f, (int)(870 / 4 * 0),									//テクスチャ切り取り座標x,y
				820, (870 / 4),													//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				0.75f,															//拡大率x方向
				0.75f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetGame2CleatTimer() > 301 && GetGame2CleatTimer() < 480)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_CLEAR_TEXT,						//テクスチャ管理番号
				(SCREEN_WIDTH / 2) - 250,										//描画座標x
				(SCREEN_HEIGHT / 2) - 25,										//描画座標y
				(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
				500, 50,														//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				1.0f,															//拡大率x方向
				1.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}
	}
	
	if (GetSceneNumber() == SCENE_INDEX_GAME3)
	{
		if (GetGame3CleatTimer() > 181 && GetGame3CleatTimer() < 480 && GetGame3DirectingFlg() == false)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//テクスチャ管理番号
				0.0f,															//描画座標x
				60.0f,															//描画座標y
				(int)0.0f, (int)(870 / 4 * 1),									//テクスチャ切り取り座標x,y
				820, (870 / 4),													//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				0.75f,															//拡大率x方向
				0.75f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetGame3CleatTimer() > 1 && GetGame3CleatTimer() < 300 && GetGame3DirectingFlg() == true)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//テクスチャ管理番号
				0.0f,															//描画座標x
				60.0f,															//描画座標y
				(int)0.0f, (int)(870 / 4 * 0),									//テクスチャ切り取り座標x,y
				820, (870 / 4),													//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				0.75f,															//拡大率x方向
				0.75f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetGame3CleatTimer() > 301 && GetGame3CleatTimer() < 480 && GetGame3DirectingFlg() == true)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_CLEAR_TEXT,						//テクスチャ管理番号
				(SCREEN_WIDTH / 2) - 250,										//描画座標x
				(SCREEN_HEIGHT / 2) - 25,										//描画座標y
				(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
				500, 50,														//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				1.0f,															//拡大率x方向
				1.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME4)
	{
		if (GetGame4CleatTimer() > 181 && GetGame4CleatTimer() < 480 && GetGame4DirectingFlg() == false)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//テクスチャ管理番号
				0.0f,															//描画座標x
				60.0f,															//描画座標y
				(int)0.0f, (int)(870 / 4 * 2),									//テクスチャ切り取り座標x,y
				820, (870 / 4),													//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				0.75f,															//拡大率x方向
				0.75f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetGame4CleatTimer() > 1 && GetGame4CleatTimer() < 300 && GetGame4DirectingFlg() == true)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//テクスチャ管理番号
				0.0f,															//描画座標x
				60.0f,															//描画座標y
				(int)0.0f, (int)(870 / 4 * 0),									//テクスチャ切り取り座標x,y
				820, (870 / 4),													//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				0.75f,															//拡大率x方向
				0.75f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}

		if (GetGame4CleatTimer() > 301 && GetGame4CleatTimer() < 480 && GetGame4DirectingFlg() == true)
		{
			//ミッション概要
			Sprite_Draw(TEXTURE_INDEX_CLEAR_TEXT,						//テクスチャ管理番号
				(SCREEN_WIDTH / 2) - 250,										//描画座標x
				(SCREEN_HEIGHT / 2) - 25,										//描画座標y
				(int)0.0f, (int)0.0f,											//テクスチャ切り取り座標x,y
				500, 50,														//テクスチャ切り取り幅x,y
				0.0f,															//中心座標x
				0.0f,															//中心座標y
				1.0f,															//拡大率x方向
				1.0f,															//拡大率y方向
				0.0f,															//回転角度(ラジアン)
				0);																//描画する色
		}
	}
}