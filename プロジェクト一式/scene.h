#pragma once

enum SCENE_INDEX
{
	SCENE_NONE,				//ÉVÅ[ÉìèàóùÇ»Çµ

	SCENE_INDEX_TITLE,
	SCENE_INDEX_TUTORIAL,
	SCENE_INDEX_SELECT,
	SCENE_INDEX_GAME,
	SCENE_INDEX_GAME2,
	SCENE_INDEX_GAME3,
	SCENE_INDEX_GAME4,
	SCENE_INDEX_RESULT,
	SCENE_INDEX_ENDING,
	SCENE_INDEX_PROLOGUE,

	SCENE_INDEX_MAX
};

void Scene_Initialize(SCENE_INDEX index);
void Scene_Finalize(void);
void Scene_Update(void);
void Scene_Draw(void);

void Scene_Change(SCENE_INDEX index);
void Scene_Check(void);
int GetSceneNumber(void);
int GetClearCnt(void);

void AddClearCnt(void);
void ResetClearCnt(void);

