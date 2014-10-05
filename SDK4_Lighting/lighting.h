#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")

extern LPDIRECT3DDEVICE9 pD3DDEV;
extern IDirect3DVertexBuffer9 *pVB;

bool Setup();
bool Render(float DeltaTime);
void Cleanup();

#endif