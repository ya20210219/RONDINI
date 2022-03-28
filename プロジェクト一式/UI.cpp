//=============================================================================
//
// UI���� [UI.cpp]
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
// �}�N����`
//*****************************************************************************

#define	TEXTURE_RADAR		"data/TEXTURE/HPbar.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_RADAR_MAIN	"data/TEXTURE/Radar_main.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RADAR_WIDTH					(5.0f)					// ���[�_�[�̓_�̕�
#define	RADAR_HEIGHT				(5.0f)					// ���[�_�[�̓_�̍���
#define RADAR_MAIN_WIDHT			(250.0f)
#define RADAR_MAIN_HEIGHT			(250.0f)
#define MAX_RADAR					(MAX_ENEMY + MAX_BOSS + MAX_FRIEND)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR2		pos;		// �ʒu
	float			rot;		// ��]
	D3DXVECTOR2		scale;		// �X�P�[��
	D3DXVECTOR2		dir;		// �ړ���

	bool			bUse;		// �g�p���Ă��邩�ǂ����̃t���O

} RADAR;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

static RADAR					g_Radar[MAX_RADAR];			//���[�_�[���[�N
static D3DXVECTOR2				g_BirthPos;
static float					g_range;
static int						g_DrawCnt = 0;
static int						g_DrawNum = 0;

