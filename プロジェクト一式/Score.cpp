#include "main.h"
#include "Score.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"
#include "scene.h"
#include "ending.h"

//グローバル変数-------------------------------------------------//

static int g_Score;

static Score g_ScorePos[MAX_SCORE];

//初期化-------------------------------------------------//
void InitScore()
{
	if (GetSceneNumber() == SCENE_INDEX_GAME)
	{
		g_Score = 0;
	}

	for (int i = 0; i < MAX_SCORE; i++)
	{
		g_ScorePos[i].ScorePosX = SCORE_POS_X;
		g_ScorePos[i].ScorePosY = SCORE_POS_Y;
		g_ScorePos[i].DrawScoreNumX = 0;
		g_ScorePos[i].DrawScoreNumY = 0;
	}
}

void UninitScore()
{

}

//更新-------------------------------------------------//
void UpdateScore()
{
	PLAYER* pPlayer = GetPlayer();

	//ゲームオーバーになったら、リザルト時のポジションへ移動させる
	if (pPlayer->bUse == false)
	{
		for (int i = 0; i < MAX_SCORE; i++)
		{
			g_ScorePos[i].ScorePosX = SCORE_POS_X - ((SCREEN_WIDTH / 3) + (NUM_SIZE_X / 2));
			g_ScorePos[i].ScorePosY = SCORE_POS_Y + (SCREEN_HEIGHT / 2);
			g_ScorePos[i].DrawScoreNumX = 0;
			g_ScorePos[i].DrawScoreNumY = 0;
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_ENDING)
	{
		for (int i = 0; i < MAX_SCORE; i++)
		{
			g_ScorePos[i].ScorePosX = SCORE_POS_X - ((SCREEN_WIDTH / 3) + (NUM_SIZE_X / 2)) - 25;
			g_ScorePos[i].ScorePosY = SCORE_POS_Y + (SCREEN_HEIGHT / 3 * 2);
			g_ScorePos[i].DrawScoreNumX = 0;
			g_ScorePos[i].DrawScoreNumY = 0;
		}
	}
}

//描画-------------------------------------------------//
void DrawScore()
{
	//計算用変数にスコアを入れる
	int score = g_Score;  //計算用変数

	for (int i = 0; i < MAX_SCORE; i++)
	{
		int num = 0;				    //分割した値を入れ
		g_ScorePos[i].DrawScoreNumY = 0;

		num = score % 10;				//スコアの1の位を抜き出す
		score /= 10;					//スコアを1の位ずらしている

		if (num >= 5)
		{
			g_ScorePos[i].DrawScoreNumY = 1;
			num -= 5;
		}

		g_ScorePos[i].DrawScoreNumX = num;

		int tw = Texture_GetWidth(TEXTURE_INDEX_NUMBER) / 5;
		int th = Texture_GetHeight(TEXTURE_INDEX_NUMBER) / 2;

		Sprite_Draw(TEXTURE_INDEX_NUMBER,						//テクスチャ管理番号
			g_ScorePos[i].ScorePosX - (NUM_SIZE_X * i),			//描画座標x
			g_ScorePos[i].ScorePosY,							//描画座標y
			g_ScorePos[i].DrawScoreNumX * NUM_SIZE_X,			//テクスチャ切り取り座標x
			(int)(NUM_SIZE_Y * g_ScorePos[i].DrawScoreNumY),	//テクスチャ切り取り座標y
			(int)(NUM_SIZE_X), (int)(NUM_SIZE_Y),				//テクスチャ切り取り幅x,y
			tw * 0.5f,											//中心座標x
			th * 0.5f,											//中心座標y
			1.0f,												//拡大率x方向
			1.0f,												//拡大率y方向
			0.0f,												//回転角度(ラジアン)
			0);
	}

	//上限上回った時
	if (g_Score > 99999)
	{
		g_Score = 99999;
	}
}

//スコアの増加関数
void AddScore(int add)
{
	g_Score += add;
}

//スコアの設定関数
void SetScore(int set)
{
	g_Score += set;
}

int ScoreCount()
{
	return g_Score;
}
