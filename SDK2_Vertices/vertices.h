#ifndef _VERTICES_H_
#define _VERTICES_H_

#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")

extern IDirect3DDevice9 *pD3DDEV;
extern IDirect3DVertexBuffer9 *pVB;

bool Setup();
bool Render(float DeltaTime);
void CleanUp();

#endif