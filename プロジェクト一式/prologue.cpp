#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "Score.h"
#include "sound.h"
#include "debugproc.h"

static int g_PrologueTimer;
static int g_ProNum;

void Prologue_Initialize(void)
{
	g_PrologueTimer = 0;
	g_ProNum = 0;
	PlaySound(SOUND_LABEL_PROLOGUEBGM);
}

void Prologue_Finalize(void)
{
	StopSound(SOUND_LABEL_PROLOGUEBGM);
}

void Prologue_Update(void)
{
	g_PrologueTimer++;

	if (g_PrologueTimer % 120 == 0)
	{
		g_ProNum++;
		if (g_ProNum > 8)
		{
			g_ProNum = 8;
		}
	}

	//スペースキーが押されたらシーン遷移する
	if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_SPACE) || g_PrologueTimer > 1200)
	{
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			Fade_SceneTransition(SCENE_INDEX_TUTORIAL);
		}
	}
}

void Prologue_Draw(void)
{
	if (g_PrologueTimer < 120)
	{
		Sprite_Draw(TEXTURE_INDEX_BACKGROUND_PROLOGUE, 0, 0, 0);
	}
	if (g_PrologueTimer > 120)
	{
		Sprite_Draw(TEXTURE_INDEX_BACKGROUND_PROLOGUE, 0, 0, 4);
	}

	Sprite_Draw(TEXTURE_INDEX_PROLOGUE,									//テクスチャ管理番号
		500,															//描画座標x
		160,															//描画座標y
		0, 0,															//テクスチャ切り取り座標x,y
		1000, (320 / 8) * g_ProNum,										//テクスチャ切り取り幅x,y
		1000 / 2,														//中心座標x
		320 / 2,														//中心座標y
		1.0f,															//拡大率x方向
		1.0f,															//拡大率y方向
		0.0f,															//回転角度(ラジアン)
		0);																//描画する色

	//PrintDebugProc("%d\n", g_PrologueTimer);
}
