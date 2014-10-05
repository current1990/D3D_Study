#ifndef _ROTATELIGHT_H_
#define _ROTATELIGHT_H_

#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")

extern LPDIRECT3DDEVICE9 pD3DDEV;
extern ID3DXMesh *pMsh;

bool Setup();
bool Render(float DeltaTime);
void Cleanup();

#endif