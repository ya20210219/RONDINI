//インクルードガード
#pragma once

#include <windows.h>
#include <d3dx9.h>

bool MyD3D_Initialize(HWND hWnd);
void MyD3D_Finalize(void);

LPDIRECT3DDEVICE9 MyD3D_GetDevice(void);
