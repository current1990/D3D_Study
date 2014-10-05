#ifndef _RENDER_H_
#define _RENDER_H_

#include "d3dUtility.h"


extern IDirect3DDevice9 *pD3DDEV;
extern IDirect3DVertexBuffer9 *pVB;
extern IDirect3DIndexBuffer9 *pIB;

bool Setup();
bool Render(float DeltaTime);
void CleanUp(); 

#endif