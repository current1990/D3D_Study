#include "d3dUtility.h"
#include "render.h"

#pragma comment(lib, "d3dUtility.lib")
using namespace d3d;

IDirect3DDevice9 *pD3DDEV = NULL;
IDirect3DVertexBuffer9 *pVB = NULL;
IDirect3DIndexBuffer9 *pIB = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int iCmdShow)
{
	if( !D3DInit(hInstance, 800, 600, 1, D3DDEVTYPE_HAL, TEXT("RotatingCube"), &pD3DDEV) )
	{
		::MessageBox(0, TEXT("D3DInit Failure"), TEXT("ERROR"), MB_OK);
		return 0;
	}

	if( !Setup() )
	{
		::MessageBox(0, TEXT("Setup Failure"), TEXT("ERROR"), MB_OK);
		return 0;
	}

	D3DMsgLoop(Render);

	CleanUp();


	return 0;
}