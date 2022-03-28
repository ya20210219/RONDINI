#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "Score.h"
#include "sound.h"

static int  g_EndingTime;
static int  g_EndingMoveTime;
static int  g_EpiNum;
static int  g_CreditPosY;
static bool g_EndScoreFlg;

void Ending_Initialize(void)
{
	g_EndingTime = 0;
	g_EndingMoveTime = 0;
	g_EpiNum = 0;
	g_EndingMoveTime;g_EndScoreFlg = false;
	g_EpiNum;g_CreditPosY = SCREEN_HEIGHT;
	g_CreditPosY;PlaySound(SOUND_LABEL_ENDINGBGM);

}

void Ending_Finalize(void)
{
	SetScore(0);
	StopSound(SOUND_LABEL_ENDINGBGM);
}

void Ending_Update(void)
{
	UpdateScore();

	if (g_EndingTime != -1)
	{
		g_EndingTime++;
	}
	
	if (g_EndingTime % 180 == 0)
	{
		g_EpiNum++;
		if (g_EpiNum > 5)
		{
			g_EpiNum = 5;
		}
	}
	if (g_EndingTime > 1250)
	{
		g_EndingTime = -1;
	}

	if (g_EndingTime == -1)
	{
		g_CreditPosY--;
		if (g_CreditPosY < -1250)
		{
			g_EndScoreFlg = true;
		}
		
	}

	//スペースキーが押されたらシーン遷移する
	if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_SPACE) || g_CreditPosY < -1800)
	{
		if (Fade_GetState() == FADE_STATE_NONE)
		{
			Fade_SceneTransition(SCENE_INDEX_TITLE);
			ResetClearCnt();
		}
	}
}

void Ending_Draw(void)
{	
	Sprite_Draw(TEXTURE_INDEX_ENDING, 0.0f, 0.0f, 0);

	if (g_EndingTime < 1200 && g_EndingTime != -1)
	{
		Sprite_Draw(TEXTURE_INDEX_EPILOGUE,									//テクスチャ管理番号
			SCREEN_WIDTH / 2,												//描画座標x
			SCREEN_HEIGHT / 2,												//描画座標y
			0, 0,															//テクスチャ切り取り座標x,y
			850, (200 / 5) * g_EpiNum,										//テクスチャ切り取り幅x,y
			850 / 2,														//中心座標x
			200 / 2,														//中心座標y
			1.0f,															//拡大率x方向
			1.0f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色
	}

	if (g_EndingTime == -1)
	{
		Sprite_Draw(TEXTURE_INDEX_CREDIT,									//テクスチャ管理番号
			SCREEN_WIDTH / 4,												//描画座標x
			g_CreditPosY,													//描画座標y
			0, 0,															//テクスチャ切り取り座標x,y
			1800, 2500,														//テクスチャ切り取り幅x,y
			0.0f,															//中心座標x
			0.0f,															//中心座標y
			0.5f,															//拡大率x方向
			0.5f,															//拡大率y方向
			0.0f,															//回転角度(ラジアン)
			0);																//描画する色
	}

	if (g_EndScoreFlg == true)
	{
		DrawScore();
		Sprite_Draw(TEXTURE_INDEX_SCORE_TEXT, SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2, 0);
	}
}

bool GetEndScoreFlg()
{
	return g_EndScoreFlg;
}
