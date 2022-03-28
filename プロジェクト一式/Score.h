#pragma once
//-------------------------------------------------//
//[score.h]スコア
//                      author:俺   date:2020/06/09
//-------------------------------------------------//

#include <d3dx9.h>

#define MAX_SCORE (5)
#define NUM_SIZE_X (256 / 5)
#define NUM_SIZE_Y (50.0f)
#define SCORE_POS_X (SCREEN_WIDTH - (NUM_SIZE_X / 4 * MAX_SCORE))        //スコアを表示する場所のx
#define SCORE_POS_Y ((SCREEN_HEIGHT - SCREEN_HEIGHT) + (NUM_SIZE_Y / 2))  //スコアを表示する場所のy

typedef struct Score
{
	float ScorePosX;
	float ScorePosY;
	int   DrawScoreNumX;
	int   DrawScoreNumY;
}Score;

//プロトタイプ宣言
void InitScore();
void UninitScore();
void UpdateScore();
void DrawScore();
void SetScore(int num);
void AddScore(int add);
void SetScore(int set);
void SetScorePos(D3DXVECTOR2 pos);
int ScoreCount();
