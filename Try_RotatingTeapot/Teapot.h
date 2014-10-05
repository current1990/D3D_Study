#ifndef _TEAPOT_H_
#define _TEAPOT_H_

#include "d3dUtility.h"
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dUtility.lib")

using namespace d3d;

extern IDirect3DDevice9 *pD3DDEV;
extern ID3DXMesh *pMesh;

bool Setup();
bool Render(float DeltaTime);
void Cleanup();

#endif