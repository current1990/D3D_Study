#include "lightPyramid.h"

LPDIRECT3DDEVICE9 pD3DDEV;
LPDIRECT3DVERTEXBUFFER9 pVB;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int iCmdShow)
{
	if( !D3DInit(hInstance, 800, 600, true, D3DDEVTYPE_HAL, TEXT("Pyramid"), &pD3DDEV) )
	{
		::MessageBox(0, TEXT("D3DInitFailure"), TEXT("ERROR"), MB_OK);
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