#ifndef _D3DXCREATE_H_
#define _D3DXCREATE_H_

#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")

extern LPDIRECT3DDEVICE9 pD3DDEV;

bool Setup();
bool Render(float DeltaTime);
void Cleanup();

#endif