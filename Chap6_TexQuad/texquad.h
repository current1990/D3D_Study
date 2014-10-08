#ifndef _TEXQUAD_H_
#define _TEXQUAD_H_

#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")

extern IDirect3DDevice9 *pD3DDEV;
extern IDirect3DVertexBuffer9 *pVB;
extern IDirect3DTexture9 *pTex;

bool Setup();
bool Render(float DeltaTime);
void Cleanup();

#endif