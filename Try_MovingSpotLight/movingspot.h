#ifndef _MOVINGSPOT_H_
#define _MOVINGSPOT_H_

#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")

extern IDirect3DDevice9 *pD3DDEV;
extern ID3DXMesh *pMsh[4];

bool Setup();
bool Render(float DeltaTime);
void Cleanup();

#endif