void InitUI(void)
{
	//  ���[�_�[�z��̏�����
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

	//�G�l�~�[�p
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

	//�{�X�p
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

	//�F�R�p
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

		Sprite_Draw(TEXTURE_INDEX_RONDINI_TUTORIAL_TEXT,					//�e�N�X�`���Ǘ��ԍ�
			0.0f,															//�`����Wx
			0.0f,															//�`����Wy
			(int)0.0f, (int)(TTth / 4 * g_DrawNum),							//�e�N�X�`���؂�����Wx,y
			TTtw, TTth / 4,													//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.75f,															//�g�嗦x����
			0.75f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F

		Sprite_Draw(TEXTURE_INDEX_RONDINI_TUTORIAL_TEXT,					//�e�N�X�`���Ǘ��ԍ�
			(float)SCREEN_WIDTH / 2 - (TTtw / 4),									//�`����Wx
			(float)SCREEN_HEIGHT - (TTth / 4 / 2),									//�`����Wy
			(int)0.0f, (int)(TTth / 4 * 3),									//�e�N�X�`���؂�����Wx,y
			TTtw, TTth / 4,													//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.5f,															//�g�嗦x����
			0.5f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F

		int NTtw = Texture_GetWidth(TEXTURE_INDEX_RONDINI_NEXT_TEXT);
		int NTth = Texture_GetHeight(TEXTURE_INDEX_RONDINI_NEXT_TEXT);

		//���ւ̃��b�Z�[�W
		Sprite_Draw(TEXTURE_INDEX_RONDINI_NEXT_TEXT,						//�e�N�X�`���Ǘ��ԍ�
			(float)SCREEN_WIDTH - (NTtw / 2),										//�`����Wx
			0.0f,															//�`����Wy
			(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
			NTtw, NTth,														//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.5f,															//�g�嗦x����
			0.5f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F
	}

	if (GetSceneNumber() == SCENE_INDEX_SELECT)
	{
		int TEXTtw = Texture_GetWidth(TEXTURE_INDEX_RONDINI_TEXT);
		int TEXTth = Texture_GetHeight(TEXTURE_INDEX_RONDINI_TEXT);

		int STATUStw = Texture_GetWidth(TEXTURE_INDEX_RONDINI_STATUS_TEXT);
		int STATUSth = Texture_GetHeight(TEXTURE_INDEX_RONDINI_STATUS_TEXT);

		Sprite_Draw(TEXTURE_INDEX_RONDINI_TEXT,								//�e�N�X�`���Ǘ��ԍ�
			0.0f,															//�`����Wx
			0.0f,															//�`����Wy
			(int)0.0f, (int)(TEXTth / 3 * SelectPlayer()),					//�e�N�X�`���؂�����Wx,y
			TEXTtw, TEXTth / 3,												//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.75f,															//�g�嗦x����
			0.75f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F


		Sprite_Draw(TEXTURE_INDEX_RONDINI_SKILL_TEXT,						//�e�N�X�`���Ǘ��ԍ�
			0.0f,															//�`����Wx
			200.0f,															//�`����Wy
			(int)0.0f, (int)(TEXTth / 3 * SelectPlayer()),					//�e�N�X�`���؂�����Wx,y
			TEXTtw, TEXTth / 3,												//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.75f,															//�g�嗦x����
			0.75f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F

		Sprite_Draw(TEXTURE_INDEX_RONDINI_STATUS_TEXT,						//�e�N�X�`���Ǘ��ԍ�
			0.0f,															//�`����Wx
			SCREEN_HEIGHT - 300,											//�`����Wy
			(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
			STATUStw, STATUSth,												//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			1.0f,															//�g�嗦x����
			1.0f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F

		//HP
		Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
			120.0f,															//�`����Wx
			SCREEN_HEIGHT - 185,											//�`����Wy
			(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
			10 * pSelect->life / 4, 10,										//�e�N�X�`���؂��蕝x,y
			10.0f * 0.5f,													//���S���Wx
			10.0f * 0.5f,													//���S���Wy
			2.0f,															//�g�嗦x����
			2.0f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			1);																//�`�悷��F

		//ATK
		Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
			120.0f,															//�`����Wx
			SCREEN_HEIGHT - 125,											//�`����Wy
			(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
			10 * pSelect->atk / 3, 10,										//�e�N�X�`���؂��蕝x,y
			10.0f * 0.5f,													//���S���Wx
			10.0f * 0.5f,													//���S���Wy
			2.0f,															//�g�嗦x����
			2.0f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			3);																//�`�悷��F

		//SPD
		Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
			120.0f,															//�`����Wx
			SCREEN_HEIGHT - 65,												//�`����Wy
			(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
			10 * pSelect->spd / 3, 10,										//�e�N�X�`���؂��蕝x,y
			10.0f * 0.5f,													//���S���Wx
			10.0f * 0.5f,													//���S���Wy
			2.0f,															//�g�嗦x����
			2.0f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			5);																//�`�悷��F

		//�~�b�V�����T�v
		Sprite_Draw(TEXTURE_INDEX_RONDINI_STAGE_TEXT,						//�e�N�X�`���Ǘ��ԍ�
			SCREEN_WIDTH - 620,												//�`����Wx
			0.0f,															//�`����Wy
			(int)0.0f, (int)(870 / 4 * GetClearCnt()),						//�e�N�X�`���؂�����Wx,y
			820, (870 / 4),													//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.75f,															//�g�嗦x����
			0.75f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F

		//AD�ŃL�����ϊ�������
		Sprite_Draw(TEXTURE_INDEX_AD,										//�e�N�X�`���Ǘ��ԍ�
			SCREEN_WIDTH / 2,												//�`����Wx
			SCREEN_HEIGHT - 45,												//�`����Wy
			(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
			330, 180,														//�e�N�X�`���؂��蕝x,y
			330 / 2,														//���S���Wx
			180 / 2,														//���S���Wy
			0.5f,															//�g�嗦x����
			0.5f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F

		//���ւ̃��b�Z�[�W
		Sprite_Draw(TEXTURE_INDEX_RONDINI_NEXT_TEXT,						//�e�N�X�`���Ǘ��ԍ�
			(float)SCREEN_WIDTH - 300,										//�`����Wx
			SCREEN_HEIGHT - 50,												//�`����Wy
			(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
			600, 100,														//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.5f,															//�g�嗦x����
			0.5f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F
	}

	if (pPlayer->bUse == true)
	{ 
		//���ւ̃��b�Z�[�W
		Sprite_Draw(TEXTURE_INDEX_PLAYERGAUGE,								//�e�N�X�`���Ǘ��ԍ�
			0.0f,															//�`����Wx
			SCREEN_HEIGHT - 100,												//�`����Wy
			(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
			800, 200,														//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			0.5f,															//�g�嗦x����
			0.5f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			0);																//�`�悷��F

		//HP�o�[
		Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
			60.0f,															//�`����Wx
			SCREEN_HEIGHT - 31.0f,											//�`����Wy
			0, 0,															//�e�N�X�`���؂�����Wx,y
			10 * pPlayer->life, 10,											//�e�N�X�`���؂��蕝x,y
			10.0f * 0.5f,													//���S���Wx
			10.0f * 0.5f,													//���S���Wy
			0.5f,															//�g�嗦x����
			2.0f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			pPlayer->HpColor);												//�`�悷��F

		if (g_DrawCnt == -1)
		{
			if (GetSceneNumber() != SCENE_INDEX_GAME3)
			{
				//�~�b�V�����l�[��
				Sprite_Draw(TEXTURE_INDEX_BOSSHP,									//�e�N�X�`���Ǘ��ԍ�
					SCREEN_WIDTH / 2,												//�`����Wx
					25.0f,															//�`����Wy
					0, 0,															//�e�N�X�`���؂�����Wx,y
					700, 50,														//�e�N�X�`���؂��蕝x,y
					700 * 0.5f,														//���S���Wx
					50 * 0.5f,														//���S���Wy
					1.0f,															//�g�嗦x����
					1.0f,															//�g�嗦y����
					0.0f,															//��]�p�x(���W�A��)
					0);																//�`�悷��F
			}


			for (int i = 0; i < MAX_BOSS; i++)
			{
				if (GetSceneNumber() == SCENE_INDEX_GAME)
				{
					//�{�XHP�o�[
					Sprite_Draw(TEXTURE_INDEX_HPBAR,															//�e�N�X�`���Ǘ��ԍ�
						(float)(SCREEN_WIDTH / 3 + 50) - ((pBoss + i)->MaxLife / ((pBoss + i)->MaxLife) / 36),	//�`����Wx
						15.0f,																					//�`����Wy
						0, 0,																					//�e�N�X�`���؂�����Wx,y
						//(10 * pBoss->Life / (pBoss->MaxLife / 50)), 10,										//�e�N�X�`���؂��蕝x,y
						(10 * pBoss->Life / 10), 10,															//�e�N�X�`���؂��蕝x,y
						0.0f,																					//���S���Wx
						0.0f,																					//���S���Wy
						0.5f,																					//�g�嗦x����
						2.0f,																					//�g�嗦y����
						0.0f,																					//��]�p�x(���W�A��)
						(pBoss + i)->HpColor);																	//�`�悷��F
				}
				if (GetSceneNumber() == SCENE_INDEX_GAME2)
				{
					//�{�XHP�o�[
					Sprite_Draw(TEXTURE_INDEX_HPBAR,															//�e�N�X�`���Ǘ��ԍ�
						(float)(SCREEN_WIDTH / 3 + 50) - ((pBoss + i)->MaxLife / ((pBoss + i)->MaxLife) / 50),	//�`����Wx
						15.0f,																					//�`����Wy
						0, 0,																					//�e�N�X�`���؂�����Wx,y
						//(10 * ((pBoss + i)->Life / (pBoss->MaxLife / 50))), 10,								//�e�N�X�`���؂��蕝x,y
						(10 * pBoss->Life / 10), 10,															//�e�N�X�`���؂��蕝x,y
						0.0f,																					//���S���Wx
						0.0f,																					//���S���Wy
						0.5f,																					//�g�嗦x����
						2.0f,																					//�g�嗦y����
						0.0f,																					//��]�p�x(���W�A��)
						(pBoss + i)->HpColor);																	//�`�悷��F
				}
				if (GetSceneNumber() == SCENE_INDEX_GAME4)
				{
					//�{�XHP�o�[																				
					Sprite_Draw(TEXTURE_INDEX_HPBAR,															//�e�N�X�`���Ǘ��ԍ�
						(float)(SCREEN_WIDTH / 3 + 50) - ((pBoss + i)->MaxLife / ((pBoss + i)->MaxLife) / 72),		//�`����Wx
						15.0f,																					//�`����Wy
						0, 0,																					//�e�N�X�`���؂�����Wx,y
						//(10 * ((pBoss + i)->Life / (pBoss->MaxLife / 50))), 10,								//�e�N�X�`���؂��蕝x,y
						(10 * pBoss->Life / 10), 10,															//�e�N�X�`���؂��蕝x,y
						0.0f,																					//���S���Wx
						0.0f,																					//���S���Wy
						0.5f,																					//�g�嗦x����
						2.0f,																					//�g�嗦y����
						0.0f,																					//��]�p�x(���W�A��)
						(pBoss + i)->HpColor);																	//�`�悷��F
				}
			}
		}


		//�A�r���e�B�o�[
		if (pPlayer->Pattern == 0)
		{
			//�A�r���e�B�o�[(��)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
				60.0f,															//�`����Wx
				SCREEN_HEIGHT - 65.0f,											//�`����Wy
				0, 0,															//�e�N�X�`���؂�����Wx,y
				10 * 33, 10,													//�e�N�X�`���؂��蕝x,y
				10.0f * 0.5f,													//���S���Wx
				10.0f * 0.5f,													//���S���Wy
				0.5f,															//�g�嗦x����
				2.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				1);

			//�A�r���e�B�o�[(���F)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
				60.0f,															//�`����Wx
				SCREEN_HEIGHT - 65.0f,											//�`����Wy
				0, 0,															//�e�N�X�`���؂�����Wx,y
				10 * (pLockon->LockonCoolDownCnt / 6), 10,							//�e�N�X�`���؂��蕝x,y
				10.0f * 0.5f,													//���S���Wx
				10.0f * 0.5f,													//���S���Wy
				0.5f,															//�g�嗦x����
				2.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				6);
		}

		//�A�r���e�B�o�[
		if (pPlayer->Pattern == 1)
		{
			//�A�r���e�B�o�[(��)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
				60.0f,															//�`����Wx
				SCREEN_HEIGHT - 65.0f,											//�`����Wy
				0, 0,															//�e�N�X�`���؂�����Wx,y
				10 * 50, 10,													//�e�N�X�`���؂��蕝x,y
				10.0f * 0.5f,													//���S���Wx
				10.0f * 0.5f,													//���S���Wy
				0.5f,															//�g�嗦x����
				2.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				1);

			//�A�r���e�B�o�[(���F)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
				60.0f,															//�`����Wx
				SCREEN_HEIGHT - 65.0f,											//�`����Wy
				0, 0,															//�e�N�X�`���؂�����Wx,y
				10 * (pAT->PlusThunderCnt / 6), 10,							//�e�N�X�`���؂��蕝x,y
				10.0f * 0.5f,													//���S���Wx
				10.0f * 0.5f,													//���S���Wy
				0.5f,															//�g�嗦x����
				2.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				6);
		}		

		//�A�r���e�B�o�[
		if (pPlayer->Pattern == 2)
		{
			//�A�r���e�B�o�[(��)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
				60.0f,															//�`����Wx
				SCREEN_HEIGHT - 65.0f,											//�`����Wy
				0, 0,															//�e�N�X�`���؂�����Wx,y
				10 * 60, 10,													//�e�N�X�`���؂��蕝x,y
				10.0f * 0.5f,													//���S���Wx
				10.0f * 0.5f,													//���S���Wy
				0.5f,															//�g�嗦x����
				2.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				1);

			//�A�r���e�B�o�[(���F)
			Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
				60.0f,															//�`����Wx
				SCREEN_HEIGHT - 65.0f,											//�`����Wy
				0, 0,															//�e�N�X�`���؂�����Wx,y
				10 * (pAB->BombCoolDownCnt / 5), 10,							//�e�N�X�`���؂��蕝x,y
				10.0f * 0.5f,													//���S���Wx
				10.0f * 0.5f,													//���S���Wy
				0.5f,															//�g�嗦x����
				2.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				6);
		}
	}

	if (GetSceneNumber() != SCENE_INDEX_SELECT)
	{
		//���[�_�[
		Sprite_Draw(TEXTURE_INDEX_RADAR,									//�e�N�X�`���Ǘ��ԍ�
			g_BirthPos.x,													//�`����Wx
			g_BirthPos.y,													//�`����Wy
			0, 0,															//�e�N�X�`���؂�����Wx,y
			250, 250,														//�e�N�X�`���؂��蕝x,y
			0.0f,															//���S���Wx
			0.0f,															//���S���Wy
			1.0f,															//�g�嗦x����
			1.0f,															//�g�嗦y����
			0.0f,															//��]�p�x(���W�A��)
			4);																//�`�悷��F
	}

	if (g_DrawCnt != -1 && GetSceneNumber() != SCENE_INDEX_RESULT && GetSceneNumber() != SCENE_INDEX_SELECT && GetSceneNumber() != SCENE_INDEX_TUTORIAL)
	{
		if (GetSceneNumber() == SCENE_INDEX_GAME)
		{
			//�~�b�V�����l�[��
			Sprite_Draw(TEXTURE_INDEX_RONDINI_MISSION_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				SCREEN_WIDTH / 2,												//�`����Wx
				25.0f,															//�`����Wy
				0, 0,															//�e�N�X�`���؂�����Wx,y
				700, 50,														//�e�N�X�`���؂��蕝x,y
				700 * 0.5f,														//���S���Wx
				50 * 0.5f,														//���S���Wy
				1.0f,															//�g�嗦x����
				1.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetSceneNumber() == SCENE_INDEX_GAME2)
		{
			//�~�b�V�����l�[��
			Sprite_Draw(TEXTURE_INDEX_RONDINI_MISSION_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				SCREEN_WIDTH / 2,												//�`����Wx
				25.0f,															//�`����Wy
				0, 50,															//�e�N�X�`���؂�����Wx,y
				700, 50,														//�e�N�X�`���؂��蕝x,y
				700 * 0.5f,														//���S���Wx
				50 * 0.5f,														//���S���Wy
				1.0f,															//�g�嗦x����
				1.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetSceneNumber() == SCENE_INDEX_GAME3)
		{
			//�~�b�V�����l�[��
			Sprite_Draw(TEXTURE_INDEX_RONDINI_MISSION_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				SCREEN_WIDTH / 2,												//�`����Wx
				25.0f,															//�`����Wy
				0, 100,															//�e�N�X�`���؂�����Wx,y
				700, 50,														//�e�N�X�`���؂��蕝x,y
				700 * 0.5f,														//���S���Wx
				50 * 0.5f,														//���S���Wy
				1.0f,															//�g�嗦x����
				1.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetSceneNumber() == SCENE_INDEX_GAME4)
		{
			//�~�b�V�����l�[��
			Sprite_Draw(TEXTURE_INDEX_RONDINI_MISSION_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				SCREEN_WIDTH / 2,												//�`����Wx
				25.0f,															//�`����Wy
				0, 150,															//�e�N�X�`���؂�����Wx,y
				700, 50,														//�e�N�X�`���؂��蕝x,y
				700 * 0.5f,														//���S���Wx
				50 * 0.5f,														//���S���Wy
				1.0f,															//�g�嗦x����
				1.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}
	}		

	if (GetSceneNumber() != SCENE_INDEX_SELECT)
	{
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if ((pEnemy + i)->bUse == true && g_Radar[i].bUse == true)
			{
				Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
					g_Radar[i].pos.x,												//�`����Wx
					g_Radar[i].pos.y,												//�`����Wy
					0, 0,															//�e�N�X�`���؂�����Wx,y
					(int)RADAR_WIDTH, (int)RADAR_HEIGHT,							//�e�N�X�`���؂��蕝x,y
					10.0f * 0.5f,													//���S���Wx
					10.0f * 0.5f,													//���S���Wy
					1.0f,															//�g�嗦x����
					1.0f,															//�g�嗦y����
					0.0f,															//��]�p�x(���W�A��)
					1);																//�`�悷��F
			}
		}

		for (int i = MAX_ENEMY; i < MAX_RADAR; i++)
		{
			if ((pBoss + (i - MAX_ENEMY))->bUse == true && g_Radar[i].bUse == true)
			{
				Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
					g_Radar[i].pos.x,												//�`����Wx
					g_Radar[i].pos.y,												//�`����Wy
					0, 0,															//�e�N�X�`���؂�����Wx,y
					(int)RADAR_WIDTH, (int)RADAR_HEIGHT,							//�e�N�X�`���؂��蕝x,y
					10.0f * 0.5f,													//���S���Wx
					10.0f * 0.5f,													//���S���Wy
					1.5f,															//�g�嗦x����
					1.5f,															//�g�嗦y����
					0.0f,															//��]�p�x(���W�A��)
					1);																//�`�悷��F
			}
		}

		for (int i = MAX_ENEMY + MAX_BOSS; i < MAX_RADAR; i++)
		{
			if ((pFriend + (i - (MAX_ENEMY + MAX_BOSS)))->bUse == true && g_Radar[i].bUse == true)
			{
				Sprite_Draw(TEXTURE_INDEX_HPBAR,									//�e�N�X�`���Ǘ��ԍ�
					g_Radar[i].pos.x,												//�`����Wx
					g_Radar[i].pos.y,												//�`����Wy
					0, 0,															//�e�N�X�`���؂�����Wx,y
					(int)RADAR_WIDTH, (int)RADAR_HEIGHT,							//�e�N�X�`���؂��蕝x,y
					10.0f * 0.5f,													//���S���Wx
					10.0f * 0.5f,													//���S���Wy
					1.5f,															//�g�嗦x����
					1.5f,															//�g�嗦y����
					0.0f,															//��]�p�x(���W�A��)
					3);																//�`�悷��F
			}
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME)
	{
		if (GetGame1CleatTimer() > 1 && GetGame1CleatTimer() < 300 && GetGame1DirectingFlg() == true)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				0.0f,															//�`����Wx
				60.0f,															//�`����Wy
				(int)0.0f, (int)(870 / 4 * 0),									//�e�N�X�`���؂�����Wx,y
				820, (870 / 4),													//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				0.75f,															//�g�嗦x����
				0.75f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetGame1CleatTimer() > 181 && GetGame1CleatTimer() < 480 && GetGame1DirectingFlg() == false)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				0.0f,															//�`����Wx
				60.0f,															//�`����Wy
				(int)0.0f, (int)(870 / 4 * 3),									//�e�N�X�`���؂�����Wx,y
				820, (870 / 4),													//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				0.75f,															//�g�嗦x����
				0.75f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetGame1CleatTimer() > 301 && GetGame1CleatTimer() < 480 && GetGame1DirectingFlg() == true)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_CLEAR_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				(SCREEN_WIDTH / 2) - 250,										//�`����Wx
				(SCREEN_HEIGHT / 2) - 25,										//�`����Wy
				(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
				500, 50,														//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				1.0f,															//�g�嗦x����
				1.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME2)
	{
		if (GetGame2CleatTimer() > 1 && GetGame2CleatTimer() < 480)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				0.0f,															//�`����Wx
				60.0f,															//�`����Wy
				(int)0.0f, (int)(870 / 4 * 0),									//�e�N�X�`���؂�����Wx,y
				820, (870 / 4),													//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				0.75f,															//�g�嗦x����
				0.75f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetGame2CleatTimer() > 301 && GetGame2CleatTimer() < 480)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_CLEAR_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				(SCREEN_WIDTH / 2) - 250,										//�`����Wx
				(SCREEN_HEIGHT / 2) - 25,										//�`����Wy
				(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
				500, 50,														//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				1.0f,															//�g�嗦x����
				1.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}
	}
	
	if (GetSceneNumber() == SCENE_INDEX_GAME3)
	{
		if (GetGame3CleatTimer() > 181 && GetGame3CleatTimer() < 480 && GetGame3DirectingFlg() == false)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				0.0f,															//�`����Wx
				60.0f,															//�`����Wy
				(int)0.0f, (int)(870 / 4 * 1),									//�e�N�X�`���؂�����Wx,y
				820, (870 / 4),													//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				0.75f,															//�g�嗦x����
				0.75f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetGame3CleatTimer() > 1 && GetGame3CleatTimer() < 300 && GetGame3DirectingFlg() == true)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				0.0f,															//�`����Wx
				60.0f,															//�`����Wy
				(int)0.0f, (int)(870 / 4 * 0),									//�e�N�X�`���؂�����Wx,y
				820, (870 / 4),													//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				0.75f,															//�g�嗦x����
				0.75f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetGame3CleatTimer() > 301 && GetGame3CleatTimer() < 480 && GetGame3DirectingFlg() == true)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_CLEAR_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				(SCREEN_WIDTH / 2) - 250,										//�`����Wx
				(SCREEN_HEIGHT / 2) - 25,										//�`����Wy
				(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
				500, 50,														//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				1.0f,															//�g�嗦x����
				1.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}
	}

	if (GetSceneNumber() == SCENE_INDEX_GAME4)
	{
		if (GetGame4CleatTimer() > 181 && GetGame4CleatTimer() < 480 && GetGame4DirectingFlg() == false)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				0.0f,															//�`����Wx
				60.0f,															//�`����Wy
				(int)0.0f, (int)(870 / 4 * 2),									//�e�N�X�`���؂�����Wx,y
				820, (870 / 4),													//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				0.75f,															//�g�嗦x����
				0.75f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetGame4CleatTimer() > 1 && GetGame4CleatTimer() < 300 && GetGame4DirectingFlg() == true)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_RONDINI_COMMU_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				0.0f,															//�`����Wx
				60.0f,															//�`����Wy
				(int)0.0f, (int)(870 / 4 * 0),									//�e�N�X�`���؂�����Wx,y
				820, (870 / 4),													//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				0.75f,															//�g�嗦x����
				0.75f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}

		if (GetGame4CleatTimer() > 301 && GetGame4CleatTimer() < 480 && GetGame4DirectingFlg() == true)
		{
			//�~�b�V�����T�v
			Sprite_Draw(TEXTURE_INDEX_CLEAR_TEXT,						//�e�N�X�`���Ǘ��ԍ�
				(SCREEN_WIDTH / 2) - 250,										//�`����Wx
				(SCREEN_HEIGHT / 2) - 25,										//�`����Wy
				(int)0.0f, (int)0.0f,											//�e�N�X�`���؂�����Wx,y
				500, 50,														//�e�N�X�`���؂��蕝x,y
				0.0f,															//���S���Wx
				0.0f,															//���S���Wy
				1.0f,															//�g�嗦x����
				1.0f,															//�g�嗦y����
				0.0f,															//��]�p�x(���W�A��)
				0);																//�`�悷��F
		}
	}
}