#include "main.h"
#include "Score.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"
#include "scene.h"
#include "ending.h"

//�O���[�o���ϐ�-------------------------------------------------//

static int g_Score;

static Score g_ScorePos[MAX_SCORE];

//������-------------------------------------------------//
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

//�X�V-------------------------------------------------//
void UpdateScore()
{
	PLAYER* pPlayer = GetPlayer();

	//�Q�[���I�[�o�[�ɂȂ�����A���U���g���̃|�W�V�����ֈړ�������
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

//�`��-------------------------------------------------//
void DrawScore()
{
	//�v�Z�p�ϐ��ɃX�R�A������
	int score = g_Score;  //�v�Z�p�ϐ�

	for (int i = 0; i < MAX_SCORE; i++)
	{
		int num = 0;				    //���������l�����
		g_ScorePos[i].DrawScoreNumY = 0;

		num = score % 10;				//�X�R�A��1�̈ʂ𔲂��o��
		score /= 10;					//�X�R�A��1�̈ʂ��炵�Ă���

		if (num >= 5)
		{
			g_ScorePos[i].DrawScoreNumY = 1;
			num -= 5;
		}

		g_ScorePos[i].DrawScoreNumX = num;

		int tw = Texture_GetWidth(TEXTURE_INDEX_NUMBER) / 5;
		int th = Texture_GetHeight(TEXTURE_INDEX_NUMBER) / 2;

		Sprite_Draw(TEXTURE_INDEX_NUMBER,						//�e�N�X�`���Ǘ��ԍ�
			g_ScorePos[i].ScorePosX - (NUM_SIZE_X * i),			//�`����Wx
			g_ScorePos[i].ScorePosY,							//�`����Wy
			g_ScorePos[i].DrawScoreNumX * NUM_SIZE_X,			//�e�N�X�`���؂�����Wx
			(int)(NUM_SIZE_Y * g_ScorePos[i].DrawScoreNumY),	//�e�N�X�`���؂�����Wy
			(int)(NUM_SIZE_X), (int)(NUM_SIZE_Y),				//�e�N�X�`���؂��蕝x,y
			tw * 0.5f,											//���S���Wx
			th * 0.5f,											//���S���Wy
			1.0f,												//�g�嗦x����
			1.0f,												//�g�嗦y����
			0.0f,												//��]�p�x(���W�A��)
			0);
	}

	//�����������
	if (g_Score > 99999)
	{
		g_Score = 99999;
	}
}

//�X�R�A�̑����֐�
void AddScore(int add)
{
	g_Score += add;
}

//�X�R�A�̐ݒ�֐�
void SetScore(int set)
{
	g_Score += set;
}

int ScoreCount()
{
	return g_Score;
}
