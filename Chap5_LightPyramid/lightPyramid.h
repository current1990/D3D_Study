#ifndef _LIGHTPYRAMID_H_
#define _LIGHTPYRAMID_H_

#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")
using namespace d3d;

extern LPDIRECT3DDEVICE9 pD3DDEV;
extern LPDIRECT3DVERTEXBUFFER9 pVB;

bool Setup();
bool Render(float DeltaTime);
void Cleanup();

#endif