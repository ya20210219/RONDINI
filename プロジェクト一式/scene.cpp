#include "scene.h"

//全シーンのヘッダーをインクルードする
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "game2.h"
#include "game3.h"
#include "game4.h"
#include "result.h"
#include "Select.h"
#include "ending.h"
#include "prologue.h"

static SCENE_INDEX g_SceneIndex;		//現在実行されているシーン
static SCENE_INDEX g_SceneNextIndex;	//切り替え先のシーン

static int		   g_ClearCnt = 0;


void Scene_Initialize(SCENE_INDEX index)
{
	g_SceneNextIndex = g_SceneIndex = index;
	
	switch (g_SceneIndex) 
	{
	case SCENE_NONE:
		break;

	case SCENE_INDEX_TITLE:
		Title_Initialize();
		break;

	case SCENE_INDEX_TUTORIAL:
		Tutorial_Initialize();
		break;

	case SCENE_INDEX_SELECT:
		InitSelect();
		break;

	case SCENE_INDEX_GAME:
		Game_Initialize();
		break;

	case SCENE_INDEX_GAME2:
		Game2_Initialize();
		break;

	case SCENE_INDEX_GAME3:
		Game3_Initialize();
		break;

	case SCENE_INDEX_GAME4:
		Game4_Initialize();
		break;

	case SCENE_INDEX_RESULT:
		Result_Initialize();
		break;

	case SCENE_INDEX_ENDING:
		Ending_Initialize();
		break;
	
	case SCENE_INDEX_PROLOGUE:
		Prologue_Initialize();
		break;
	}
}

void Scene_Finalize(void)
{
	switch( g_SceneIndex )
	{
	case SCENE_NONE:
		break;

	case SCENE_INDEX_TITLE:
		Title_Finalize();
		break;

	case SCENE_INDEX_TUTORIAL:
		Tutorial_Finalize();
		break;

	case SCENE_INDEX_SELECT:
		UninitSelect();
		break;

	case SCENE_INDEX_GAME:
		Game_Finalize();
		break;

	case SCENE_INDEX_GAME2:
		Game2_Finalize();
		break;

	case SCENE_INDEX_GAME3:
		Game3_Finalize();
		break;

	case SCENE_INDEX_GAME4:
		Game4_Finalize();
		break;

	case SCENE_INDEX_RESULT:
		Result_Finalize();
		break;

	case SCENE_INDEX_ENDING:
		Ending_Finalize();
		break;

	case SCENE_INDEX_PROLOGUE:
		Prologue_Finalize();
		break;
	}
}

void Scene_Update(void)
{
	switch( g_SceneIndex )
	{
	case SCENE_NONE:
		break;

	case SCENE_INDEX_TITLE:
		Title_Update();
		break;

	case SCENE_INDEX_TUTORIAL:
		Tutorial_Update();
		break;

	case SCENE_INDEX_SELECT:
		UpdateSelect();
		break;

	case SCENE_INDEX_GAME:
		Game_Update();
		break;

	case SCENE_INDEX_GAME2:
		Game2_Update();
		break;

	case SCENE_INDEX_GAME3:
		Game3_Update();
		break;

	case SCENE_INDEX_GAME4:
		Game4_Update();
		break;

	case SCENE_INDEX_RESULT:
		Result_Update();
		break;
	
	case SCENE_INDEX_ENDING:
		Ending_Update();
		break;

	case SCENE_INDEX_PROLOGUE:
		Prologue_Update();
		break;
	
	}

	Fade_Update();
}

void Scene_Draw(void)
{
	switch( g_SceneIndex )
	{
	case SCENE_NONE:
		break;

	case SCENE_INDEX_TITLE:
		Title_Draw();
		break;

	case SCENE_INDEX_TUTORIAL:
		Tutorial_Draw();
		break;

	case SCENE_INDEX_SELECT:
		DrawSelect();
		break;

	case SCENE_INDEX_GAME:
		Game_Draw();
		break;

	case SCENE_INDEX_GAME2:
		Game2_Draw();
		break;

	case SCENE_INDEX_GAME3:
		Game3_Draw();
		break;

	case SCENE_INDEX_GAME4:
		Game4_Draw();
		break;

	case SCENE_INDEX_RESULT:
		Result_Draw();
		break;
	
	case SCENE_INDEX_ENDING:
		Ending_Draw();
		break;

	case SCENE_INDEX_PROLOGUE:
		Prologue_Draw();
		break;
	}

	Fade_Draw();
}

void Scene_Check(void)
{
	if( g_SceneIndex != g_SceneNextIndex ) 
	{
		//現在のシーンを終了させる
		Scene_Finalize();

		//遷移先シーンの初期化処理を行う
		Scene_Initialize(g_SceneNextIndex);
	}
}

void Scene_Change(SCENE_INDEX index)
{
	g_SceneNextIndex = index;
}

int GetSceneNumber(void)
{
	return g_SceneIndex;
}

int GetClearCnt(void)
{
	return g_ClearCnt;
}

void AddClearCnt(void)
{
	g_ClearCnt++;
}

void ResetClearCnt(void)
{
	g_ClearCnt = 0;
}

