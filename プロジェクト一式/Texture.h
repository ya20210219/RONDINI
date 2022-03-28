/*==============================================================================

   テクスチャ管理モジュール [texture.h]
                                                         Author : Youhei Sato
                                                         Date   : 2018/06/16
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include <d3d9.h>


/*------------------------------------------------------------------------------
   テクスチャ列挙型
------------------------------------------------------------------------------*/
// テクスチャ管理番号
typedef enum
{
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_TUTORIAL,
	TEXTURE_INDEX_RESULT,
	TEXTURE_INDEX_FIELD,
	TEXTURE_INDEX_BULLET,
	TEXTURE_INDEX_SHADOW,
	TEXTURE_INDEX_BACKFIRE,
	TEXTURE_INDEX_HPBAR,
	TEXTURE_INDEX_RADAR,
	TEXTURE_INDEX_TEPPAN01,
	TEXTURE_INDEX_RONDINI_TEXT,
	TEXTURE_INDEX_RONDINI_STATUS_TEXT,
	TEXTURE_INDEX_RONDINI_SKILL_TEXT,
	TEXTURE_INDEX_RONDINI_MISSION_TEXT,
	TEXTURE_INDEX_RONDINI_TUTORIAL_TEXT,
	TEXTURE_INDEX_RONDINI_NEXT_TEXT,
	TEXTURE_INDEX_NUMBER,
	TEXTURE_INDEX_RONDINI_STAGE_TEXT,
	TEXTURE_INDEX_RONDINI_COMMU_TEXT,
	TEXTURE_INDEX_CLEAR_TEXT,
	TEXTURE_INDEX_ENDING,
	TEXTURE_INDEX_EPILOGUE,
	TEXTURE_INDEX_CREDIT,
	TEXTURE_INDEX_PROLOGUE,
	TEXTURE_INDEX_BACKGROUND_PROLOGUE,
	TEXTURE_INDEX_AD,
	TEXTURE_INDEX_PRESSENTER,
	TEXTURE_INDEX_BOSSHP,
	TEXTURE_INDEX_PLAYERGAUGE,
	TEXTURE_INDEX_SCORE_TEXT,

    TEXTURE_INDEX_MAX
}TextureIndex;


/*------------------------------------------------------------------------------
   関数のプロトタイプ宣言
------------------------------------------------------------------------------*/

// テクスチャの読み込み
//
// 戻り値:読み込めなかった数
//
int Texture_Load(void);

// テクスチャの解放
void Texture_Release(void);

// テクスチャインターフェースの取得
//
// 引数 ... index テクスチャ管理番号
//
// 戻り値:テクスチャインターフェース
//        ただし存在しないindexを指定した場合、NULL
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index);

// テクスチャ解像度幅の取得
//
// 引数 ... index テクスチャ管理番号
//
int Texture_GetWidth(TextureIndex index);

// テクスチャ解像度高さの取得
//
// 引数 ... index テクスチャ管理番号
//
int Texture_GetHeight(TextureIndex index);
