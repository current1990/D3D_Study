#include "lighting.h"

using namespace d3d;

LPDIRECT3DDEVICE9 pD3DDEV;
ID3DXMesh *pMsh1;
ID3DXMesh *pMsh2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int iCmdShow)
{
	if( !D3DInit(hInstance, 800, 600, 1, D3DDEVTYPE_HAL, TEXT("Rotating Teapot"), &pD3DDEV) )
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
	Cleanup();

	return 0;
}