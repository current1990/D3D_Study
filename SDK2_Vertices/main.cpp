#include "d3dUtility.h"
#include "vertices.h"

using namespace d3d;

IDirect3DDevice9 *pD3DDEV = NULL;
IDirect3DVertexBuffer9 *pVB = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int iCmdShow)
{
	if( !D3DInit(hInstance, 800, 600, true, D3DDEVTYPE_HAL, TEXT("Vertices"), &pD3DDEV) )
	{
		::MessageBox(0, TEXT("D3DInit Failure"), TEXT("ERROR"), MB_OK);
		return 0;
	}

	if( !Setup() )
	{
		::MessageBox(0, TEXT("Setup Failuer"), TEXT("ERROR"), MB_OK);
		return 0;
	}


	d3d::D3DMsgLoop(Render);

	CleanUp();

	return 0;
}
	